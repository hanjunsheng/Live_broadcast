#include "stdafx.h"
#include "MyEncode.h"
#include "Archor.h"
#include "ArchorDlg.h"
#include "AbsIMediator.h"

CMyEncode::CMyEncode(void)
{
	pFormatCtx = NULL;
	fmt = NULL;
	video_st=  NULL;
	pCodecCtx = NULL;
	pCodec = NULL;
	picture_buf = NULL;
	picture = NULL;
	in_file = NULL;
	out_file = NULL;
	m_w = 0;
	m_h = 0;
	framenum = 0;
	inputfilepath = NULL;
	out_file = NULL;
	m_malv = 3500000;
}

CMyEncode::~CMyEncode(void)
{
}

void CMyEncode::InitialEncode()
{
	//宽高
	this->m_w = GetSystemMetrics(SM_CXSCREEN);
	this->m_h = GetSystemMetrics(SM_CYSCREEN);	

	this->framenum = 1;

	this->inputfilepath = "../TempFile/output1.yuv";
	this->out_file = "../TempFile/src01.h264";

	//输出文件路径
}

bool CMyEncode::StartEncode()
{
	//视频YUV源文件 
	fopen_s(&this->in_file,inputfilepath,"rb");	

	//写在最前面的初始化
	av_register_all();
	//初始化pFormatCtx
	pFormatCtx = avformat_alloc_context();
	//猜格式
	fmt = av_guess_format(NULL,out_file,NULL);
	pFormatCtx->oformat = fmt;

	//注意输出路径
	if (avio_open(&pFormatCtx->pb,out_file,AVIO_FLAG_READ_WRITE) < 0)
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("输出文件打开失败"));
		return false;
	}

	video_st = avformat_new_stream(pFormatCtx,0);
	if (video_st == NULL)
	{
		return false;
	}

	pCodecCtx = video_st->codec;
	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = PIX_FMT_YUV420P;
	pCodecCtx->width = this->m_w;  
	pCodecCtx->height = this->m_h;
	pCodecCtx->time_base.num = 1;  
	pCodecCtx->time_base.den = 25;  
	pCodecCtx->bit_rate = m_malv;  
	pCodecCtx->gop_size = 250;
	//H264
	//pCodecCtx->me_range = 16;
	//pCodecCtx->max_qdiff = 4;
	pCodecCtx->qmin = 10;
	pCodecCtx->qmax = 51;
	//pCodecCtx->qcompress = 0.6;
	//输出格式信息
	av_dump_format(pFormatCtx, 0, out_file, 1);

	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("没有找到合适的编码器"));
		return false;
	}
	if (avcodec_open2(pCodecCtx,pCodec,NULL) < 0)
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("编码器打开失败"));
		return false;
	}

	picture = av_frame_alloc();
	int size = avpicture_get_size(pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);
	picture_buf = (uint8_t *)av_malloc(size);
	avpicture_fill((AVPicture *)picture,picture_buf,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);

	//写文件头
	avformat_write_header(pFormatCtx,NULL);

	AVPacket pkt;
	int y_size = pCodecCtx->width * pCodecCtx->height;
	av_new_packet(&pkt,y_size*3);

	for (int i=0; i<framenum; i++)
	{
		//读入YUV
		if (fread(picture_buf,1,y_size*3/2,in_file) < 0)
		{
			(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("文件读取错误"));
			return false;
		}
		else if(feof(in_file))
		{
			break;
		}
		picture->data[0] = picture_buf;  // 亮度Y
		picture->data[1] = picture_buf+ y_size;  // U 
		picture->data[2] = picture_buf+ y_size*5/4; // V
		//PTS
		picture->pts = i;
		int got_picture = 0;
		//编码
		int ret = avcodec_encode_video2(pCodecCtx, &pkt,picture, &got_picture);
		if(ret < 0)
		{
			(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("编码错误"));
			return false;
		}
		if (got_picture == 1)
		{
			pkt.stream_index = video_st->index;
			ret = av_write_frame(pFormatCtx, &pkt);
			av_free_packet(&pkt);
		}
	}
	//Flush Encoder
	int ret = flush_encoder(pFormatCtx,0);
	if (ret < 0)
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("Flushing encoder failed"));
		return false;
	}

	//写文件尾
	av_write_trailer(pFormatCtx);

	//清理
	if (video_st)
	{
		avcodec_close(video_st->codec);
		av_frame_free(&picture);
		av_free(picture_buf);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);

	fclose(in_file);

	return true;
}

int CMyEncode::flush_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index)
{
	int ret;
	int got_frame;
	AVPacket enc_pkt;

	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities & CODEC_CAP_DELAY))
		return 0;
	while (1) 
	{
		//ret = encode_write_frame(NULL, stream_index, &got_frame);
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_video2(fmt_ctx->streams[stream_index]->codec,&enc_pkt,NULL,&got_frame);
		av_frame_free(NULL);
		if (ret < 0)
			break;
		if (!got_frame)
		{
			ret=0;
			break;
		}
		/* mux encoded frame */
		ret = av_write_frame(fmt_ctx,&enc_pkt);
		if (ret < 0)
			break;
	}

	return ret;
}

void CMyEncode::Setmalv(int malv)
{
	this->m_malv = malv;
}
