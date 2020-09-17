#include "xslider.h"

XSlider::XSlider()
{

}

void XSlider::mousePressEvent(QMouseEvent *e)
{
    int pos = (double)e->pos().x()/(double)width();
    setValue(pos*this->maximum());
    QSlider::sliderReleased();
}
