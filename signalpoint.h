#ifndef SIGNALPOINT_H
#define SIGNALPOINT_H

#include "point.h"

class SignalPoint : public Point
{
private:
    bool signalVisited;
public:
    SignalPoint(int x = 0, int y = 0, bool visited = false);
    SignalPoint(const Point &point, bool visited = false);
    bool isVisited() const;
    void setVisited(bool value);
};

#endif // SIGNALPOINT_H
