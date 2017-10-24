#ifndef DSHOWVIDEO_H
#define DSHOWVIDEO_H

#include "compointer.h"

#include <dshow.h>

class DShowVideo
{
    public:

        DShowVideo();

        int initialize(LPCWSTR filename, void * hWnd);  // set all
        void setSource(bool fromCamera);
        int startVideo();                               // start
        int pauseVideo();                               // stop locally
        void deinitialize();                            // flush all

        const char * getErrorMessage(int code) const;

    private:

        /* DShow objects wrapped with ComPointer guard */

        ComPointer<IGraphBuilder>           pGraphBuilder;          // base interface for filter graph
        ComPointer<ICaptureGraphBuilder2>   pCaptureGraphBuilder2;  // interface for graph builder
        ComPointer<IMediaControl>           pMediaControl;          // Interface for data stream management
        ComPointer<IVideoWindow>            pVideoWindow;           // Interface for video output window
        ComPointer<IBaseFilter>             pSourceFile;            // Interface for video file

        ComPointer<ICreateDevEnum>          devs;                   // Capture devices
        ComPointer<IEnumMoniker>            cams;                   // Cameras
        ComPointer<IMoniker>                mon;                    // Binder
        ComPointer<IBaseFilter>             cam;                    // Found camera

        /* Additional data */

        long                                NativeVideoWidth = 0,
                                            NativeVideoHeight = 0;  // native video resolution

        HRESULT                             hr;                     // Variable for return codes

        bool                                fromCamera;             // Whether to render from cam or not

        /* Stream state */

        enum state_t {
            READY, PLAYING, PAUSED, EMPTY
        };

        state_t STATE;

        /* Private common methods */

        int createGraph();
        int setSourceFile(LPCWSTR filename);
        int setSourceCam();
        int setInterfaces();
        int setWindow(void * hWnd);

};

#endif