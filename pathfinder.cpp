#include "pathfinder.h"

PathFinder::PathFinder(std::vector<std::vector<bool> > convertedRoom, Point start, Point finish)
{
    rowQuantity = convertedRoom.size();
    if(rowQuantity == 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);
    columnQuantity = convertedRoom[0].size();
    if(columnQuantity == 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);

    room = std::vector<std::vector<Vertex*> >(rowQuantity, std::vector<Vertex*>(columnQuantity));

    for(int i = 0; i < rowQuantity; i++)
        for(int j = 0; j < columnQuantity; j++)
            room[i][j] = new Vertex(i, j, convertedRoom[i][j]);

    startingVertex = room[start.X()][start.Y()];
    finishingVertex = room[finish.X()][finish.Y()];
}

Vertex* PathFinder::leftNeighbour(const Vertex *currentVertex) const
{
    if(currentVertex->Y() > 0)
        return room[currentVertex->X()][currentVertex->Y() - 1];
    else
        return NULL;
}

Vertex* PathFinder::rightNeighbour(const Vertex *currentVertex) const
{
    if(currentVertex->Y() + 1 < columnQuantity)
        return room[currentVertex->X()][currentVertex->Y() + 1];
    else
        return NULL;
}

Vertex* PathFinder::upperNeighbour(const Vertex *currentVertex) const
{
    if(currentVertex->X() > 0)
        return room[currentVertex->X() - 1][currentVertex->Y()];
    else
        return NULL;
}

Vertex* PathFinder::lowerNeighbour(const Vertex *currentVertex) const
{
    if(currentVertex->X() + 1 < rowQuantity)
        return room[currentVertex->X() + 1][currentVertex->Y()];
    else
        return NULL;
}

std::vector<Point> PathFinder::buildPath() const
{
    std::vector<Point> foundPath;
    Vertex* currentVertex = finishingVertex;

    while(currentVertex != startingVertex)
    {
        foundPath.push_back(Point(currentVertex->X(), currentVertex->Y()));

        if(upperNeighbour(currentVertex) != NULL && upperNeighbour(currentVertex)->isVisited() && upperNeighbour(currentVertex)->pathLengthGet() + 1 == currentVertex->pathLengthGet())
        {
            currentVertex = upperNeighbour(currentVertex);
        }
        else if(leftNeighbour(currentVertex) != NULL && leftNeighbour(currentVertex)->isVisited() && leftNeighbour(currentVertex)->pathLengthGet() + 1 == currentVertex->pathLengthGet())
        {
            currentVertex = leftNeighbour(currentVertex);
        }
        else if(lowerNeighbour(currentVertex) != NULL && lowerNeighbour(currentVertex)->isVisited() && lowerNeighbour(currentVertex)->pathLengthGet() + 1 == currentVertex->pathLengthGet())
        {
            currentVertex = lowerNeighbour(currentVertex);
        }
        else
        {
            currentVertex = rightNeighbour(currentVertex);
        }
    }

    foundPath.push_back(Point(startingVertex->X(), currentVertex->Y()));
    return foundPath;
}

int PathFinder::euristicFunctionManhattan(const Vertex *currentVertex) const
{
    return currentVertex->pathLengthGet() + abs(currentVertex->X() - finishingVertex->X()) + abs(currentVertex->Y() - finishingVertex->Y());
}

double PathFinder::euristicFunctionEuclidean(const Vertex *currentVertex) const
{
    return currentVertex->pathLengthGet() + sqrt((currentVertex->X() - finishingVertex->X()) * (currentVertex->X() - finishingVertex->X()) + (currentVertex->Y() - finishingVertex->Y()) * (currentVertex->Y() - finishingVertex->Y()));
}

