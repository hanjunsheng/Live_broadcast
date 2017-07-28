#include "stdafx.h"
#include "CollectDesk.h"
#include "Archor.h"
#include "ArchorDlg.h"
#include "AbsIMediator.h"
#include <iostream>
using namespace std;

CCollectDesk::CCollectDesk(void)
{
	pFormatCtx = NULL;
	videoindex = -1;
	pCodecCtx = NULL;
	pCodec = NULL;
	pFrame = NULL;
	pFrameYUV = NULL;
	packet = NULL;
	out_buffer = NULL;
	gWindow = NULL;
	sdlTexture = NULL;
	sdlRenderer = NULL;
	img_convert_ctx = NULL;
	m_bflagshowwindow = false;
	m_bflagvideochange = false;
	thread_exit = false;
	m_bflagfinishvadio = false;
	m_hthread = NULL;

	video_tid = NULL;
	thread_timer_exit = false;
}


CCollectDesk::~CCollectDesk(void)
{
}

bool CCollectDesk::Initial()
{
	if(this->InitialAvcode() == false)
	{
		return false;
	}

	if(this->SetSDL() == false)
	{
		return false;
	}

	//��ʼ��������
	this->myencode.InitialEncode();

	return true;
}

bool CCollectDesk::InitialAvcode()
{
	//ע�Ḵ�������������ȵĺ���
	av_register_all();
	//����������ǰ��Ҫ���Ϻ���avformat_network_init()
	avformat_network_init();
	//AVFormatContext��ͳ��ȫ�ֵĻ����ṹ��
	//��ʼ��������avformat_alloc_context()
	this->pFormatCtx = avformat_alloc_context();
	//��ʹ��libavdevice֮ǰ������������avdevice_register_all()���豸����ע�ᣬ����ͻ����
	avdevice_register_all();

	AVDictionary* options = NULL;
	//��Ҫ���Ȳ�������������豸 libavdecive ֧��win32�µ�GDIGRAB
	AVInputFormat *ifmt=av_find_input_format("gdigrab");
	//�򿪶�ý�����ݲ��һ��һЩ��ص���Ϣ 0 �ɹ�
	if(avformat_open_input(&pFormatCtx,"desktop",ifmt,&options) != 0)
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("�޷���������!"));
		return false;
	}

	//�ú������Զ�ȡһ��������Ƶ���ݲ��һ��һЩ��ص���Ϣ
	if(avformat_find_stream_info(pFormatCtx,NULL) < 0)
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("�޷���ȡ����Ϣ!"));
		return false;
	}
	//���е�����Ƶ��nb_streams
	for(unsigned int i=0; i<pFormatCtx->nb_streams; i++) 
	{
		//codec_type �������������
		if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			break;
		}
	}
	//
	if(videoindex == -1)
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("û���ҵ���Ƶ��!"));
		return false;
	}

	pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	//avcodec_find_encoder()���ڲ���FFmpeg�ı�����
	//avcodec_find_decoder()���ڲ���FFmpeg�Ľ�����
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

	if(pCodec == NULL)
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("û���ҵ�������!"));
		return false;
	}
	//��ʼ��һ������Ƶ���������AVCodecContext
	if(avcodec_open2(pCodecCtx, pCodec,NULL) < 0)
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("�޷��򿪽�����!"));
		return false;
	}

	//AVFrame�ĳ�ʼ��������av_frame_alloc()
	this->pFrame = av_frame_alloc();
	this->pFrameYUV = av_frame_alloc();

	//av_malloc ��װ��malloc �����ڴ�ռ�
	//avpicture_get_size �������ظ�ʽ����Ƶ�ֱ��ʻ�� picture �洢��С
	this->out_buffer = (uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	//��֡��������������ڴ������
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

	this->packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	this->img_convert_ctx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height,PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL); 

	return true;
}

