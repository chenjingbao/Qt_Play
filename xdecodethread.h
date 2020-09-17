#ifndef XDECODETHREAD_H
#define XDECODETHREAD_H

#include <QThread>
#include <mutex>
#include <list>
struct AVCodecParameters;
class XDecode;
class XAudioPlay;
class XResample;
struct AVPacket;

class XDecodeThread: public QThread
{
public:
    XDecodeThread();
    virtual ~XDecodeThread();
    virtual void Push(AVPacket *pkt);
    virtual void Clear();
    virtual void Close();


    virtual AVPacket *Pop();
    int maxList = 100;
    bool isExit = false;
    XDecode *decode = 0;
protected:

    std::list<AVPacket *> packs;
    std::mutex mux;

};

#endif // XDECODETHREAD_H
