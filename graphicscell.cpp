#include "graphicscell.h"
GraphicsCell::GraphicsCell()
{
    cellSize = 30;
    scaleValue = 1;
    type = FreeSpace;
    cellRectangle = boundingRect();
}

GraphicsCell::GraphicsCell(const GraphicsCell &cell): QObject(), QGraphicsItem()
{
    cellSize = cell.cellSize;
    scaleValue = cell.scaleValue;
    type = cell.type;
    cellRectangle = cell.cellRectangle;
}

QRectF GraphicsCell::boundingRect() const
{
    return QRectF(0, 0, cellSize, cellSize);
}

void GraphicsCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(0.5);
    QBrush brush(color);
    painter->setPen(pen);
    painter->fillRect(cellRectangle, brush);
    painter->drawRect(cellRectangle);
}

GraphicsCell::ElementType GraphicsCell::getType() const
{
    return type;
}

void GraphicsCell::setType(const ElementType &value)
{
    type = value;
}

double GraphicsCell::getScaleValue() const
{
    return scaleValue;
}

void GraphicsCell::setScaleValue(double value)
{
    QPointF p = cellRectangle.center();
    setTransformOriginPoint(p);
    scaleValue = value;
    setScale(scaleValue);
}

void GraphicsCell::startAnimation()
{
    QPropertyAnimation* animation = new QPropertyAnimation(this, "scaleFactor");
    animation->setDuration(150);
    animation->setStartValue(1.5);
    animation->setEndValue(1);
    animation->start();
}
QColor GraphicsCell::getColor() const
{
    return color;
}

void GraphicsCell::setColor(const QColor &value)
{
    color = value;
    update();
}

int GraphicsCell::getCellSize() const
{
    return cellSize;
}