PathFinderResult PathFinder::breadthFirstSearch()
{
    PathFinderResult result;
    std::queue<Vertex*> searchingQueue;
    Vertex *currentVertex;
    long long start, finish;
    start = clock();
    startingVertex->pathLengthSet(0);
    startingVertex->visit();
    searchingQueue.push(startingVertex);

    while(!finishingVertex->isVisited() && !searchingQueue.empty())
    {
        Vertex* neighbour;
        currentVertex = searchingQueue.front();
        searchingQueue.pop();
        currentVertex->setUnderConsideration(false);
        currentVertex->visit();
        result.pushSignalPoint(SignalPoint(Point(currentVertex->X(), currentVertex->Y()), true));

        neighbour = upperNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration())
        {
            neighbour->setUnderConsideration(true);
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingQueue.push(neighbour);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = leftNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration())
        {
            neighbour->setUnderConsideration(true);
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingQueue.push(neighbour);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = lowerNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration())
        {
            neighbour->setUnderConsideration(true);
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingQueue.push(neighbour);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = rightNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration())
        {
            neighbour->setUnderConsideration(true);
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingQueue.push(neighbour);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }
    }
    finish = clock();
    result.setTimeTaken(finish - start);
    if(finishingVertex->isVisited())
        result.setPath(buildPath());
    return result;
}

PathFinderResult PathFinder::depthFirstSearch()
{
    PathFinderResult result;
    std::stack<Vertex*> searchingStack;
    Vertex *currentVertex;

    long long start, finish;
    start = clock();
    startingVertex->pathLengthSet(0);
    searchingStack.push(startingVertex);

    while(!finishingVertex->isVisited() && !searchingStack.empty())
    {
        Vertex* neighbour;
        currentVertex = searchingStack.top();
        searchingStack.pop();        
        currentVertex->setUnderConsideration(false);
        currentVertex->visit();
        result.pushSignalPoint(SignalPoint(Point(currentVertex->X(), currentVertex->Y()), true));

        neighbour = upperNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration())
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingStack.push(neighbour);
            neighbour->setUnderConsideration(true);
            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = leftNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration())
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingStack.push(neighbour);
            neighbour->setUnderConsideration(true);
            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = lowerNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration())
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingStack.push(neighbour);
            neighbour->setUnderConsideration(true);
            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = rightNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration())
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingStack.push(neighbour);
            neighbour->setUnderConsideration(true);
            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }
    }

    finish = clock();
    result.setTimeTaken(finish - start);
    if(finishingVertex->isVisited())
        result.setPath(buildPath());

    return result;
}

PathFinderResult PathFinder::depthFirstSearchFull()
{
    PathFinderResult result;
    std::stack<Vertex*> searchingStack;
    Vertex *currentVertex;

    long long start, finish;
    start = clock();
    startingVertex->pathLengthSet(0);
    searchingStack.push(startingVertex);

    while(!searchingStack.empty())
    {
        Vertex* neighbour;
        currentVertex = searchingStack.top();
        searchingStack.pop();
        currentVertex->visit();
        currentVertex->setUnderConsideration(false);
        result.pushSignalPoint(SignalPoint(Point(currentVertex->X(), currentVertex->Y()), true));

        neighbour = upperNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            if(!neighbour->isUnderConsideration())
            {
                searchingStack.push(neighbour);
                neighbour->setUnderConsideration(true);
            }

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = leftNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            if(!neighbour->isUnderConsideration())
            {
                searchingStack.push(neighbour);
                neighbour->setUnderConsideration(true);
            }

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = lowerNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            if(!neighbour->isUnderConsideration())
            {
                searchingStack.push(neighbour);
                neighbour->setUnderConsideration(true);
            }

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = rightNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            if(!neighbour->isUnderConsideration())
            {
                searchingStack.push(neighbour);
                neighbour->setUnderConsideration(true);
            }

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }
    }

    finish = clock();
    result.setTimeTaken(finish - start);
    if(finishingVertex->isVisited())
        result.setPath(buildPath());

    return result;
}

