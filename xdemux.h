#ifndef XDEMUX_H
#define XDEMUX_H
#include <mutex>
struct AVFormatContext;
struct AVPacket;
struct AVCodecParameters;

class XDemux
{
public:
    virtual bool Open(const char *url);

    //av_packet_free
    virtual AVPacket *Read();

    virtual bool isAudio(AVPacket *pkt);


    //AVCodec_Parameters_free
    virtual AVCodecParameters *CopyVPara();
    virtual AVCodecParameters *CopyAPara();

    //pos 0.0~1.0
    virtual bool Seek(double pos);

    virtual void Clear();
    virtual void Close();

    XDemux();

    int totalMs = 0;
    int width = 0;
    int height = 0;

    int sampleRate = 0;
    int channels = 0;
    int videoStream = 0;
    int audioStream = 1;
protected:
    std::mutex mux;
    AVFormatContext *ic = nullptr;

};

#endif // XDEMUX_H
