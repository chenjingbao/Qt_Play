#ifndef XVIDEOTHREAD_H
#define XVIDEOTHREAD_H

struct AVPacket;
#include <list>
#include <mutex>
#include <QThread>
#include <ivideocall.h>
#include <xdecodethread.h>
class XDecode;
struct AVCodecParameters;
class XVideoThread : public XDecodeThread
{
public:

    virtual bool Open(AVCodecParameters *para, IVideoCall *call, int width, int height);
    virtual void Push(AVPacket *pkt);

    void run() ;
    int maxList = 100;
    bool isExit = false;
    long long synpts = 0;

    void SetPause(bool isPause);
    bool isPause = false;
    IVideoCall *call = 0;

    XVideoThread();
    ~XVideoThread();
protected:
    std::list<AVPacket *>packs;
    std::mutex vmux;
    XDecode *decode = 0;

};

#endif // XVIDEOTHREAD_H