bool CCollectDesk::SetSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) 
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("��ʼ��SDLʧ��!"));
		return false;
	}

	int screen_w = pCodecCtx->width/3;
	int screen_h = pCodecCtx->height/3;

	gWindow = SDL_CreateWindowFrom((void *)(((CArchorDlg *)theApp.m_pMainWnd)->m_cmaindlg.GetDlgItem(IDC_PICTRUE)->GetSafeHwnd()));
	if(!gWindow)
	{  
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("���Ŵ��ڳ�ʼ��ʧ��"));
		return false;
	}

	this->sdlRenderer = SDL_CreateRenderer(gWindow, -1, 0);

	this->sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pCodecCtx->width,pCodecCtx->height);

	//����SDL_Rect ��С
	this->rect.x = 0;
	this->rect.y = 0;
	this->rect.w = screen_w; 
	this->rect.h = screen_h;

	return true;
}

int CCollectDesk::ThreadVideoTimer(void *opaque)
{
	CCollectDesk *pthis = (CCollectDesk *)opaque;
	while (pthis->thread_timer_exit == true) 
	{
		SDL_Event event;
		event.type = SFM_REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}
	return 0;
}

DWORD WINAPI CCollectDesk::ThreadRun(LPVOID lpParameter)
{
	CCollectDesk *pthis = (CCollectDesk *)lpParameter;

	pthis->CollectDesktop();

	return 0;
}

void CCollectDesk::CollectDesktop()
{
	this->thread_exit = true;
	this->thread_timer_exit = true;

	//Event Loop
	SDL_Event event;
	for (int i=0;this->thread_exit == true;i++) 
	{
		//Wait
		SDL_WaitEvent(&event);
		if(event.type == SFM_REFRESH_EVENT)
		{
			if(av_read_frame(pFormatCtx, packet) >= 0)
			{
				if(packet->stream_index == videoindex)
				{
					int got_picture = 0;
					int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
					if(ret < 0)
					{
						(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("���Ŵ��ڳ�ʼ��ʧ��"));
						return ;
					}
					if(got_picture)
					{
						sws_scale(this->img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

#if OUTPUT_YUV420P		
						//���ļ�  д���ļ�
						FILE *fp = NULL;
						fopen_s(&fp,"../TempFile/output1.yuv","wb+");
						if(fp == NULL)
						{
							continue;
						}
						int y_size = pCodecCtx->width * pCodecCtx->height;    
						fwrite(pFrameYUV->data[0],1,y_size,fp);    //Y   
						fwrite(pFrameYUV->data[1],1,y_size/4,fp);  //U  
						fwrite(pFrameYUV->data[2],1,y_size/4,fp);  //V  
						fclose(fp);
						fp = NULL;

						//����
						if(!this->myencode.StartEncode())
						{
							AfxGetMainWnd()->MessageBox(_T("����ʧ��"));
						}

						if(!((CABsIMediator *)((CArchorDlg *)theApp.m_pMainWnd)->GetIMediator())->m_tcp->sendfile())
						{
							(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("send failed"));
						}
						
#endif  
						SDL_UpdateTexture( sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0]);
						SDL_RenderClear( sdlRenderer );    
						SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, NULL);
						SDL_RenderPresent( sdlRenderer );

						//��Ļ
						((CArchorDlg *)theApp.m_pMainWnd)->m_cmaindlg.Move();
						((CArchorDlg *)theApp.m_pMainWnd)->m_cmaindlg.Show();
					}
				}
				av_free_packet(packet);
			}
		}
	}
}


void CCollectDesk::Run()
{
	this->m_hthread = CreateThread(NULL,0,&ThreadRun,this,0,NULL);
	Sleep(100);
	this->video_tid = SDL_CreateThread(ThreadVideoTimer,NULL,this);
}

void CCollectDesk::Close()
{
	//�˳�SDL����ˢ���߳�
	this->thread_exit = false;

	if(WAIT_TIMEOUT == WaitForSingleObject(m_hthread,1000))
	{
		TerminateThread(m_hthread,-1);
	}

	if(this->m_hthread)
	{
		CloseHandle(m_hthread);
		m_hthread = NULL;
	}

	this->thread_timer_exit = false;

	if(this->video_tid)
	{
		this->video_tid = NULL;
	}
}

void CCollectDesk::Destroy()
{
	sws_freeContext(img_convert_ctx);
 
	SDL_Quit();
	av_free(out_buffer);
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_free_context(pFormatCtx);
}
