#include "xresample.h"
extern "C"{
    #include <libswresample/swresample.h>
    #include<libavcodec/avcodec.h>

}
#include<iostream>
using namespace std;
#pragma comment(lib, "swresample.lib")
XResample::XResample()
{

}

void XResample::Close()
{
    mux.lock();
    if (actx)
        swr_free(&actx);

    mux.unlock();
}

bool XResample::Open(AVCodecParameters *para, bool isClearPara = false)
{
    if (!para) return false;
    mux.lock();
    SwrContext *actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
            av_get_default_channel_layout(2),	//输出格式
            (AVSampleFormat)outFormat,					//输出样本格式
            para->sample_rate,					//输出采样率
            av_get_default_channel_layout(para->channels),//输入格式
            (AVSampleFormat)para->format,
            para->sample_rate,
            0,0
        );
    avcodec_parameters_free(&para);
    if (isClearPara)
        avcodec_parameters_free(&para);
    int re = swr_init(actx);
    mux.unlock();
    if (re != 0)
    {

        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        cout << "swr_init  failed! :" << buf << endl;
        getchar();
        return false;
    }
    //unsigned char *pcm = NULL;
    return true;
}

int XResample::Resample(AVFrame *indata, unsigned char *d)
{
    cout<<"CCCCCCCCCCCCCCCCCCCC";
    if (!indata) return 0;
    //cout<<"DDDDDDDDDDDDDDDDDDDDD";
    if (!d)
    {
        av_frame_free(&indata);
        return 0;
    }
    //cout<<"EEEEEEEEEEEEEEEEEEEEEE";

    uint8_t *data[2] = { 0 };
    data[0] = d;
    int re = swr_convert(actx,
                              data, indata->nb_samples,		//输出
                              (const uint8_t**)indata->data,indata->nb_samples	//输入
                              );
//    //cout << "sws_scale = " << re << endl;


    int outSize = re * indata->channels * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    av_frame_free(&indata);
    if (re <= 0) return re;
    return outSize;
}




























