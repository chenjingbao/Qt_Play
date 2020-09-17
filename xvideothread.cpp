#include "xvideothread.h"
#include <iostream>
#include "xdecode.h"
using namespace std;

extern "C"
{
    #include<libavcodec/avcodec.h>
}


XVideoThread::XVideoThread()
{

}

bool XVideoThread::Open(AVCodecParameters *para, IVideoCall *call, int width, int height)
{
    synpts = 0;
    if (!para) return false;
    Clear();

    vmux.lock();
    this->call = call;
    if (call)
    {
        call->Init(width, height);
    }

    vmux.unlock();
    int re = true;
    if (!decode->Open(para))
    {
        cout <<"audio XDecode open failed!"<<endl;
        re = false;
    }

    cout <<"XAudio::Open: " << re << endl;
    return true;


}

void XVideoThread::run()
{

    while (!isExit)
    {
        vmux.lock();
        if (this->isPause)
        {
            vmux.unlock();
            msleep(5);
            continue;
        }
        if (synpts >0 && synpts < decode->pts)
        {
            vmux.unlock();
            msleep(1);
            continue;
        }
        AVPacket *pkt = Pop();
//        if (packs.empty() || !decode)
//        {
//            vmux.unlock();
//            msleep(1);
//            continue;
//        }

        if (synpts < decode ->pts)
        {
            vmux.unlock();
            msleep(1);
            continue;
        }

//        AVPacket *pkt = packs.front();
//        packs.pop_front();
        bool re = decode->Send(pkt);
        if(!re)
        {
            vmux.unlock();
            msleep(1);
            continue;
        }

        while (!isExit)
        {
            AVFrame *frame = decode->Recv();
            if (!frame) break;
            if (call)
            {
                call->Repain(frame);
            }
        }

        vmux.unlock();
    }

}

void XVideoThread::SetPause(bool isPause)
{
    vmux.lock();
    this->isPause = isPause;
    vmux.unlock();
}
void XVideoThread::~XVideoThread()
{
    isExit = true;
    wait();
}
