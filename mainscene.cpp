#include "mainscene.h"
#include <QDebug>
MainScene::MainScene(QObject *parent) : QGraphicsScene(parent), cellColors(0)
{
    isClearing = false;
    isExtreme = false;
    isInterrupted = false;
    isEditable = true;
    roomHeight = 0;
    roomWidth = 0;
    prev.setX(-1);
    prev.setY(-1);
    zVal = 1;
    extreme.setX(-1);
    extreme.setY(-1);
    cellColors.push_back(QColor::fromRgb(0x00, 0xCC, 0x00));
    cellColors.push_back(QColor(Qt::red));
    cellColors.push_back(QColor::fromRgb(0x68, 0x68, 0x68));
    cellColors.push_back(QColor(Qt::white));
    cellColors.push_back(QColor::fromRgb(0x7A, 0xFF, 0xA7));
    cellColors.push_back(QColor::fromRgb(0x00, 0x66, 0xFF));
    lineColor = QColor(Qt::yellow);
    lineWidth = 3;
    stepDelay = 10;
    populateScene(21, 21);
}

MainScene::~MainScene()
{
    clearScene();
}

void MainScene::populateScene(int rows, int cols)
{
    if(rows < 3 || cols < 3 || rows > 99 || cols > 99)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);
    roomHeight = rows;
    roomWidth = cols;
    room.resize(roomHeight);
    for(int i = 0; i < roomHeight; ++i)
    {
        room[i].resize(roomWidth);
        for(int j = 0; j < roomWidth; ++j)
        {
            room[i][j] = new GraphicsCell();
            room[i][j]->setColor(cellColors.at(GraphicsCell::ElementType::FreeSpace));
            room[i][j]->setPos(j * room[i][j]->getCellSize(), i * room[i][j]->getCellSize());
            addItem(room[i][j]);
            room[i][j]->setZValue(-1);
        }
    }

    room[0][0]->setType(GraphicsCell::ElementType::StartingPoint);
    room[0][0]->setColor(cellColors[room[0][0]->getType()]);
    sizeOfCell = room[0][0]->getCellSize();
    room[roomHeight - 1][roomWidth - 1]->setType(GraphicsCell::ElementType::FinishingPoint);
    room[roomHeight - 1][roomWidth - 1]->setColor(cellColors.at(room[roomHeight - 1][roomWidth - 1]->getType()));
}

void MainScene::clearScene()
{
    for(int i = 0; i < roomHeight; ++i)
    {
        for(int j = 0; j < roomWidth; ++j)
        {
            removeItem(room[i][j]);
            delete room[i][j];
            room[i][j] = NULL;
        }
    }

    for(unsigned i = 0; i < drawnPath.size(); ++i)
        removeItem(drawnPath[i]);
    drawnPath.resize(0);
    zVal = 1;
}

std::vector<Point> MainScene::getFoundPath() const
{
    return foundPath;
}

void MainScene::setFoundPath(const std::vector<Point> &value)
{
    foundPath = value;
    historyIndex = 0;
}

Point MainScene::startingPoint() const
{
    for(int i = 0; i < roomHeight; i++)
    {
        for(int j = 0; j < roomWidth; j++)
        {
            if(room[i][j]->getType() == GraphicsCell::ElementType::StartingPoint)
                return Point(i, j);
        }
    }
    throw AlgorythmException(AlgorythmException::ErrorCode::StartingPointNotFoundError);
}

Point MainScene::finishingPoint() const
{
    for(int i = 0; i < roomHeight; i++)
    {
        for(int j = 0; j < roomWidth; j++)
        {
            if(room[i][j]->getType() == GraphicsCell::ElementType::FinishingPoint)
                return Point(i, j);
        }
    }
    throw AlgorythmException(AlgorythmException::ErrorCode::FinishingPointNotFoundError);
}

void MainScene::interrupt()
{
    isInterrupted = true;
}

