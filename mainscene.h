#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QtWidgets>
#include "graphicscell.h"
#include "pathfinderresult.h"
#include "memoryexception.h"
#include "algorythmexception.h"

class MainScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MainScene(QObject *parent = 0);
    ~MainScene();
    GraphicsCell at(int i, int j) const;
    void resize(int rows, int cols);
    void drawPath();
    bool showPathHistory(const std::vector<SignalPoint> &value);
    bool performPathHistoryStep(const std::vector<SignalPoint> &value);
    void skipPathHistory(const std::vector<SignalPoint> &value);
    void clearPath();
    void clearAll();
    int getRoomHeight() const;
    int getRoomWidth() const;
    void setRoom(const std::vector<std::vector<GraphicsCell> > &roomFromFile);
    void setEditing(bool value);
    QColor getColorOfCell(int i);
    std::vector<QColor> getCellColors();
    int getStepDelay() const;
    QColor getLineColor() const;
    int getLineWidth() const;
    void interrupt();
    void hideProceccedCells();
    void hidePathLine();
    void showPathLine();
    void showProceccedCells();
    Point startingPoint() const;
    Point finishingPoint() const;
    void setFoundPath(const std::vector<Point> &value);
    std::vector<Point> getFoundPath() const;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void populateScene(int rows, int cols);
    void clearScene();
    std::vector<QGraphicsLineItem *> drawnPath;
    std::vector<std::vector<GraphicsCell *> > room;
    unsigned int historyIndex;
    std::vector<Point> foundPath;
    std::vector<QColor> cellColors;
    int roomHeight;
    int roomWidth;
    bool isClearing;
    bool isExtreme;
    bool isEditable;
    bool isInterrupted;
    Point curr;
    Point prev;
    Point extreme;
    int zVal;
    int sizeOfCell;
    QColor lineColor;
    int lineWidth;
    int stepDelay;
public slots:
    void setNewCellColors(std::vector<QColor> newColors);
    void setLineParams(QColor color, int width);
    void setStepDelay(int value);
};

#endif // MAINSCENE_H
