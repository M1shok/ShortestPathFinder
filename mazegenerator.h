#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <vector>
#include <ctime>
#include <cstdlib>
#include <stack>
#include "memoryexception.h"
#include "point.h"

class MazeGenerator
{
public:
    MazeGenerator();
    static std::vector<std::vector<bool> > generate(int rows, int columns);
};

#endif // MAZEGENERATOR_H
