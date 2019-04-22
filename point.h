#ifndef POINT_H
#define POINT_H

class Point
{
private:
    int xCoordinate;
    int yCoordinate;

public:
    Point(int x = 0, int y = 0);
    Point(const Point &obj);
    int X() const;
    int Y() const;
    void setX(int x);
    void setY(int y);
};

#endif // POINT_H
