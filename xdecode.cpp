#include "xdecode.h"
extern "C"
{
    #include<libavcodec/avcodec.h>
}
#include <iostream>
using namespace std;


void XDecode::Close()
{
    mux.lock();
    if(codec)
    {
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    pts = 0;
    mux.unlock();
}

void XDecode::Clear()
{
    mux.lock();
    if(codec)
        avcodec_flush_buffers(codec);

    mux.unlock();
}

bool XDecode::Open(AVCodecParameters *para)
{

    if (!para) return false;
    Close();
    /////////////////
    /// \open vcodec
    ///find decode
    AVCodec *vcodec = avcodec_find_decoder(para->codec_id);
    if (!vcodec)
    {
        avcodec_parameters_free(&para);
        cout << "can't find the codec id "<<para->codec_id<<endl;
        return false;
    }
    cout << "find the AVCode " <<para->codec_id<<endl;
    ////////////////
    /// video decode open

    mux.lock();
    codec = avcodec_alloc_context3(vcodec);

    avcodec_parameters_to_context(codec, para);
    avcodec_parameters_free(&para);
    codec->thread_count = 8;

    int re = avcodec_open2(codec, 0, 0);
    if (re != 0)
    {
        avcodec_free_context(&codec);
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf)-1);
        cout<<"avcodec_open2 failed! :" <<buf<<endl;

        return false;
    }

    mux.unlock();
    cout<<"video avcodec_open2 sucess!"<<endl;
//    if(codec)
//    {
//        cout<<"test4"<<endl;

//    }

    return true;
}

bool XDecode::Send(AVPacket *pkt)
{
    //cout<<"test1"<<endl;
    if(!pkt || pkt->size<=0 || !pkt->data) return false;
    //cout<<"test2"<<endl;
    mux.lock();
    if(!codec)
    {
        //cout<<"test3"<<endl;
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec, pkt);
    mux.unlock();
    av_packet_free(&pkt);
    //cout<<"test4"<<endl;
    if (re != 0) return false;
    return true;
}

AVFrame *XDecode::Recv()
{
    mux.lock();
    if(!codec)
    {
        mux.unlock();
        return 0;
    }
    AVFrame *frame = av_frame_alloc();
    int re = avcodec_receive_frame(codec, frame);
    mux.unlock();
    if(re != 0)
    {
        av_frame_free(&frame);
        return 0;
    }

    cout << "[" <<frame->linesize[0]<<"]"<<" "<<flush;
    pts = frame->pts;
    return frame;
}

XDecode::XDecode()
{

}

void XFreePacket(AVPacket **pkt)
{
    if (pkt || (*pkt)) return;
    av_packet_free(pkt);
}

