#include "xdecodethread.h"
#include "xdecode.h"


AVPacket *XDecodeThread::Pop()
{
    mux.lock();

    if (packs.empty())
    {
        mux.unlock();
        return 0;
    }

    AVPacket *pkt = packs.front();
    packs.pop_front();

    mux.unlock();
    return pkt;
}
XDecodeThread::XDecodeThread()
{
    if (!decode)
    {
        decode = new XDecode();
    }
}
void XDecodeThread::Push(AVPacket *pkt)
{
    if (!pkt) return ;

    while (!isExit)
    {
        mux.lock();
        if (packs.size() < maxList)
        {
            packs.push_back(pkt);
                    mux.unlock();
            break;
        }
        mux.unlock();
        msleep(1);
    }

}

void XDecodeThread::Clear()
{
    mux.lock();
    decode->Clear();
    while(!packs.empty())
    {
        AVPacket *pkt = packs.front();
        XFreePacket( &pkt);
        packs.pop_front();
    }
    mux.unlock();
}

void XDecodeThread::Close()
{
    Clear();
    mux.lock();
    isExit = true;
    wait();
    decode->Close();
    delete decode;
    decode = 0;
    mux.unlock();
}

XDecodeThread::~XDecodeThread()
{
    isExit = true;
    wait();
}
