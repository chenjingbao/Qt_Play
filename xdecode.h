#ifndef XDECODE_H
#define XDECODE_H

struct AVCodecParameters;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;

#include<mutex>
extern void XFreePacket (AVPacket **pkt);
class XDecode
{
public:
    bool isAudio = false;

    long long pts = 0;
    //open decoder , free para
    virtual bool Open(AVCodecParameters *para);

    virtual bool Send(AVPacket *pkt);
    virtual AVFrame* Recv();

    virtual void Close();
    virtual void Clear();
    XDecode();
//protected:
    AVCodecContext *codec = 0;
    std::mutex mux;

};

#endif // XDECODE_H
