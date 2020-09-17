#include "xdemuxthread.h"
#include "xdemux.h"
#include "xvideothread.h"
#include "xaudiotread.h"
#include <iostream>
#include "xdecode.h"
extern "C"
{
#include<libavformat/avformat.h>

}

using namespace std;

void XDemuxThread::run()
{
    while (!isExit){
        mux.lock();
        if (isPause)
        {
            msleep(5);
            continue;
        }

        if (!demux)
        {
            mux.unlock();
            msleep(5);
            continue;
        }

        if (vt && at)
        {
            vt->synpts = at->pts;
            pts = at->pts;
        }

        AVPacket *pkt = demux->Read();
        if (!pkt)
        {
            mux.unlock();
            msleep(5);
            continue;
        }
        if (demux->isAudio(pkt))
        {
            if (at) at->Push(pkt);
        }
        else{
            if (vt) vt->Push(pkt);
        }
        mux.unlock();
        msleep(1);
    }
}

bool XDemuxThread::Open(const char *url, IVideoCall *call)
{
    if (url == 0 || url[0] == '\0')
        return false;

    mux.lock();
    if (!demux)demux = new XDemux();
    if (!vt) vt = new XVideoThread();
    if (!at) at = new XAudioTread();

    bool re = demux->Open(url);
    if (!re)
    {
        cout << "demux->Open(url) failed !"<<endl;
        return false;
    }
    if(!vt->Open(demux->CopyVPara(), call, demux->width, demux->height))
    {
        re = false;
        cout <<"vt->Open failed!" <<endl;
    }

    if (!at->Open(demux->CopyAPara(), demux->sampleRate, demux->channels))
    {

        re = false;
        cout <<"at->Open failed!" <<endl;
    }
    totalMs = demux->totalMs;
    mux.unlock();
    cout <<"XDemuxThread::Open() :" << re<<endl;
    return re;
}

bool XDemuxThread::Start(){
    mux.lock();
    QThread::start();
    if (vt) vt->start();
    if (at) at->start();


}

void XDemuxThread::Close()
{
    isExit = true;
    wait();
    if(vt) vt->Close();
    if (at) at->Close();
    mux.lock();
    delete vt;
    delete at;
    vt = 0;
    at = 0;
    mux.unlock();
}

void XDemuxThread::Clear()
{
    mux.lock();
    if (demux) demux->Clear();
    if (vt) vt->Clear();
    if (at) at->Clear();
    mux.unlock();
}

void XDemuxThread::Seek(double pos)
{
    Clear();
    mux.lock();
    bool status = this->isPause;

    mux.unlock();
    SetPause(true);
    mux.lock();
    if(demux)
    {
        demux->Seek(pos);
    }
    long long seekPts = pos*demux->totalMs;
    while (!isExit)
    {
        AVPacket *pkt = demux->Read();
        if (!pkt) break;
        if (pkt->stream_index == demux-> audioStream)
        {
            av_packet_free(&pkt);
            continue;
        }
        bool re = vt->decode->Send(pkt);
        av_packet_free(&pkt);
        if(!re) break;
        AVFrame *frame = vt->decode->Recv();
        if (!frame) continue;
        if(frame->pts >= seekPts)
        {
            this->pts = frame->pts;
            vt-> call->Repain(frame);
            break;
        }
        av_frame_free(&frame);
    }

    mux.unlock();
    if (!status)
        SetPause(false);
}

XDemuxThread::XDemuxThread()
{

}

XDemuxThread::~XDemuxThread()
{
    isExit = true;
    wait();
}

void XDemuxThread::SetPause(bool isPause)
{
    mux.lock();
    this->isPause = isPause;
    if (at) at->SetPause(isPause);
    if (vt) vt->SetPause(isPause);
    mux.unlock();
}





