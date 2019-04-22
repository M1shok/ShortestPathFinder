#include "point.h"

Point::Point(int x, int y)
{
    xCoordinate = x;
    yCoordinate = y;
}

Point::Point(const Point &obj)
{
    xCoordinate = obj.xCoordinate;
    yCoordinate = obj.yCoordinate;
}

int Point::X() const
{
    return xCoordinate;
}

int Point::Y() const
{
    return yCoordinate;
}

void Point::setX(int x)
{
    xCoordinate = x;
}

void Point::setY(int y)
{
    yCoordinate = y;
}
