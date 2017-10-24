#include "DShowVideo.h"

DShowVideo::DShowVideo()
{
    STATE = EMPTY;
}

int DShowVideo::initialize(LPCWSTR filename, void * hWnd)
{
    int code = 0;
    if (STATE == EMPTY)
    {
        if (FAILED(CoInitialize(NULL)))
        {
            return 1;
        }

        code = createGraph();
        if (code)
        {
            return code;
        }

        code = (fromCamera ? setSourceCam() : setSourceFile(filename));

        if (code)
        {
            return code;
        }

        code = setInterfaces();
        if (code)
        {
            return code;
        }

        code = setWindow(hWnd);
        if (code)
        {
            return code;
        }

        STATE = READY;
    }

    return 0;
}

void DShowVideo::deinitialize()
{
    if (STATE == PAUSED)
    {
        CoUninitialize();
        STATE = EMPTY;
    }
}

int DShowVideo::startVideo()
{
    if (STATE == READY || STATE == PAUSED)
    {
        hr = pMediaControl->Run();

        if (FAILED(hr))
        {
            return 13;
        }

        STATE = PLAYING;
    }

    return 0;
}

int DShowVideo::pauseVideo()
{
    if (STATE == PLAYING)
    {
        hr = pMediaControl->Stop();

        if (FAILED(hr))
        {
            return 14;
        }

        STATE = PAUSED;
    }

    return 0;
}

const int maxErrCode = 19;
char * errMessages[] = {
	"1 Failed to CoInitialize Dshow",
	"2 Failed to create FilterGraph",
	"3 Failed to create GraphBuilder",
	"4 Failed to connect GraphBuilder to FilterGraph",
	"5 Failed to set up video-file filter",
	"6 Failed to connect GraphBuilder and SourceFile",
	"7 Failed to request interface for MediaControl",
	"8 Failed to request interface for VideoWindow",
	"9 Failed to set window for video output",
	"10 Failed to set WindowStyle",
	"11 Failed to request interface for BasicVideo",
	"12 Failed to get video resolution",
	"13 Failed to run video",
	"14 Failed to stop video",
	"15 Failed to set DevEnum",
	"16 Failed to get devices",
	"17 Failed to get camera",
	"18 Failed to bind camera",
	"19 Failed to add filter for camera"
};

const char * DShowVideo::getErrorMessage(int code) const
{
	if (code <= maxErrCode)
	{
		return errMessages[code];
	}

    return "No such code for error";
}

int DShowVideo::createGraph()
{
    // Graph creation and retrieving interface

    pGraphBuilder = ComPointer<IGraphBuilder>(CLSID_FilterGraph, IID_IGraphBuilder);

    if (pGraphBuilder == NULL)
    {
        return 2;
    }

    // GraphBuilder creation

    pCaptureGraphBuilder2 = ComPointer<ICaptureGraphBuilder2>
                            (CLSID_CaptureGraphBuilder2, IID_ICaptureGraphBuilder2);

    if (pCaptureGraphBuilder2 == NULL)
    {
        return 3;
    }

    // Connect GraphBuilder and FilterGraph

    hr = pCaptureGraphBuilder2->SetFiltergraph(pGraphBuilder);

    if (FAILED(hr))
    {
        return 4;
    }

    return 0;
}

int DShowVideo::setSourceCam()
{
    devs = ComPointer<ICreateDevEnum>
           (CLSID_SystemDeviceEnum, IID_ICreateDevEnum);

    if (devs == NULL)
    {
        return 15;
    }

    hr = devs->CreateClassEnumerator(
             CLSID_VideoInputDeviceCategory, cams.pparam(), 0);

    if (FAILED(hr) || cams == NULL)
    {
        return 16;
    }

    hr = cams->Next(1, mon.pparam(), 0);

    if (FAILED(hr) || mon == NULL)
    {
        return 17;
    }

    hr = mon->BindToObject(0, 0, IID_IBaseFilter, (void **)&cam);

    if (FAILED(hr) || cam == NULL)
    {
        return 18;
    }

    hr = pGraphBuilder->AddFilter(cam, L"Capture Source");

    if (FAILED(hr))
    {
        return 19;
    }

    // Connect filters. Decompression filter will be added automatically

    hr = pCaptureGraphBuilder2->RenderStream(NULL, NULL,
            cam, NULL, NULL);

    if (FAILED(hr))
    {
        return 6;
    }

	return 0;
}

int DShowVideo::setSourceFile(LPCWSTR filename)
{
    // Add video-file filter

    hr = pGraphBuilder->AddSourceFilter(filename, L"",
                                        (IBaseFilter **)& pSourceFile);

    if (FAILED(hr))
    {
        return 5;
    }

    hr = pCaptureGraphBuilder2->RenderStream(NULL, NULL,
            pSourceFile,
            NULL, NULL);

    //hr = pGraphBuilder->RenderFile(filename, NULL); // without pCaptureGraphBuilder2

    if (FAILED(hr))
    {
        return 6;
    }

    return 0;
}

int DShowVideo::setInterfaces()
{
    // Request interface for MediaControl

    pMediaControl = ComPointer<IMediaControl>
                    (pGraphBuilder, IID_IMediaControl);

    if (pMediaControl == NULL)
    {
        return 7;
    }

    // Request interface for VideoWindow

    pVideoWindow = ComPointer<IVideoWindow>
                   (pGraphBuilder, IID_IVideoWindow);

    if (pVideoWindow == NULL)
    {
        return 8;
    }


    return 0;
}

int DShowVideo::setWindow(void * hWnd)
{
    // Set up window for video output

	if (hWnd == NULL)
	{
		// Video will be translated in separate window
		return 0;
	}

    hr = pVideoWindow->put_Owner((OAHWND)hWnd);

    if (FAILED(hr))
    {
        return 9;
    }

    hr = pVideoWindow->put_WindowStyle(WS_CHILD);

    pVideoWindow->SetWindowPosition(0, 0, 400, 400);

    if (FAILED(hr))
    {
        return 10;
    }

    return 0;
}

void DShowVideo::setSource(bool fromCamera)
{
    this->fromCamera = fromCamera;
}