PathFinderResult PathFinder::dijkstraSearch()
{
    PathFinderResult result;
    Vertex *currentVertex;
    std::list<Vertex*> searchingVector;

    long long start, finish;
    start = clock();
    startingVertex->pathLengthSet(0);
    searchingVector.push_back(startingVertex);

    while(!finishingVertex->isVisited() && !searchingVector.empty())
    {
        std::list<Vertex*>::iterator itDelete = searchingVector.begin();
        for(std::list<Vertex*>::iterator it = ++searchingVector.begin(); it != searchingVector.end(); it++)
        {
            if((*it)->pathLengthGet() < (*itDelete)->pathLengthGet())
                itDelete = it;
        }

        currentVertex = (*itDelete);
        searchingVector.erase(itDelete);

        currentVertex->setUnderConsideration(false);
        currentVertex->visit();
        result.pushSignalPoint(SignalPoint(Point(currentVertex->X(), currentVertex->Y()), true));

        Vertex *neighbour;
        neighbour = upperNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = leftNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = lowerNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = rightNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }
    }

    finish = clock();
    result.setTimeTaken(finish - start);
    if(finishingVertex->isVisited())
        result.setPath(buildPath());

    return result;
}

PathFinderResult PathFinder::aStarSearchEuclidean()
{
    PathFinderResult result;
    Vertex *currentVertex;
    std::list<Vertex*> searchingVector;

    long long start, finish;
    start = clock();
    startingVertex->pathLengthSet(0);
    searchingVector.push_back(startingVertex);

    while(!finishingVertex->isVisited() && !searchingVector.empty())
    {
        std::list<Vertex*>::iterator itDelete = searchingVector.begin();
        for(std::list<Vertex*>::iterator it = ++searchingVector.begin(); it != searchingVector.end(); it++)
        {
            if(euristicFunctionEuclidean(*it) < euristicFunctionEuclidean(*itDelete))
                itDelete = it;
        }
        currentVertex = (*itDelete);
        searchingVector.erase(itDelete);

        currentVertex->setUnderConsideration(false);
        currentVertex->visit();
        result.pushSignalPoint(SignalPoint(Point(currentVertex->X(), currentVertex->Y()), true));

        Vertex *neighbour;
        neighbour = upperNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = leftNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = lowerNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = rightNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }
    }

    finish = clock();
    result.setTimeTaken(finish - start);
    if(finishingVertex->isVisited())
        result.setPath(buildPath());

    return result;
}

PathFinderResult PathFinder::aStarSearchManhattan()
{
    PathFinderResult result;
    Vertex *currentVertex;
    std::list<Vertex*> searchingVector;

    long long start, finish;
    start = clock();
    startingVertex->pathLengthSet(0);
    searchingVector.push_back(startingVertex);

    while(!finishingVertex->isVisited() && !searchingVector.empty())
    {
        std::list<Vertex*>::iterator itDelete = searchingVector.begin();
        for(std::list<Vertex*>::iterator it = ++searchingVector.begin(); it != searchingVector.end(); it++)
        {
            if(euristicFunctionManhattan(*it) < euristicFunctionManhattan(*itDelete))
                itDelete = it;
        }
        currentVertex = (*itDelete);
        searchingVector.erase(itDelete);

        currentVertex->setUnderConsideration(false);
        currentVertex->visit();
        result.pushSignalPoint(SignalPoint(Point(currentVertex->X(), currentVertex->Y()), true));

        Vertex *neighbour;
        neighbour = upperNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = leftNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = lowerNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }

        neighbour = rightNeighbour(currentVertex);
        if(neighbour != NULL && neighbour->isPassable() && !neighbour->isVisited() && !neighbour->isUnderConsideration() && neighbour->pathLengthGet() > currentVertex->pathLengthGet() + 1)
        {
            neighbour->pathLengthSet(currentVertex->pathLengthGet() + 1);
            searchingVector.push_back(neighbour);
            neighbour->setUnderConsideration(true);

            result.pushSignalPoint(SignalPoint(Point(neighbour->X(), neighbour->Y()), false));
        }
    }

    finish = clock();
    result.setTimeTaken(finish - start);
    if(finishingVertex->isVisited())
        result.setPath(buildPath());

    return result;
}

PathFinder::~PathFinder()
{
    for(int i = 0; i < rowQuantity; i++)
        for(int j = 0; j < columnQuantity; j++)
            delete room[i][j];
}
