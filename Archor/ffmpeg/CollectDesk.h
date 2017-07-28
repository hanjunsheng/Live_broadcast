#pragma once
#ifndef COLLECTDESK_H__
#define COLLECTDESK_H__

#include "MyEncode.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
//SDL
#include "sdl/SDL.h"
#include "sdl/SDL_thread.h"
};

#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define OUTPUT_YUV420P 1

class CCollectDesk
{
public:
	CCollectDesk(void);
	~CCollectDesk(void);
//基本结构体
public:
	AVFormatContext	*pFormatCtx;
	int				videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	        *pFrame;
	AVFrame         *pFrameYUV;
	uint8_t         *out_buffer;
	AVPacket        *packet;
	struct SwsContext *img_convert_ctx;
	bool m_bflagvideochange;
//线程相关
public:
	HANDLE m_hthread;
	bool thread_exit;
	static int CCollectDesk::ThreadVideoTimer(void *opaque);
	static DWORD WINAPI ThreadRun(LPVOID lpParameter);
	SDL_Thread *video_tid;
	bool thread_timer_exit;
//SDL相关
public:
	SDL_Window      *gWindow;
	SDL_Texture     *sdlTexture; 
	SDL_Renderer    *sdlRenderer;
	SDL_Rect        rect; 
	bool            m_bflagshowwindow;
//封装函数
public:
	bool Initial();
	bool InitialAvcode();
	bool SetSDL();
	void CollectDesktop();
	void Run();
	void Close();
	void Destroy();
//H264编码类
public:
	CMyEncode myencode;
	static DWORD WINAPI ThreadH264Encode(LPVOID lpParameter);
	bool m_bflagfinishvadio;
};

#endif