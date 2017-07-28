#pragma once

#ifndef MYFFMPEG_H_
#define MYFFMPEG_H_


#include "CollectDesk.h"
#include "CollectAudio.h"
#include "CollectCamera.h"

class CMyffmpeg
{
public:
	CMyffmpeg(void);
	~CMyffmpeg(void);
//²É¼¯×ÀÃæ
public:
	CCollectDesk * m_collectdesktop;
	CCollectAudio * m_collectaudio;
	CCollectCamera * m_collectcamera;

public:
	void Factory(bool desk,bool camera,bool audio,bool microphone);
	void Delete();
	void SetStart(bool desk,bool camera,bool audio,bool microphone);
	void SetStop(bool desk,bool camera,bool audio,bool microphone);
public:
	CString cameradevicename;
	void SetCameraDeviceName(CString name);
};

#endif