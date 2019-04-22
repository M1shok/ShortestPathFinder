#ifndef MAINGRAPHICSVIEW_H
#define MAINGRAPHICSVIEW_H


#include <QGraphicsView>
#include <QWheelEvent>
#include <QPointF>
class MainGraphicsView : public QGraphicsView
{
public:
    MainGraphicsView(QObject *parent = 0);
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    const double scaleFactor;
    QPointF origin;
};

#endif // MAINGRAPHICSVIEW_H
