#ifndef PATHFINDERRESULT_H
#define PATHFINDERRESULT_H

#include "signalpoint.h"
#include <vector>

class PathFinderResult
{
private:
    long long timeTaken;
    std::vector<Point> path;
    std::vector<SignalPoint> pathFinderHistory;

public:
    PathFinderResult();
    std::vector<Point> getPath() const;
    void setPath(const std::vector<Point> &value);
    std::vector<SignalPoint> getPathFinderHistory() const;
    void pushSignalPoint(const SignalPoint &point);
    bool reachedDestination() const;
    long long getTimeTaken() const;
    void setTimeTaken(long long value);
};

#endif // PATHFINDERRESULT_H
