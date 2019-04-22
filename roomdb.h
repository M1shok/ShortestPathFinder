#ifndef ROOMDB_H
#define ROOMDB_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QCoreApplication>
#include <QPixmap>
#include <QMessageBox>
#include <list>
#include "csvroom.h"

class RoomDB
{
public:
    RoomDB();
    void addFile(QString &name, const std::vector<std::vector<int>> &room, QPixmap &picture);
    std::vector<std::vector<int>> loadRoom(QString &name);
    QString getPicturePath(QString name);
    void deleteFile(QString &name);
    void updateName(QString &oldName, QString &newName);
    QList<QString> loadAll();

private:
    QSqlDatabase DB;
    enum LoadType{Room = 1, Picture = 2};

    void loadDB();
    void createTable();
    void addToDB(QString &name, QString &roomFilename, QString &picFilename);
    QString loadFromDB(QString &name, LoadType loadType);
    void deleteFromDB(QString &name);
    void updateInDB(QString &oldName, QString &newName);
    bool containSuchRec(QString &name);
};

#endif // ROOMDB_H
