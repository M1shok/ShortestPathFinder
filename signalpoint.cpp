#include "signalpoint.h"

bool SignalPoint::isVisited() const
{
    return signalVisited;
}

void SignalPoint::setVisited(bool value)
{
    signalVisited = value;
}

SignalPoint::SignalPoint(int x, int y, bool visited): Point(x, y)
{
    signalVisited = visited;
}

SignalPoint::SignalPoint(const Point &point, bool visited): Point(point)
{
    signalVisited = visited;
}
