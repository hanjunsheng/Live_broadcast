#include "stdafx.h"
#include "Myffmpeg.h"
#include "Archor.h"
#include "ArchorDlg.h"
#include "AbsIMediator.h"

CMyffmpeg::CMyffmpeg(void)
{
	//³õÊ¼»¯
	m_collectdesktop = NULL;
	m_collectaudio = NULL;
	m_collectcamera = NULL;

	cameradevicename = _T("video=");
}

CMyffmpeg::~CMyffmpeg(void)
{
}

void CMyffmpeg::Factory(bool desk,bool camera,bool audio,bool microphone)
{
	if(desk == true)
	{
		this->m_collectdesktop = new CCollectDesk;
		m_collectdesktop->Initial();
	}
	if(camera == true)
	{
		this->m_collectcamera = new CCollectCamera(cameradevicename);
		m_collectcamera->Start();
	}
	if(audio == true)
	{
		
	}
	if(microphone == true)
	{
		
	}
}

void CMyffmpeg::Delete()
{
	if(m_collectdesktop)
	{
		m_collectdesktop->Destroy();
		delete m_collectdesktop;
		m_collectdesktop = NULL;
	}
	if(m_collectaudio)
	{
		m_collectaudio->Destroy();
		delete m_collectaudio;
		m_collectaudio = NULL;
	}
	if(m_collectcamera)
	{
		m_collectcamera->Stop();
		delete m_collectcamera;
		m_collectcamera = NULL;
	}
}

void CMyffmpeg::SetStart(bool desk,bool camera,bool audio,bool microphone)
{
	if(desk == true)
	{
		this->m_collectdesktop->Run();
	}
	if(camera == true)
	{
		this->m_collectdesktop->Run();
	}
	if(audio == true)
	{
		this->m_collectaudio->Run();
	}
	if(microphone == true)
	{
		this->m_collectaudio->Run();
	}
}

void CMyffmpeg::SetStop(bool desk,bool camera,bool audio,bool microphone)
{
	if(desk == true)
	{
		this->m_collectdesktop->Close();
	}
	if(camera == true)
	{
		this->m_collectdesktop->Close();
	}
	if(audio == true)
	{
		this->m_collectaudio->Close();
	}
	if(microphone == true)
	{
		this->m_collectaudio->Close();
	}
}

void CMyffmpeg::SetCameraDeviceName(CString name)
{
	this->cameradevicename += name;
}