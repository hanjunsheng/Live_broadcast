#include "stdafx.h"
#include "MyDecodevideo.h"
#include "Audience.h"
#include "AudienceDlg.h"

CMyDecodevideo::CMyDecodevideo(void)
{
	//视频
	pFormatCtx = NULL;
	pCodecCtx = NULL;
	pCodec = NULL;
	filepath = NULL;
	pFrame = NULL;
	pFrameYUV = NULL;
	out_buffer = NULL;
	gWindow = NULL;
	sdlTexture = NULL;
	sdlRenderer = NULL;
	packet = NULL;

	gWindowsmall = NULL;
	sdlRenderersmall = NULL;
	gWindowfull = NULL;
	sdlRendererfull = NULL;
}

CMyDecodevideo::~CMyDecodevideo(void)
{
}

bool CMyDecodevideo::DecodeVideo(const char * filepathstr)
{
	//加载文件
	this->filepath = filepathstr;

	//初始化所有
	av_register_all();
	avformat_network_init();
	this->pFormatCtx = avformat_alloc_context();

	if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL) != 0)
	{
		return false;
	}

	if(avformat_find_stream_info(pFormatCtx,NULL) < 0)
	{
		(CAudienceDlg *)theApp.m_pMainWnd->MessageBox(_T("Couldn't find stream information"));
		return false;
	}

	int videoindex = -1;

	for(unsigned int i=0;i<pFormatCtx->nb_streams;i++)
	{
		if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex=i;
			break;
		}
	}

	if(videoindex == -1)
	{
		(CAudienceDlg *)theApp.m_pMainWnd->MessageBox(_T("Didn't find a video stream"));
		return false;
	}

	pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

	if(pCodec == NULL)
	{
		(CAudienceDlg *)theApp.m_pMainWnd->MessageBox(_T("Codec not found"));
		return false;
	}

	if(avcodec_open2(pCodecCtx,pCodec,NULL) < 0)
	{
		(CAudienceDlg *)theApp.m_pMainWnd->MessageBox(_T("Could not open codec"));
		return false;
	}

	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();
	out_buffer = new uint8_t[avpicture_get_size(PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height)];
	avpicture_fill((AVPicture *)pFrameYUV,out_buffer,PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height);

	int ret, got_picture;
	static struct SwsContext *img_convert_ctx;
	int y_size = pCodecCtx->width * pCodecCtx->height;

	this->packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	//av_new_packet(packet,y_size);

	this->sdlTexture = SDL_CreateTexture(sdlRenderer,SDL_PIXELFORMAT_IYUV,SDL_TEXTUREACCESS_STREAMING,pCodecCtx->width,pCodecCtx->height);

	while(av_read_frame(pFormatCtx, packet) >= 0)
	{
		if(packet->stream_index == videoindex)
		{
			while(1)
			{
				ret = avcodec_decode_video2(pCodecCtx,pFrame,&got_picture,packet);
			 	if(ret < 0)
				{
					(CAudienceDlg *)theApp.m_pMainWnd->MessageBox(_T("解码错误v"));
					return false;
				}

				if(got_picture)
				{
					img_convert_ctx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height,PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL); 
					sws_scale(img_convert_ctx,(const uint8_t* const*)pFrame->data,pFrame->linesize,0,pCodecCtx->height,pFrameYUV->data,pFrameYUV->linesize);

					SDL_UpdateTexture(sdlTexture,NULL,pFrameYUV->data[0],pFrameYUV->linesize[0]);
					SDL_RenderClear(sdlRenderer);  
					SDL_RenderCopy(sdlRenderer,sdlTexture,NULL,NULL);
					SDL_RenderPresent(sdlRenderer);
					//延时40ms
					//SDL_Delay(40);
					((CAudienceDlg *)theApp.m_pMainWnd)->m_cmaindlg.Move();
					((CAudienceDlg *)theApp.m_pMainWnd)->m_cmaindlg.Show();
					break;
				}
			}
		}
		av_free_packet(packet);
	}


	SDL_DestroyTexture(this->sdlTexture);
	this->sdlTexture = NULL;

	delete[] out_buffer;
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	// Close the codec
	avcodec_close(pCodecCtx);

	avformat_close_input(&pFormatCtx);

	avformat_free_context(pFormatCtx);

	return true;
}

void CMyDecodevideo::SDLInitial()
{
//------------SDL------------------------------------------------------------------------------------------------
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) 
	{
		(CAudienceDlg *)theApp.m_pMainWnd->MessageBox(_T("初始化SDL失败!"));
		return ;
	}

	int screen_w = GetSystemMetrics(SM_CXSCREEN) / 2;
	int screen_h = GetSystemMetrics(SM_CYSCREEN) / 2;

	//------------小窗口----------------------------------------------------------------------------------------------------------------
	gWindowsmall = SDL_CreateWindowFrom((void *)(((CAudienceDlg *)theApp.m_pMainWnd)->m_cmaindlg.GetDlgItem(IDC_PICTURE)->GetSafeHwnd()));
	if(!gWindowsmall)
	{  
		(CAudienceDlg *)theApp.m_pMainWnd->MessageBox(_T("播放窗口初始化失败"));
		return ;
	}

	this->sdlRenderersmall = SDL_CreateRenderer(gWindowsmall,-1,0);
	//----------------------------------------------------------------------------------------------------------------------------------

	//--------------全屏----------------------------------------------------------------------------------------------------------------
	gWindowfull = SDL_CreateWindowFrom((void *)(((CAudienceDlg *)theApp.m_pMainWnd)->m_cfulldlg.m_picture.GetSafeHwnd()));
	if(!gWindowfull)
	{  
		(CAudienceDlg *)theApp.m_pMainWnd->MessageBox(_T("播放窗口初始化失败"));
		return ;
	}

	this->sdlRendererfull = SDL_CreateRenderer(gWindowfull,-1,0);
	//----------------------------------------------------------------------------------------------------------------------------------

	this->gWindow = gWindowsmall;
	this->sdlRenderer = sdlRenderersmall;

	//设置SDL_Rect 大小
	this->rect.x = 0;
	this->rect.y = 0;
	this->rect.w = screen_w; 
	this->rect.h = screen_h;
//-----------------------------------------------------------------------------------------------------------
}

void CMyDecodevideo::UnInitial()
{
	SDL_DestroyWindow(gWindowsmall);
	SDL_DestroyWindow(gWindowfull);
	gWindowsmall = NULL;
	gWindowfull = NULL;
	SDL_Quit();
}