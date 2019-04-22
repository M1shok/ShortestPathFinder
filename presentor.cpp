#include "presentor.h"

Presentor::Presentor()
{

}

std::vector<std::vector<GraphicsCell> > Presentor::intToGraphicsCell(const std::vector<std::vector<int> > &convertedRoom)
{
    int rowQuantity = convertedRoom.size();
    if(rowQuantity == 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);
    int columnQuantity = convertedRoom[0].size();
    if(columnQuantity == 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);

    std::vector<std::vector<GraphicsCell> > room(rowQuantity, std::vector<GraphicsCell>(columnQuantity));

    bool start = false, finish = false;
    for(int i = 0; i < rowQuantity; i++)
    {
        for(int j = 0; j < columnQuantity; j++)
        {
            switch (convertedRoom[i][j])
            {
            case 0:
                if(start)
                    throw AlgorythmException(AlgorythmException::ErrorCode::StartingPointDuplicateError);

                start = true;
                room[i][j].setType(GraphicsCell::ElementType::StartingPoint);
                break;
            case 1:
                if(finish)
                    throw AlgorythmException(AlgorythmException::ErrorCode::FinishingPointDuplicateError);

                finish = true;
                room[i][j].setType(GraphicsCell::ElementType::FinishingPoint);
                break;
            case 2:
                room[i][j].setType(GraphicsCell::ElementType::Wall);
                break;
            case 3:
                room[i][j].setType(GraphicsCell::ElementType::FreeSpace);
                break;
            }
        }
    }

    if(!start)
        throw AlgorythmException(AlgorythmException::ErrorCode::StartingPointNotFoundError);
    if(!finish)
        throw AlgorythmException(AlgorythmException::ErrorCode::FinishingPointNotFoundError);

    return room;
}

std::vector<std::vector<GraphicsCell> > Presentor::boolToGraphicsCell(const std::vector<std::vector<bool> > &convertedRoom)
{
    int rowQuantity = convertedRoom.size();
    if(rowQuantity == 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);

    int columnQuantity = convertedRoom[0].size();
    if(columnQuantity == 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);

    std::vector<std::vector<GraphicsCell> > room(rowQuantity, std::vector<GraphicsCell>(columnQuantity));

    for(int i = 0; i < rowQuantity; i++)
    {
        for(int j = 0; j < columnQuantity; j++)
        {
            if(convertedRoom[i][j])
                room[i][j].setType(GraphicsCell::ElementType::FreeSpace);
            else
                room[i][j].setType(GraphicsCell::ElementType::Wall);
        }
    }

    bool start = false;
    for(int i = 0; i < rowQuantity && !start; i++)
    {
        for(int j = 0; j < columnQuantity && !start; j++)
        {
            if(room[i][j].getType() == GraphicsCell::ElementType::FreeSpace)
            {
                room[i][j].setType(GraphicsCell::ElementType::StartingPoint);
                start = true;
            }
        }
    }

    bool finish = false;
    for(int i = rowQuantity - 1; i >= 0 && !finish; i--)
    {
        for(int j = columnQuantity - 1; j >= 0 && !finish; j--)
        {
            if(room[i][j].getType() == GraphicsCell::ElementType::FreeSpace)
            {
                room[i][j].setType(GraphicsCell::ElementType::FinishingPoint);
                finish = true;
            }
        }
    }

    if(!start)
        throw AlgorythmException(AlgorythmException::ErrorCode::StartingPointNotFoundError);
    if(!finish)
        throw AlgorythmException(AlgorythmException::ErrorCode::FinishingPointNotFoundError);

    return room;
}

std::vector<std::vector<bool> > Presentor::sceneToBool(const MainScene &scene)
{
    int rowQuantity = scene.getRoomHeight();
    int columnQuantity = scene.getRoomWidth();
    if(rowQuantity <= 0 || columnQuantity <= 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);

    std::vector<std::vector<bool> > room(rowQuantity, std::vector<bool>(columnQuantity));

    for(int i = 0; i < rowQuantity; i++)
    {
        for(int j = 0; j < columnQuantity; j++)
        {
            if(scene.at(i, j).getType() == GraphicsCell::ElementType::Wall)
                room[i][j] = false;
            else
                room[i][j] = true;
        }
    }

    return room;
}

std::vector<std::vector<int> > Presentor::sceneToInt(const MainScene &scene)
{
    int rowQuantity = scene.getRoomHeight();
    int columnQuantity = scene.getRoomWidth();
    if(rowQuantity <= 0 || columnQuantity <= 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);

    std::vector<std::vector<int> > room(rowQuantity, std::vector<int>(columnQuantity));

    for(int i = 0; i < rowQuantity; i++)
    {
        for(int j = 0; j < columnQuantity; j++)
        {
            switch (scene.at(i, j).getType())
            {
            case GraphicsCell::ElementType::StartingPoint:
                room[i][j] = 0;
                break;
            case GraphicsCell::ElementType::FinishingPoint:
                room[i][j] = 1;
                break;
            case GraphicsCell::ElementType::Wall:
                room[i][j] = 2;
                break;
            default:
                room[i][j] = 3;
                break;
            }
        }
    }

    return room;
}


