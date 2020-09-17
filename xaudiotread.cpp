#include "xaudiotread.h"

#include "xdecode.h"
#include "xaudioplay.h"
#include "xresample.h"
#include <iostream>
using namespace std;

extern "C"
{
    #include<libavcodec/avcodec.h>
}


XAudioTread::XAudioTread()
{
    if (!res) res = new XResample();
    if (!ap) ap = XAudioPlay::Get();
}

XAudioTread::~XAudioTread()
{
    isExit = true;
    wait();
}


bool XAudioTread::Open(AVCodecParameters *para, int sampleRate, int channels)
{
    if (!para) return false;
    amux.lock();
    pts = 0;

    bool re = true;

    if (!res->Open(para, false))
    {
        cout <<"XResample open failed" <<endl;
        re = false;
//        mux.lock();
//        return false;
    }
    ap->sampleRate = sampleRate;
    ap->channels = channels;


    if(!ap->Open())
    {
        re = false;
        cout <<"XAudioPlay open failed!"<<endl;
//        mux.lock();
//        return false;
    }

    if (!decode->Open(para))
    {
        cout <<"audio XDecode open failed!"<<endl;
        re = false;
    }
    amux.unlock();
    cout <<"XAudio::Open: " << re << endl;
    return true;
}

void XAudioTread::Close()
{
    XDecodeThread::Close();
    if (res)
    {
        res->Close();
        amux.lock();
        delete res;
        res = 0;
        amux.unlock();
    }
    if (ap)
    {
        ap->Close();
        amux.lock();
        ap = 0;
        amux.unlock();
    }
}

void XAudioTread::Clear()
{
    XDecodeThread::Clear();
    mux.lock();

    if (ap) ap->Clear();
    mux.unlock();
}

void XAudioTread::run()
{
    unsigned char *pcm = new unsigned char [1024*1024 *10];
    while (!isExit)
    {
        amux.lock();
        if (isPause)
        {
            amux.lock();
            msleep(5);
            continue;
        }
        AVPacket *pkt = Pop();
        bool re = decode->Send(pkt);
        if(!re)
        {
            amux.unlock();
            msleep(1);
            continue;
        }
        AVFrame *frame = decode->Recv();
        int size = res->Resample(frame, pcm);
        while (!isExit)
        {
            if (size<=0)break;

            if (!frame) break;

            pts = decode->pts - ap->GetNoPlayMs();
            cout << "audio pts = " << pts <<endl;

            while (!isExit)
            {
                if (ap->GetFree()< size || isPause)
                {
                    msleep(1);
                    continue;
                }
                ap->Write(pcm, size);
                break;
            }
        }

        amux.unlock();
    }
    delete pcm;
}

void XAudioTread::SetPause(bool isPause)
{
    amux.lock();

    this->isPause = isPause;
    if (ap)
        ap->SetPause(isPause);
    amux.unlock();
}