void MainScene::hideProceccedCells()
{
    for(int i = 0; i < roomHeight; i++)
    {
        for(int j = 0; j < roomWidth; j++)
        {
            if(room[i][j]->getType() == GraphicsCell::ElementType::Visited
                    || room[i][j]->getType() == GraphicsCell::ElementType::Queued)
            room[i][j]->setColor(cellColors[GraphicsCell::ElementType::FreeSpace]);
        }
    }
}

void MainScene::hidePathLine()
{

    for(unsigned i = 0; i < drawnPath.size(); ++i)
        drawnPath[i]->setVisible(false);
}

void MainScene::showPathLine()
{
    for(unsigned i = 0; i < drawnPath.size(); ++i)
        drawnPath[i]->setVisible(true);
}

void MainScene::showProceccedCells()
{
    for(int i = 0; i < roomHeight; i++)
    {
        for(int j = 0; j < roomWidth; j++)
        {
            if(room[i][j]->getType() == GraphicsCell::ElementType::Visited
                    || room[i][j]->getType() == GraphicsCell::ElementType::Queued)
            room[i][j]->setColor(cellColors[room[i][j]->getType()]);
        }
    }
}

int MainScene::getStepDelay() const
{
    return stepDelay;
}

QColor MainScene::getLineColor() const
{
    return lineColor;
}

int MainScene::getLineWidth() const
{
    return lineWidth;
}

void MainScene::setStepDelay(int value)
{
    stepDelay = value;
}

QColor MainScene::getColorOfCell(int i)
{
    if(i < 0 || i > 5)
        throw MemoryException(MemoryException::ErrorCode::OutOfRangeIndexError);

    return cellColors.at(i);
}

std::vector<QColor> MainScene::getCellColors()
{
    return cellColors;
}

void MainScene::setNewCellColors(std::vector<QColor> newColors)
{
    cellColors = newColors;

    for(int i = 0; i < roomHeight; ++i)
        for(int j = 0; j < roomWidth; ++j)
            room[i][j]->setColor(cellColors[room[i][j]->getType()]);
}

void MainScene::setLineParams(QColor color, int width)
{
    lineColor = color;
    lineWidth = width;
    for(unsigned i = 0; i < drawnPath.size(); ++i)
    {
        drawnPath[i]->setPen(QPen(color, width));
        drawnPath[i]->update();
    }
}

void MainScene::drawPath()
{
    QPoint s;
    QPoint f;
    for(unsigned i = 1; i < foundPath.size(); ++i)
    {
        s.setX(foundPath[i - 1].Y()*sizeOfCell + sizeOfCell / 2);
        s.setY(foundPath[i - 1].X()*sizeOfCell + sizeOfCell / 2);
        f.setX(foundPath[i].Y()*sizeOfCell + sizeOfCell / 2);
        f.setY(foundPath[i].X()*sizeOfCell + sizeOfCell / 2);

        drawnPath.push_back(addLine(QLine(s, f), QPen(lineColor, lineWidth)));
        drawnPath.back()->setZValue(++zVal);
    }
}

bool MainScene::showPathHistory(const std::vector<SignalPoint> &pathHistory)
{
    QEventLoop loop;
    isInterrupted = false;
    for( ; historyIndex < pathHistory.size() && !isInterrupted; ++historyIndex)
    {
        if(room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->getType() == GraphicsCell::ElementType::StartingPoint
           || room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->getType() == GraphicsCell::ElementType::FinishingPoint)
            continue;

        if(pathHistory.at(historyIndex).isVisited())
        {
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setType(GraphicsCell::ElementType::Visited);
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setColor(cellColors[GraphicsCell::ElementType::Visited]);
        }
        else
        {
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setType(GraphicsCell::ElementType::Queued);
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setColor(cellColors[GraphicsCell::ElementType::Queued]);
        }

        QTimer::singleShot(stepDelay, &loop, SLOT(quit()));
        loop.exec();
    }

    if(historyIndex == pathHistory.size())
    {
        drawPath();
        return true;
    }
    else
        return false;
}

