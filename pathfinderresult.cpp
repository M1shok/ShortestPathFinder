#include "pathfinderresult.h"

std::vector<SignalPoint> PathFinderResult::getPathFinderHistory() const
{
    return pathFinderHistory;
}

void PathFinderResult::pushSignalPoint(const SignalPoint &point)
{
    pathFinderHistory.push_back(point);
}

bool PathFinderResult::reachedDestination() const
{
    if(path.size() == 0)
        return false;
    else
        return true;
}

std::vector<Point> PathFinderResult::getPath() const
{
    return path;
}


void PathFinderResult::setPath(const std::vector<Point> &value)
{
    path = value;
}

long long PathFinderResult::getTimeTaken() const
{
    return timeTaken;
}

void PathFinderResult::setTimeTaken(long long value)
{
    timeTaken = value;
}

PathFinderResult::PathFinderResult(): path(0), pathFinderHistory(0)
{
    timeTaken = 0;
}
