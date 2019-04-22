#include "roomdb.h"

RoomDB::RoomDB()
{
    loadDB();
}

void RoomDB::addFile(QString &name, const std::vector<std::vector<int>> &room, QPixmap &picture)
{
    if(!DB.isOpen())
        throw FileException(FileException::ErrorCode::DBNotOpenedError);
    if(name == "")
        throw FileException(FileException::ErrorCode::WrongNameError);

    QString roomFilename = name + "_room.csv",
            picFilename = name + "_room.png";

    if(!containSuchRec(name))
        addToDB(name, roomFilename, picFilename);
    else
    {
        QMessageBox msgBox;
        msgBox.setText("The room with such name exists.");
        msgBox.setInformativeText("Do you want to rewrite data?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::No)
            return;
    }

    CSVRoom::saveFile(roomFilename, room);

    QFile picFile(picFilename);
    picFile.open(QFile::WriteOnly);
    picture.save(&picFile, "PNG");
}

std::vector<std::vector<int>> RoomDB::loadRoom(QString &name)
{
    if(!DB.isOpen())
        throw FileException(FileException::ErrorCode::DBNotOpenedError);
    QString roomFilename = loadFromDB(name, Room);
    return CSVRoom::openFile(roomFilename);
}

QString RoomDB::getPicturePath(QString name)
{
    if(!DB.isOpen())
        throw FileException(FileException::ErrorCode::DBNotOpenedError);
    return loadFromDB(name, Picture);
}

void RoomDB::deleteFile(QString &name)
{
    if(!DB.isOpen())
        throw FileException(FileException::ErrorCode::DBNotOpenedError);
    if(name == "")
        throw FileException(FileException::ErrorCode::WrongNameError);

    if(!containSuchRec(name))
        throw FileException(FileException::ErrorCode::RecordNotFoundInDBError);

    QFile::remove(loadFromDB(name, Room));
    QFile::remove(loadFromDB(name, Picture));
    deleteFromDB(name);
}

void RoomDB::updateName(QString &oldName, QString &newName)
{
    if(!DB.isOpen())
        throw FileException(FileException::ErrorCode::DBNotOpenedError);
    if(newName == "")
        throw FileException(FileException::ErrorCode::WrongNameError);

    if(containSuchRec(newName))
        throw FileException(FileException::ErrorCode::DBContainsSuchNameError);

    QString oldFileName = loadFromDB(oldName, Room);
    QString newFileName = newName + "_room.csv";

    QFile::rename(oldFileName, newFileName);

    oldFileName = loadFromDB(oldName, Picture);
    newFileName = newName + "_room.png";

    QFile::rename(oldFileName, newFileName);
    updateInDB(oldName, newName);
}

void RoomDB::loadDB()
{
    try
    {
        if(!QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        {
            DB = QSqlDatabase::addDatabase("QSQLITE");
            QString name = QCoreApplication::applicationDirPath() + "/roomdatabase.db";
            DB.setDatabaseName(name);
        }
        else
            DB = QSqlDatabase::database();

        DB.open();
        if(!DB.isOpen())
            throw FileException(FileException::ErrorCode::DBNotOpenedError);
        createTable();
    }
    catch(Exception &exception)
    {
        QMessageBox messageBox(QMessageBox::Critical, "Error!", exception.show(), QMessageBox::Ok);
        messageBox.exec();
    }
}

void RoomDB::createTable()
{
    QSqlQuery query(QSqlDatabase::database());
    query.exec("CREATE TABLE IF NOT EXISTS roomstable("
               "name VARCHAR(255) NOT NULL UNIQUE,"
               "roomfile VARCHAR(255) NOT NULL UNIQUE,"
               "picfile VARCHAR(255) NOT NULL UNIQUE)");

    if(query.lastError().type() != QSqlError::NoError)
        throw FileException(FileException::ErrorCode::BadQueryResultError);

    query.finish();
}

QList<QString> RoomDB::loadAll()
{
    if(!DB.isOpen())
        throw FileException(FileException::ErrorCode::DBNotOpenedError);
    QSqlQuery query(QSqlDatabase::database());
    query.exec("SELECT name FROM roomstable");
    QList<QString> rooms;
    while(query.next())
    {
        QString name = query.value(0).toString();
        rooms.append(name);
    }

    if(query.lastError().type() != QSqlError::NoError)
        throw FileException(FileException::ErrorCode::BadQueryResultError);

    query.finish();
    return rooms;
}

bool RoomDB::containSuchRec(QString &name)
{
    if(name == "")
        throw FileException(FileException::ErrorCode::WrongNameError);

    bool found = false;

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT name FROM roomstable WHERE name = :name");
    query.bindValue(":name", name);
    query.exec();

    if(query.lastError().type() != QSqlError::NoError)
        throw FileException(FileException::ErrorCode::BadQueryResultError);

    if(query.next())
        found = true;
    else
        found = false;

    query.finish();
    return found;
}

void RoomDB::addToDB(QString &name, QString &roomFileName, QString &picFileName)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("INSERT INTO roomstable VALUES (:name, :room, :picture)");
    query.bindValue(":name", name);
    query.bindValue(":room", roomFileName);
    query.bindValue(":picture", picFileName);
    query.exec();

    if(query.lastError().type() != QSqlError::NoError)
        throw FileException(FileException::ErrorCode::BadQueryResultError);

    query.finish();
}

QString RoomDB::loadFromDB(QString &name, LoadType loadType)
{
    QString filename;

    QSqlQuery query(QSqlDatabase::database());
    query.exec("SELECT * FROM roomstable");
    while(query.next())
    {
        if(query.value(0).toString() == name)
        {
            filename = query.value(loadType).toString();
            break;
        }
    }

    if(query.lastError().type() != QSqlError::NoError)
        throw FileException(FileException::ErrorCode::BadQueryResultError);

    query.finish();
    return filename;
}

void RoomDB::deleteFromDB(QString &name)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("DELETE FROM roomstable WHERE name = ?");
    query.addBindValue(name);
    query.exec();

    if(query.lastError().type() != QSqlError::NoError)
        throw FileException(FileException::ErrorCode::BadQueryResultError);

    query.finish();
}

void RoomDB::updateInDB(QString &oldName, QString &newName)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("UPDATE roomstable SET name = :newName, roomfile = :newRoomFile, picFile = :newPicFile WHERE name = :oldName");
    query.bindValue(":oldName", oldName);
    query.bindValue(":newName", newName);
    query.bindValue(":newRoomFile", newName + "_room.csv");
    query.bindValue(":newPicFile", newName + "_room.png");
    query.exec();

    if(query.lastError().type() != QSqlError::NoError)
        throw FileException(FileException::ErrorCode::BadQueryResultError);

    query.finish();
}
