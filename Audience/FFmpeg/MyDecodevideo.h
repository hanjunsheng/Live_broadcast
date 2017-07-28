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
	//�°����ͼ��ת���ṹ��Ҫ�����ͷ�ļ�
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
	//SDL
#include "sdl/SDL.h"
#include "sdl/SDL_thread.h"
};

#define MAX_AUDIO_FRAME_SIZE 192000   // 1 second of 48khz 32bit audio


class CMyDecodevideo
{
//��Ƶ
public:
	AVFormatContext	*pFormatCtx;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	const char      *filepath;
	AVFrame	        *pFrame;
	AVFrame         *pFrameYUV;
	uint8_t         *out_buffer;
	AVPacket        *packet;
//SDL ���
public:
	//�ܵ�ָ��
	SDL_Window      *gWindow;
	SDL_Texture     *sdlTexture; 
	SDL_Renderer    *sdlRenderer;
	//С����
	SDL_Window      *gWindowsmall;
	SDL_Renderer    *sdlRenderersmall;
	//ȫ��
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
