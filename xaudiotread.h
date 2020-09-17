#ifndef XAUDIOTREAD_H
#define XAUDIOTREAD_H

#include <QThread>
#include <mutex>
#include <list>
#include <xdecodethread.h>
struct AVCodecParameters;
class XDecode;
class XAudioPlay;
class XResample;
struct AVPacket;
class XAudioTread :public XDecodeThread
{
public:
    long long pts = 0;
    virtual bool Open(AVCodecParameters *para, int sampleRate, int channels);
//    virtual void Push(AVPacket *pkt);
    virtual void Close();
    virtual void Clear() override;
    void run() override;
    void SetPause(bool isPause);
    bool isPause = false;

    XAudioTread();
    ~XAudioTread();
protected:

    std::list<AVPacket *> packs;
    std::mutex amux;
    XDecode *decode = 0;
    XAudioPlay *ap = 0;
    XResample *res = 0;
};

#endif // XAUDIOTREAD_H
