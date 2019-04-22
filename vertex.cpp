#include "vertex.h"

Vertex::Vertex(const Point &obj, bool isPassable, int pathLength): Point(obj), passable(isPassable)
{
    this->pathLength = pathLength;
    this->visited = false;
    this->underConsideration = false;
}

Vertex::Vertex(int x, int y, bool isPassable, int pathLength): Point(x, y), passable(isPassable)
{
    this->pathLength = pathLength;
    this->visited = false;
    this->underConsideration = false;
}

Vertex::Vertex(const Vertex &obj): Point(obj), passable(obj.passable)
{
    this->pathLength = obj.pathLength;
    this->visited = obj.visited;
    this->underConsideration = false;
}

bool Vertex::isPassable() const
{
    return passable;
}

int Vertex::pathLengthGet() const
{
    return pathLength;
}

bool Vertex::isVisited() const
{
    return visited;
}

bool Vertex::isUnderConsideration()
{
    return underConsideration;
}

void Vertex::setUnderConsideration(bool mode)
{
    underConsideration = mode;
}

void Vertex::pathLengthSet(int newPathLength)
{
    pathLength = newPathLength;
}

void Vertex::visit()
{
    visited = true;
}
