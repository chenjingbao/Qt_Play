#include "widget.h"
#include <iostream>
#include <QApplication>
#include <QThread>
#include "xdemux.h"
#include "xdecode.h"
#include <QDebug>
#include "ui_widget.h"
#include "xresample.h"
#include "xaudioplay.h"
#include "xaudiotread.h"
#include "xvideothread.h"
#include "xdemuxthread.h"
using namespace std;

class TestThread : public QThread
{
public:
    XAudioTread at;
    XVideoThread vt;
    void Init()
    {
        //char *url = "rtmp://58.200.131.2:1935/livetv/hunantv";
        char *url = "/home/vista/QtProjects/XPlay2/1.mp4";
       // char *url = "/home/vista/QtProjects/XPlay2/out240x128.yuv";

//        cout << "demux.open = "<< demux.Open(url)<<endl;
//        demux.Read();
//        demux.Clear();
//        demux.Close();

        cout << "demux.open = "<< demux.Open(url)<<endl;
        cout << "demux.CopyVPara = "<< demux.CopyVPara()<<endl;
        cout << "demux.CopyAPara = "<< demux.CopyAPara()<<endl;
       // cout << "seek = "<< demux.Seek(0.98)<<endl;

//        XDecode vdecode;
//        cout << "vdecode.open() = " << vdecode.Open(demux.CopyVPara())<<endl;
        //vdecode.Clear();
        //vdecode.Close();
        cout<<"==========text============="<<endl;

//        if(vdecode.codec)
//        {
//            cout<<"yyyyyeeeeesssss"<<endl;
//        }
//        else{
//            cout<<"nnnnnnnnnoooooo"<<endl;
//        }
        cout << "at.Open = " << at.Open(demux.CopyAPara(), demux.sampleRate, demux.channels)
        at.start();
        vt.Open(demux.CopyVPara(), video, demux.width, demux.height);
        vt.start();
    }

    void run()
    {
  /*      XDecode vdecode;
        cout << "vdecode.open() = " << vdecode.Open(demux.CopyVPara())<<endl;
        XDecode adecode;
        cout << "adecode.open() = " << adecode.Open(demux.CopyAPara())<<endl;
        cou*/t<<"resample.Open() = " <<resample.Open(demux.CopyAPara())<<endl;

        //        if(vdecode.codec)
//        {
//            cout<<"yyyyyeeeeesssss"<<endl;
//        }
//        else{
//            cout<<"nnnnnnnnnoooooo"<<endl;
//        }
        unsigned char *pcm = new unsigned char[1024*1024];
        for (;;)
        {
            AVPacket *pkt = demux.Read();
            if(demux.isAudio(pkt))
            {
                at.Push(pkt);

//                adecode.Send(pkt);
//                AVFrame *frame = adecode.Recv();
//                int len = resample.Resample(frame, pcm);
//                cout<<"Resample:" <<len <<" ";
//                while (len >0)
//                {
//                    if (XAudioPlay::Get()->GetFree()>= len){
//                        XAudioPlay::Get()->Write(pcm, len);
//                    }
//                }

//                cout<<"BBBBBBBBBBBBBBB";
//                cout<<"Resample: "<<resample.Resample(frame, pcm)<<" ";
//                //cout <<"Audio: "<<frame<<endl;
//                XAudioPlay::Get()->channels = demux.channels;
//                XAudioPlay::Get()->sampleRate = demux.sampleRate;

//                cout<<"XAudioPlay::Get()->Open() = " << XAudioPlay::Get()->Open()<<endl;
//                cout<<"AAAAAAAAAAAAAAAAAAA";
            }
            else
            {
                vt.Push(pkt);
//                vdecode.Send(pkt);
//                AVFrame *frame = vdecode.Recv();
//                video->Repaint(frame);
//                //cout <<"Video: "<<frame<<endl;
            }
            if(!pkt)
            {
                cout <<"!pkt"<<endl;
                break;

            }
        }
    }
    XDemux demux;
    XDecode vdecode;
    XDecode adecode;
    XResample resample;
    XVideoWidget *video = 0;
};

int main(int argc, char *argv[])
{
//    XDemux demux;
//    //char *url = "rtmp://202.69.69.180:443/webcast/bshdlive-pc";
//    char *url = "/home/vista/QtProjects/XPlay2/1.mp4";

//    cout << "demux.open = "<< demux.Open(url)<<endl;
//    demux.Read();
//    demux.Clear();
//    demux.Close();

//    cout << "demux.open = "<< demux.Open(url)<<endl;
//    cout << "demux.CopyVPara = "<< demux.CopyVPara()<<endl;
//    cout << "demux.CopyAPara = "<< demux.CopyAPara()<<endl;
//    cout << "seek = "<< demux.Seek(0.98)<<endl;

//    XDecode vdecode;
//    cout << "vdecode.open() = " << vdecode.Open(demux.CopyVPara())<<endl;
//    //vdecode.Clear();
//    //vdecode.Close();
//    cout<<"==========text============="<<endl;
//    XDecode adecode;
//    cout << "adecode.open() = " << adecode.Open(demux.CopyAPara())<<endl;

///*
//    for (;;)
//    {
//        AVPacket *pkt = demux.Read();
//        if(demux.isAudio(pkt))
//        {
//            adecode.Send(pkt);
//            AVFrame *frame = adecode.Recv();
//            cout <<"Audio: "<<frame<<endl;
//        }
//        else
//        {
//            vdecode.Send(pkt);
//            AVFrame *frame = vdecode.Recv();
//            cout <<"Video: "<<frame<<endl;
//        }
//        if(!pkt) break;
//    }
//*/

    //TestThread tt;


    QApplication a(argc, argv);
    Widget w;
    w.show();

    w.ui->video->Init(tt.demux.width, tt.demux.height);
    //tt.video = w.ui->video;
    //tt.Init();
    //cout<<tt.video->width<<"++++++++++++++"<<tt.video->height<<endl;
    //tt.start();
    //w.ui.video->Init(demux.width, demux.height);
    XDemuxThread dt;
    dt.Open("v1080.mp4", w.ui->video);
    dt.start();
    return a.exec();
}
