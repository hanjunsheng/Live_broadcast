#include "stdafx.h"
#include "CollectAudio.h"
#include "Archor.h"
#include "ArchorDlg.h"

CCollectAudio::CCollectAudio(void)
{
	m_pMMDevice = NULL;
	m_hStartedEvent = NULL;
	m_hStopEvent = NULL;
	m_bflagchangefile = false;
	m_hthreadaudioendoce = NULL;
	m_eventaudioencode = NULL;
	m_threadsound = NULL;
	m_bflagfinishaudio = false;
	file = NULL;
}

CCollectAudio::~CCollectAudio(void)
{
}

IMMDevice* CCollectAudio::GetDefaultDevice()
{
	IMMDevice* pDevice = NULL;
	IMMDeviceEnumerator *pMMDeviceEnumerator = NULL;
	HRESULT hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, 
        __uuidof(IMMDeviceEnumerator),
        (void**)&pMMDeviceEnumerator);
	if(FAILED(hr)) return NULL;

    hr = pMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    pMMDeviceEnumerator->Release();
	
	return pDevice;
}

void CCollectAudio::Initial()
{
	CoInitialize(NULL);
	this->m_pMMDevice = this->GetDefaultDevice();
	this->m_hStartedEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
    this->m_hStopEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	this->myaudioencode.Initial();
}

DWORD WINAPI CCollectAudio::ThreadSound(LPVOID lpParameter)
{
    CoUninitialize();
	AudioStruct *pArgs = (AudioStruct *)lpParameter;
	CoInitialize(NULL);
	
	((CABsIMediator *)((CArchorDlg *)theApp.m_pMainWnd)->GetIMediator())->m_myffmpeg->m_collectaudio->CollectAudio(pArgs->pMMDevice,pArgs->bInt16,pArgs->hStartedEvent,pArgs->hStopEvent,&pArgs->pnFrames);

    CoUninitialize();

	return 0;
}