bool MainScene::performPathHistoryStep(const std::vector<SignalPoint> &pathHistory)
{
    if(room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->getType() != GraphicsCell::ElementType::StartingPoint
       && room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->getType() != GraphicsCell::ElementType::FinishingPoint)
    {
        if(pathHistory.at(historyIndex).isVisited())
        {
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setType(GraphicsCell::ElementType::Visited);
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setColor(cellColors[GraphicsCell::ElementType::Visited]);
        }
        else
        {
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setType(GraphicsCell::ElementType::Queued);
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setColor(cellColors[GraphicsCell::ElementType::Queued]);
        }
    }

    historyIndex++;
    if(historyIndex == pathHistory.size())
    {
        drawPath();
        return true;
    }
    else
        return false;
}

void MainScene::skipPathHistory(const std::vector<SignalPoint> &pathHistory)
{
    for( ; historyIndex < pathHistory.size(); ++historyIndex)
    {
        if(room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->getType() == GraphicsCell::ElementType::StartingPoint
           || room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->getType() == GraphicsCell::ElementType::FinishingPoint)
            continue;

        if(pathHistory.at(historyIndex).isVisited())
        {
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setType(GraphicsCell::ElementType::Visited);
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setColor(cellColors[GraphicsCell::ElementType::Visited]);
        }
        else
        {
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setType(GraphicsCell::ElementType::Queued);
            room[pathHistory.at(historyIndex).X()][pathHistory.at(historyIndex).Y()]->setColor(cellColors[GraphicsCell::ElementType::Queued]);
        }
    }
    drawPath();
}

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isEditable)
        return;
    if(event->buttons() & Qt::LeftButton)
    {
            QPointF mousePosition = event->scenePos();
            if(mousePosition.x() >= roomWidth * sizeOfCell || mousePosition.y() >= roomHeight * sizeOfCell
                    || mousePosition.x() < 0 || mousePosition.y() < 0)
                return;

            QTransform transform;
            QGraphicsItem *item;

            item = this->itemAt(mousePosition.x(), mousePosition.y(), transform);
            if(item == NULL)
                return;

            prev.setX(curr.X());
            prev.setY(curr.Y());

            curr.setX(mousePosition.x());
            curr.setY(mousePosition.y());

            if(curr.X()/sizeOfCell == prev.X()/sizeOfCell && curr.Y()/sizeOfCell == prev.Y()/sizeOfCell)
                return;

            if(room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->getType() == GraphicsCell::ElementType::StartingPoint
                    || room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->getType() == GraphicsCell::ElementType::FinishingPoint)
                return;

            if(isExtreme)
            {
                if(room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->getType() == GraphicsCell::ElementType::FreeSpace)
                {
                    room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setType(room[extreme.Y()/sizeOfCell][extreme.X()/sizeOfCell]->getType());
                    room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setColor(cellColors[room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->getType()]);


                    room[extreme.Y()/sizeOfCell][extreme.X()/sizeOfCell]->setType(GraphicsCell::ElementType::FreeSpace);
                    room[extreme.Y()/sizeOfCell][extreme.X()/sizeOfCell]->setColor(cellColors[GraphicsCell::ElementType::FreeSpace]);
                    extreme.setX(curr.X());
                    extreme.setY(curr.Y());
                }
                return;
            }

            if(isClearing)
            {
                if(room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->getType() == GraphicsCell::ElementType::FreeSpace)
                    return;
                room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setType(GraphicsCell::ElementType::FreeSpace);
                room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setColor(cellColors[GraphicsCell::ElementType::FreeSpace]);
            }
            else
            {
                if(room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->getType() == GraphicsCell::ElementType::Wall)
                    return;
                room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setType(GraphicsCell::ElementType::Wall);
                room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setColor(cellColors[GraphicsCell::ElementType::Wall]);
            }
            room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setZValue(++zVal);
            room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->startAnimation();
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void MainScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isEditable)
        return;
    if(event->button() == Qt::LeftButton)
    {
        QTransform transform;
        QGraphicsItem *item;

        QPointF mousePosition = event->scenePos();
        item = this->itemAt(mousePosition.x(), mousePosition.y(), transform);
        if(item == NULL)
            return;
        curr.setX(mousePosition.x());
        curr.setY(mousePosition.y());


        if(room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->getType() == GraphicsCell::ElementType::StartingPoint
                || room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->getType() == GraphicsCell::ElementType::FinishingPoint)
        {
            isExtreme = true;
            extreme.setX(curr.X());
            extreme.setY(curr.Y());
            return;
        }

        if(room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->getType() == GraphicsCell::ElementType::Wall)
        {
            isClearing = true;
            room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setType(GraphicsCell::ElementType::FreeSpace);
            room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setColor(cellColors[GraphicsCell::ElementType::FreeSpace]);
        }
        else
        {
            room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setType(GraphicsCell::ElementType::Wall);
            room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setColor(cellColors[GraphicsCell::ElementType::Wall]);
        }


        room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->setZValue(++zVal);
        room[curr.Y()/sizeOfCell][curr.X()/sizeOfCell]->startAnimation();
    }
    QGraphicsScene::mousePressEvent(event);
}

void MainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isClearing = false;
    isExtreme = false;
    prev.setX(-1);
    prev.setY(-1);
    extreme.setX(-1);
    extreme.setY(-1);
    QGraphicsScene::mouseReleaseEvent(event);
}

void MainScene::setRoom(const std::vector<std::vector<GraphicsCell> > &convertedRoom)
{
    clearPath();
    resize(convertedRoom.size(), convertedRoom[0].size());

    for(int i = 0; i < roomHeight; ++i)
    {
        for(int j = 0; j < roomWidth; ++j)
        {
            room[i][j]->setType(convertedRoom[i][j].getType());
            room[i][j]->setColor(cellColors[convertedRoom[i][j].getType()]);
        }
    }
}

void MainScene::setEditing(bool value)
{
    isEditable = value;
}

void MainScene::clearPath()
{
    if(drawnPath.size() > 0)
    {
        for(unsigned i = 0; i < drawnPath.size(); ++i)
            removeItem(drawnPath[i]);
        drawnPath.resize(0);
    }

    for(int i = 0; i < roomHeight; ++i)
    {
        for(int j = 0; j < roomWidth; ++j)
        {
            if(room[i][j]->getType() == GraphicsCell::ElementType::Visited
                    || room[i][j]->getType() == GraphicsCell::ElementType::Queued)
            {
                room[i][j]->setType(GraphicsCell::ElementType::FreeSpace);
                room[i][j]->setColor(cellColors[GraphicsCell::ElementType::FreeSpace]);
            }
        }
    }
}

void MainScene::clearAll()
{
    clearPath();
    for(int i = 0; i < roomHeight; ++i)
    {
        for(int j = 0; j < roomWidth; ++j)
        {
            if(room[i][j]->getType() != GraphicsCell::ElementType::FreeSpace
                    && room[i][j]->getType() != GraphicsCell::ElementType::StartingPoint
                    && room[i][j]->getType() != GraphicsCell::ElementType::FinishingPoint)
            {
                room[i][j]->setType(GraphicsCell::ElementType::FreeSpace);
                room[i][j]->setColor(cellColors[GraphicsCell::ElementType::FreeSpace]);
            }
            room[i][j]->setZValue(-1);
        }
    }
    zVal = 1;
}

GraphicsCell MainScene::at(int i, int j) const
{
    if(i < 0 || i >= roomHeight || j < 0 || j >= roomWidth)
        throw MemoryException(MemoryException::ErrorCode::OutOfRangeIndexError);

    return *(room[i][j]);
}

void MainScene::resize(int rows, int cols)
{
    if(rows < 3 || cols < 3 || rows > 99 || cols > 99)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);

    if(rows == roomHeight && cols == roomWidth)
        return;

    clearScene();
    populateScene(rows, cols);
}

int MainScene::getRoomWidth() const
{
    return roomWidth;
}

int MainScene::getRoomHeight() const
{
    return roomHeight;
}
