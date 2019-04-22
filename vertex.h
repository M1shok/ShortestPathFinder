#include <limits.h>
#include "point.h"

#ifndef VERTEX_H
#define VERTEX_H

class Vertex: public Point
{
private:
    const bool passable;
    bool visited;
    bool underConsideration;
    int pathLength;

public:
    Vertex(int x = 0, int y = 0, bool isPassable = false, int pathLength = INT_MAX);
    Vertex(const Point &obj, bool isPassable = false, int pathLength = INT_MAX);
    Vertex(const Vertex &obj);
    bool isPassable() const;
    bool isVisited() const;
    bool isUnderConsideration();
    void setUnderConsideration(bool mode = true);
    void visit();
    int pathLengthGet() const;
    void pathLengthSet(int newPathLength);
};

#endif // VERTEX_H
