#ifndef ROOMTABLE_H
#define ROOMTABLE_H

#include <QWidget>
#include "roomdb.h"
#include "languagesdb.h"

namespace Ui {
class RoomTable;
}

class RoomTable : public QWidget
{
    Q_OBJECT

public:
    explicit RoomTable(LanguagesDB::languages lg, QWidget *parent = 0);
    ~RoomTable();

private slots:
    void on_editButton_clicked();

    void on_loadButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::RoomTable *ui;
    RoomDB roomDB;
    LanguagesDB::languages currentLanguage;

    void addToTable(QString &name, int rowIndex);
    void setUpTable();
    void setText();

signals:
    void loadRoom(std::vector<std::vector<int> > convertedRoom);
};

#endif // ROOMTABLE_H