HRESULT CCollectAudio::CollectAudio(IMMDevice *pMMDevice,bool bInt16,HANDLE hStartedEvent,HANDLE hStopEvent,PUINT32 pnFrames)
{
	//接受返回值
	HRESULT hr;
	//activate an IAudioClient
    IAudioClient *pAudioClient = NULL;
    hr = pMMDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
    if (FAILED(hr)) 
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IMMDevice::Activate(IAudioClient) failed"));
        return hr;
    }

	// get the default device periodicity
    REFERENCE_TIME hnsDefaultDevicePeriod;
    hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod,NULL);
    if (FAILED(hr)) 
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IAudioClient::GetDevicePeriod failed"));
        pAudioClient->Release();
        return hr;
    }

	 // get the default device format
    WAVEFORMATEX *pwfx;
    hr = pAudioClient->GetMixFormat(&pwfx);
    if (FAILED(hr))
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IAudioClient::GetMixFormat failed"));
        CoTaskMemFree(pwfx);
        pAudioClient->Release();
        return hr;
    }

	if (bInt16)
	{
        // coerce int-16 wave format
        // can do this in-place since we're not changing the size of the format
        // also, the engine will auto-convert from float to int for us
        switch (pwfx->wFormatTag)
		{
            case WAVE_FORMAT_IEEE_FLOAT:
                pwfx->wFormatTag = WAVE_FORMAT_PCM;
                pwfx->wBitsPerSample = 16;
                pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
                pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;
                break;
            case WAVE_FORMAT_EXTENSIBLE:
                {
                    // naked scope for case-local variable
                    PWAVEFORMATEXTENSIBLE pEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pwfx);
                    if (IsEqualGUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, pEx->SubFormat)) 
					{
                        pEx->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
                        pEx->Samples.wValidBitsPerSample = 16;
                        pwfx->wBitsPerSample = 16;
                        pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
                        pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;
                    }
					else
					{
						(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("Don't know how to coerce mix format to int-16"));
                        CoTaskMemFree(pwfx);
                        pAudioClient->Release();
                        return E_UNEXPECTED;
                    }
                }
                break;
            default:
				(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("Don't know how to coerce WAVEFORMATEX with wFormatTag"));
                CoTaskMemFree(pwfx);
                pAudioClient->Release();
                return E_UNEXPECTED;
        }
    }

	// WriteWaveHeader
 //   MMCKINFO ckRIFF = {0};
 //   MMCKINFO ckData = {0};
 //   hr = WriteWaveHeader(hFile, pwfx, &ckRIFF, &ckData);
 //   if (FAILED(hr)) 
	//{
 //       // WriteWaveHeader does its own logging
 //       CoTaskMemFree(pwfx);
 //       pAudioClient->Release();
 //       return hr;
 //   }

	// create a periodic waitable timer
    HANDLE hWakeUp = CreateWaitableTimer(NULL, FALSE, NULL);
    if (NULL == hWakeUp)
	{
        DWORD dwErr = GetLastError();
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("CreateWaitableTimer failed: last error"));
        CoTaskMemFree(pwfx);
        pAudioClient->Release();
        return HRESULT_FROM_WIN32(dwErr);
    }

	UINT32 nBlockAlign = pwfx->nBlockAlign;
    *pnFrames = 0;
    
    hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,AUDCLNT_STREAMFLAGS_LOOPBACK,0,0,pwfx,0);
    if (FAILED(hr)) 
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IAudioClient::Initialize failed"));
        CloseHandle(hWakeUp);
        pAudioClient->Release();
        return hr;
    }
    CoTaskMemFree(pwfx);

    // activate an IAudioCaptureClient
    IAudioCaptureClient *pAudioCaptureClient;
    hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient),(void**)&pAudioCaptureClient);
    if (FAILED(hr)) 
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IAudioClient::GetService(IAudioCaptureClient) failed"));
        CloseHandle(hWakeUp);
        pAudioClient->Release();
        return hr;
    }

	// register with MMCSS
    DWORD nTaskIndex = 0;
    HANDLE hTask = AvSetMmThreadCharacteristics(L"Capture",&nTaskIndex);
    if (NULL == hTask) 
	{
        DWORD dwErr = GetLastError();
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("AvSetMmThreadCharacteristics failed"));
        pAudioCaptureClient->Release();
        CloseHandle(hWakeUp);
        pAudioClient->Release();
        return HRESULT_FROM_WIN32(dwErr);
    }    

    // set the waitable timer
    LARGE_INTEGER liFirstFire;
    liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // negative means relative time
    LONG lTimeBetweenFires = (LONG)hnsDefaultDevicePeriod / 2 / (10 * 1000); // convert to milliseconds
    BOOL bOK = SetWaitableTimer(hWakeUp,&liFirstFire,lTimeBetweenFires,NULL,NULL,FALSE);
    if (!bOK)
	{
        DWORD dwErr = GetLastError();
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("SetWaitableTimer failed"));
        AvRevertMmThreadCharacteristics(hTask);
        pAudioCaptureClient->Release();
        CloseHandle(hWakeUp);
        pAudioClient->Release();
        return HRESULT_FROM_WIN32(dwErr);
    }

	// call IAudioClient::Start
    hr = pAudioClient->Start();
    if (FAILED(hr))
	{
		(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IAudioClient::Start failed"));
        AvRevertMmThreadCharacteristics(hTask);
        pAudioCaptureClient->Release();
        CloseHandle(hWakeUp);
        pAudioClient->Release();
        return hr;
    }
    SetEvent(hStartedEvent);

	// loopback capture loop
    HANDLE waitArray[2] = {hStopEvent,hWakeUp};
    DWORD dwWaitResult;

    bool bDone = false;
	//FILE *filepcm;

	UINT32 nNextPacketSize;
	BYTE *pData;
    UINT32 nNumFramesToRead;
    DWORD dwFlags;

	//首先打开222
	this->m_bflagchangefile = false;
	file = mmioOpen(_T("../TempFile/222.pcm"),NULL,MMIO_CREATE|MMIO_WRITE);
	
	for (UINT32 nPasses = 0; !bDone; nPasses++) 
	{
		dwWaitResult = WaitForMultipleObjects(sizeof(waitArray)/sizeof(waitArray[0]),waitArray,FALSE,INFINITE);
		if (WAIT_OBJECT_0 == dwWaitResult)
		{
			bDone = true;
			continue; // exits loop
		}

		if (WAIT_OBJECT_0 + 1 != dwWaitResult) 
		{
			(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("Unexpected WaitForMultipleObjects return value"));
			pAudioClient->Stop();
			CancelWaitableTimer(hWakeUp);
			AvRevertMmThreadCharacteristics(hTask);
			pAudioCaptureClient->Release();
			CloseHandle(hWakeUp);
			pAudioClient->Release();
			return E_UNEXPECTED;
		}

		// got a "wake up" event - see if there's data

		if(nPasses == 200)
		{
			mmioClose(file,0);
			file = NULL;
			nPasses = 0;

			if(this->m_bflagchangefile == false)
			{
				file = mmioOpen(_T("../TempFile/333.pcm"),NULL,MMIO_CREATE|MMIO_WRITE);
			}
			else
			{
				file = mmioOpen(_T("../TempFile/222.pcm"),NULL,MMIO_CREATE|MMIO_WRITE);
			}

			this->m_bflagchangefile = !this->m_bflagchangefile;

			//完成pcm编码
			SetEvent(this->m_eventaudioencode);
		}
        
		hr = pAudioCaptureClient->GetNextPacketSize(&nNextPacketSize);
		if (FAILED(hr)) 
		{
			(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IAudioCaptureClient::GetNextPacketSize failed"));
			pAudioClient->Stop();
			CancelWaitableTimer(hWakeUp);
			AvRevertMmThreadCharacteristics(hTask);
			pAudioCaptureClient->Release();
			CloseHandle(hWakeUp);
			pAudioClient->Release();            
			return hr;
		}

		if (0 == nNextPacketSize)
		{
			// no data yet
			continue;
		}

		// get the captured data

		hr = pAudioCaptureClient->GetBuffer(&pData,&nNumFramesToRead,&dwFlags,NULL,NULL);
		if (FAILED(hr)) 
		{
			(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IAudioCaptureClient::GetBuffer failed"));
			pAudioClient->Stop();
			CancelWaitableTimer(hWakeUp);
			AvRevertMmThreadCharacteristics(hTask);
			pAudioCaptureClient->Release();
			CloseHandle(hWakeUp);
			pAudioClient->Release();            
			return hr;            
		}

		if (0 == nNumFramesToRead) 
		{
			(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IAudioCaptureClient::GetBuffer said to read 0 frames on pass"));
			pAudioClient->Stop();
			CancelWaitableTimer(hWakeUp);
			AvRevertMmThreadCharacteristics(hTask);
			pAudioCaptureClient->Release();
			CloseHandle(hWakeUp);
			pAudioClient->Release();            
			return E_UNEXPECTED;            
		}

		mmioWrite(file,reinterpret_cast<char _huge *>(pData),nNumFramesToRead * nBlockAlign);
        
		hr = pAudioCaptureClient->ReleaseBuffer(nNumFramesToRead);
		if (FAILED(hr))
		{
			(CArchorDlg *)theApp.m_pMainWnd->MessageBox(_T("IAudioCaptureClient::ReleaseBuffer failed"));
			pAudioClient->Stop();
			CancelWaitableTimer(hWakeUp);
			AvRevertMmThreadCharacteristics(hTask);
			pAudioCaptureClient->Release();
			CloseHandle(hWakeUp);
			pAudioClient->Release();            
			return hr;            
		}
       
	} // capture loop

	if(file != NULL)
	{
		mmioClose(file,0);
		file = NULL;
		this->m_bflagchangefile = !this->m_bflagchangefile;
		SetEvent(this->m_eventaudioencode);
	}

	//endding
    pAudioClient->Stop();
    CancelWaitableTimer(hWakeUp);
    AvRevertMmThreadCharacteristics(hTask);
    pAudioCaptureClient->Release();
    CloseHandle(hWakeUp);
    pAudioClient->Release();

    return hr;
}

void CCollectAudio::Run()
{
	this->m_eventaudioencode = CreateEvent(NULL,FALSE,FALSE,NULL);
	this->m_hthreadaudioendoce = CreateThread(NULL,0,&Threadaudioencode,this,0,NULL);

	AudioStruct as;
	as.pMMDevice = this->m_pMMDevice;
	as.bInt16 = true;
	as.hStartedEvent = this->m_hStartedEvent;
	as.hStopEvent = this->m_hStopEvent;
	as.pnFrames = 0;

	m_threadsound = CreateThread(NULL,0,&ThreadSound,&as,0,NULL);

}

DWORD WINAPI CCollectAudio::Threadaudioencode(LPVOID lpParameter)
{
	CCollectAudio * pthis = (CCollectAudio *)lpParameter;

	while(1)
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(pthis->m_eventaudioencode,INFINITE))
		{
			//进行AAC音频编码 0表示编码成功
		
			if(pthis->myaudioencode.AudioEncode() == 0)
			{
				pthis->m_bflagfinishaudio = true;
			}
		}
	}
	return 0;
}

void CCollectAudio::Close()
{
	SetEvent(m_hStopEvent);

	if(WAIT_TIMEOUT == WaitForSingleObject(this->m_threadsound,100))
	{
		TerminateThread(this->m_threadsound,-1);
	}

	if(this->m_threadsound)
	{
		CloseHandle(this->m_threadsound);
		this->m_threadsound = NULL;
	}

	if(file != NULL)
	{
		mmioClose(file,0);
		file = NULL;
		this->m_bflagchangefile = !this->m_bflagchangefile;
		SetEvent(this->m_eventaudioencode);
	}

	if(WAIT_TIMEOUT == WaitForSingleObject(this->m_hthreadaudioendoce,500))
	{
		TerminateThread(this->m_hthreadaudioendoce,-1);
	}

	if(this->m_hthreadaudioendoce)
	{
		CloseHandle(this->m_hthreadaudioendoce);
		this->m_hthreadaudioendoce = NULL;
	}

	if(m_eventaudioencode)
	{
		CloseHandle(m_eventaudioencode);
		m_eventaudioencode = NULL;
	}
}

void CCollectAudio::Destroy()
{
	if(m_hStartedEvent)
	{
		CloseHandle(m_hStartedEvent);
		m_hStartedEvent = NULL;
	}

	if(m_hStopEvent)
	{
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}
}