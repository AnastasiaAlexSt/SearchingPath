#include "gridview.h"

GridView::GridView(QWidget *parent) : QGraphicsView(parent), parent_(parent)
{

}

void GridView::ScaleView(qreal scale_factor)
{
    qreal factor = transform().scale(scale_factor, scale_factor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scale_factor, scale_factor);
}
void GridView::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
    {
        ScaleView(1.15);
    }
    else
    {
        ScaleView(1/1.15);
    }
}
