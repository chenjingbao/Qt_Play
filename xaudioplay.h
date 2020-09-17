#ifndef XAUDIOPLAY_H
#define XAUDIOPLAY_H


class XAudioPlay
{
public:
    int sampleRate = 44100;
    int sampleSize = 16;
    int channels = 2;

    virtual bool Open() = 0;
    virtual bool Close() = 0;
    virtual bool Clear() = 0;
    virtual long long GetNoPlayMs();
    virtual bool Write(const unsigned char *data, int datasize) = 0;
    virtual int GetFree() = 0;
    virtual void SetPause(bool isPause) = 0;
    static XAudioPlay *Get();
    XAudioPlay();
};

#endif // XAUDIOPLAY_H
