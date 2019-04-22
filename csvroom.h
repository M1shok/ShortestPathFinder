#ifndef ROOM_H
#include <QFile>
#include <QTextStream>
#include <vector>
#include "fileexception.h"
#include "memoryexception.h"
#include "point.h"
#define ROOM_H

class CSVRoom
{
public:
    CSVRoom();
    static std::vector<std::vector<int>> openFile(const QString &filename);
    static void saveFile(const QString &filename, const std::vector<std::vector<int>> &room);
    static void saveFile(const QString &filename, std::vector<std::vector<int> > room, const std::vector<Point> &path);
};

#endif // ROOM_H
