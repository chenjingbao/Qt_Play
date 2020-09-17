#ifndef XDEMUXTHREAD_H
#define XDEMUXTHREAD_H
#include <QThread>
#include "ivideocall.h"
class XDemux;
class XVideoThread;
class XAudioTread;
class XDemuxThread : public QThread
{
public:
    virtual bool Open(const char *url, IVideoCall *call);
    virtual bool Start();
    virtual void Close();
    virtual void Clear();

    virtual void Seek(double pos);
    void run() override;

    XDemuxThread();
    ~XDemuxThread();
    bool isExit = false;
    long long pts = 0;
    long long totalMs = 0;

    void SetPause(bool isPause);
    bool isPause = false;
protected:
    std::mutex mux;
    XDemux *demux =0;
    XVideoThread *vt = 0;
    XAudioTread *at = 0;
};

#endif // XDEMUXTHREAD_H
