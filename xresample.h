#ifndef XRESAMPLE_H
#define XRESAMPLE_H

#include<mutex>
struct AVCodecParameters;
struct AVFrame;
struct SwrContext;
class XResample
{
public:
    virtual bool Open(AVCodecParameters *para, bool isClearPara);
    virtual void Close();

    //free
    virtual int Resample(AVFrame *indata, unsigned char *data);
    int outFormat = 1;
    XResample();
protected:
    std::mutex mux;
    SwrContext *actx = 0;
};

#endif // XRESAMPLE_H
