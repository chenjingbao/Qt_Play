#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void timeEvent(QTimerEvent *e);

    void resizeEvent(QResizeEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void SetPause(bool isPause);
public slots:
    void OpenFile();
    void PlayOrPause();
    void SliderPress();
    void SliderRelease();

public:
    Ui::Widget *ui;
    bool isSliderPress = false;
};
#endif // WIDGET_H
