#pragma once
#ifndef MYENCODE_H__
#define MYENCODE_H__

#include <iostream>
using namespace std;

extern "C"
{
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"
};

class CMyEncode
{
public:
	CMyEncode(void);
	~CMyEncode(void);
public:
	AVFormatContext     *pFormatCtx;
	AVOutputFormat      *fmt;
	AVStream            *video_st;
	AVCodecContext      *pCodecCtx;
	AVCodec             *pCodec;
	uint8_t				*picture_buf;
	AVFrame             *picture;
	FILE                *in_file;
	const char          *out_file;
	const char          *inputfilepath;
public:
	int m_w;
	int m_h;
	int framenum;
	int m_malv;
public:
	void InitialEncode();
	void FindEncode();
	bool StartEncode();
	void Setmalv(int malv);
protected:
	int flush_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index);
};
#endif
