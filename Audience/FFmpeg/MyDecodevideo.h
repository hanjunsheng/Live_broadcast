#pragma once
#ifndef MYDECODEVIDEO_H_
#define MYDECODEVIDEO_H_

#include "targetver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <io.h>
#include <direct.h>

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
	//新版里的图像转换结构需要引入的头文件
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
	//SDL
#include "sdl/SDL.h"
#include "sdl/SDL_thread.h"
};

#define MAX_AUDIO_FRAME_SIZE 192000   // 1 second of 48khz 32bit audio


class CMyDecodevideo
{
//视频
public:
	AVFormatContext	*pFormatCtx;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	const char      *filepath;
	AVFrame	        *pFrame;
	AVFrame         *pFrameYUV;
	uint8_t         *out_buffer;
	AVPacket        *packet;
//SDL 相关
public:
	//总的指针
	SDL_Window      *gWindow;
	SDL_Texture     *sdlTexture; 
	SDL_Renderer    *sdlRenderer;
	//小窗口
	SDL_Window      *gWindowsmall;
	SDL_Renderer    *sdlRenderersmall;
	//全屏
	SDL_Window      *gWindowfull;
	SDL_Renderer    *sdlRendererfull;

	SDL_Rect        rect;
public:
	CMyDecodevideo(void);
	~CMyDecodevideo(void);
	bool DecodeVideo(const char * filepathstr);
	void SDLInitial();
	void UnInitial();
};

#endif
