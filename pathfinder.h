#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <queue>
#include <stack>
#include <cstdlib>
#include <cmath>
#include <list>
#include <ctime>
#include "vertex.h"
#include "memoryexception.h"
#include "pathfinderresult.h"

class PathFinder
{
private:
    int rowQuantity;
    int columnQuantity;
    Vertex* startingVertex;
    Vertex* finishingVertex;
    std::vector<std::vector<Vertex*> > room;

    Vertex* leftNeighbour(const Vertex* currentVertex) const;
    Vertex* rightNeighbour(const Vertex* currentVertex) const;
    Vertex* upperNeighbour(const Vertex* currentVertex) const;
    Vertex* lowerNeighbour(const Vertex* currentVertex) const;
    std::vector<Point> buildPath() const;
    int euristicFunctionManhattan(const Vertex* currentVertex) const;
    double euristicFunctionEuclidean(const Vertex* currentVertex) const;

public:
    PathFinder(std::vector<std::vector<bool> > convertedRoom, Point start, Point finish);
    PathFinderResult breadthFirstSearch();
    PathFinderResult depthFirstSearch();
    PathFinderResult depthFirstSearchFull();
    PathFinderResult dijkstraSearch();
    PathFinderResult aStarSearchEuclidean();
    PathFinderResult aStarSearchManhattan();
    ~PathFinder();
};

#endif // PATHFINDER_H
