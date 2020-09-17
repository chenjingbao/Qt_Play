#ifndef XSLIDER_H
#define XSLIDER_H
#include <QMouseEvent>
#include <QSlider>
class XSlider : public QSlider
{
public:
    XSlider();
    void mousePressEvent(QMouseEvent *e);
};

#endif // XSLIDER_H
