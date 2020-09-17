#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "xdemuxthread.h"
static XDemuxThread dt;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    dt.start();
    startTimer(40);
}

Widget::~Widget()
{
    dt.Close();
    delete ui;
}

void Widget::timeEvent(QTimerEvent *e)
{
    if (isSliderPress) return;
    long long total = dt.totalMs;
    if (dt.totalMs>0)
    {
        double pos =(double) dt.pts/(double)total;
        int v =  ui->playPos->maximum() * pos;
        ui->playPos->setValue(v);
    }

}

void Widget::resizeEvent(QResizeEvent *event)
{

    ui->playPos->move(100, this->height()-50);
    ui->playPos->resize(this->width()-100, ui->playPos->height())
    ui->isplay->move(ui.openFile->x()+ui.openFile->width()+10, ui->openFile->y());
    ui->openFile->move(100, this->height()-150);
    ui->video->resize(this->size());
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(isFullScreen())
        this->showNormal();
    else
        this->showFullScreen();
}

void Widget::SetPause(bool isPause)
{
    if(isPause)
        ui->isPlay->setText("play");
    else
        ui->isPlay->setText("pause");
}

void Widget::OpenFile()
{
    QString name =QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("choose file"));
    if (name.isEmpty())return;
    this->setWindowTitle(name);

    if(!dt.Open(name.toLocal8Bit(), ui->video))
    {
        QMessageBox::information(0, "error", "open file failed!");
        return;
    }
    setPause(dt.isPause);
    //qDebug() << name;
}

void Widget::PlayOrPause()
{
        bool isPause = dt.isPause;
        SetPause(isPause);
        dt.SetPause(isPause);
}

void Widget::SliderPress()
{
    isSliderPress = true;
}

void Widget::SliderRelease()
{
    isSliderPress = false;
    double pos = 0.0;
    pos = (double)ui->playPos->value()/(double)ui->playPos->maximum();
    dt.Seek(pos);
}




