#ifndef PRESENTOR_H
#define PRESENTOR_H

#include "mainscene.h"
#include <vector>
#include "memoryexception.h"
#include "algorythmexception.h"

class Presentor
{
public:
    Presentor();
    static std::vector<std::vector<GraphicsCell> > intToGraphicsCell(const std::vector<std::vector<int> > &convertedRoom);
    static std::vector<std::vector<GraphicsCell> > boolToGraphicsCell(const std::vector<std::vector<bool> > &convertedRoom);
    static std::vector<std::vector<bool> > sceneToBool(const MainScene &scene);
    static std::vector<std::vector<int> > sceneToInt(const MainScene &scene);
};

#endif // PRESENTOR_H
