#include "csvroom.h"

CSVRoom::CSVRoom() {}

std::vector<std::vector<int>> CSVRoom::openFile(const QString &filename)
{
    if(filename == "")
        throw FileException(FileException::ErrorCode::WrongNameError);

    QFile file(filename);
    if(!file.exists() || !file.open(QFile::ReadOnly))
        throw FileException(FileException::ErrorCode::FileNotFoundError);

    QString temp;
    QStringList wordList;
    QTextStream in(&file);
    std::vector<std::vector<int> > resultRoom;
    int index = 0, previousSize = 0;

    while(!in.atEnd())
    {
        in >> temp;
        if(temp == "")
            break;

        resultRoom.push_back(std::vector<int>());
        wordList = temp.split(';', QString::SkipEmptyParts);

        if(previousSize != 0 && previousSize != wordList.size())
            throw FileException(FileException::ErrorCode::BadDataError);
        else
            previousSize = wordList.size();

        for(int j = 0; j < wordList.size(); j++)
        {
            if(wordList[j].size() != 1 || !wordList[j][0].isDigit() || wordList[j].toInt() < 0 || (wordList[j].toInt() > 3 && wordList[j].toInt() != 9))
                throw FileException(FileException::ErrorCode::BadDataError);

            if(wordList[j].toInt() == 9)
                resultRoom[index].push_back(3);
            else
                resultRoom[index].push_back(wordList[j].toInt());
        }

        index++;
    }

    file.close();
    return resultRoom;
}

void CSVRoom::saveFile(const QString &filename, const std::vector<std::vector<int>> &room)
{
    if(filename == "")
        throw FileException(FileException::ErrorCode::WrongNameError);

    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
        throw FileException(FileException::ErrorCode::FileNotFoundError);

    QTextStream out(&file);

    int rows = room.size(), columns = room[0].size();

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            out << room[i][j] << ';';
        }
        out << '\n';
    }

    file.close();
}

void CSVRoom::saveFile(const QString &filename, std::vector<std::vector<int> > room, const std::vector<Point> &path)
{
    if(filename == "")
        throw FileException(FileException::ErrorCode::WrongNameError);

    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
        throw FileException(FileException::ErrorCode::FileNotFoundError);

    QTextStream out(&file);

    int rows = room.size();
    if(rows == 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);
    int columns = room[0].size();
    if(columns == 0)
        throw MemoryException(MemoryException::ErrorCode::ForbiddenSizeError);

    for(unsigned i = 1; i < path.size() - 1; i++)
    {
        if(path[i].X() < 0 || path[i].X() >= rows || path[i].Y() < 0 || path[i].Y() >= columns)
            throw MemoryException(MemoryException::ErrorCode::OutOfRangeIndexError);

        room[path[i].X()][path[i].Y()] = 9;
    }

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
            out << room[i][j] << ';';

        out << '\n';
    }

    file.close();
}
