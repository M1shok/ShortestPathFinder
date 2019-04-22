#include "mazegenerator.h"

MazeGenerator::MazeGenerator() {}

std::vector<std::vector<bool> > MazeGenerator::generate(int rows, int columns)
{
    srand(time(NULL));
    std::vector<std::vector<bool> > maze(rows, std::vector<bool>(columns, 0));

    for(int i = 0; i < rows; i += 2)
        for(int j = 0; j < columns; j += 2)
            maze[i][j] = true;

    std::vector<std::vector<bool> > visitedPoints(rows, std::vector<bool>(columns, 0));
    std::stack<Point> backtracker;
    Point startingPoint;
    startingPoint.setX((rand() % ((rows + 1) / 2)) * 2);
    startingPoint.setY((rand() % ((columns + 1) / 2)) * 2);
    backtracker.push(startingPoint);
    while(!backtracker.empty())
    {
        Point currentPoint = backtracker.top();
        std::vector<Point> neighbours;
        visitedPoints[currentPoint.X()][currentPoint.Y()] = true;

        if(currentPoint.X() - 2 >= 0 && !visitedPoints[currentPoint.X() - 2][currentPoint.Y()])
            neighbours.push_back(Point(currentPoint.X() - 2, currentPoint.Y()));
        if(currentPoint.Y() - 2 >= 0 && !visitedPoints[currentPoint.X()][currentPoint.Y() - 2])
            neighbours.push_back(Point(currentPoint.X(), currentPoint.Y() - 2));
        if(currentPoint.X() + 2 < rows && !visitedPoints[currentPoint.X() + 2][currentPoint.Y()])
            neighbours.push_back(Point(currentPoint.X() + 2, currentPoint.Y()));
        if(currentPoint.Y() + 2 < columns && !visitedPoints[currentPoint.X()][currentPoint.Y() + 2])
            neighbours.push_back(Point(currentPoint.X(), currentPoint.Y() + 2));

        if(neighbours.size() == 0)
            backtracker.pop();
        else
        {
            int randNeighbour = rand() % neighbours.size();
            maze[(currentPoint.X() + neighbours[randNeighbour].X()) / 2][(currentPoint.Y() + neighbours[randNeighbour].Y()) / 2] = true;
            backtracker.push(neighbours[randNeighbour]);
        }
    }

    return maze;
}
