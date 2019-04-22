#ifndef GRAPHICSCELL_H
#define GRAPHICSCELL_H

#include <QPainter>
#include <QGraphicsItem>
#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QPropertyAnimation>

class GraphicsCell : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(double scaleFactor READ getScaleValue WRITE setScaleValue)
public:
    enum ElementType {StartingPoint, FinishingPoint, Wall, FreeSpace, Visited, Queued, ElementsQuantity};

    GraphicsCell();
    GraphicsCell(const GraphicsCell &cell);
    ElementType getType() const;
    void setType(const ElementType &value);
    double getScaleValue() const;
    void setScaleValue(double value);
    void startAnimation();
    QColor getColor() const;
    void setColor(const QColor &value);
    int getCellSize() const;
private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    ElementType type;
    int cellSize;
    QColor color;
    double scaleValue;
    QRectF cellRectangle;
};

#endif // GRAPHICSCELL_H
