#include "maingraphicsview.h"

MainGraphicsView::MainGraphicsView(QObject *parent) : QGraphicsView(), scaleFactor(1.1) {}

void MainGraphicsView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if(event->delta() > 0)
    {
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        scale(1/scaleFactor, 1/scaleFactor);
    }
     QGraphicsView::wheelEvent(event);
}

void MainGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
       {
           origin.setX(event->x());
           origin.setY(event->y());
       }
    QGraphicsView::mousePressEvent(event);
}

void MainGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::MidButton)
        {
            QPointF oldp = mapToScene(origin.x(), origin.y());
            QPointF newP = mapToScene(event->pos());
            QPointF translation = newP - oldp;

            translate(translation.x(), translation.y());

            origin.setX(event->x());
            origin.setY(event->y());
        }
    QGraphicsView::mouseMoveEvent(event);
}
