#include "roomtable.h"
#include "ui_roomtable.h"
#include "savingdialog.h"
#include "presentor.h"

RoomTable::RoomTable(LanguagesDB::languages lg, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomTable)
{
    ui->setupUi(this);

    currentLanguage = lg;
    setText();
    setUpTable();
}

RoomTable::~RoomTable()
{
    delete ui;
}

void RoomTable::on_editButton_clicked()
{
    QString oldName, newName;
    if(ui->table->selectedItems().size() > 0)
        oldName = ui->table->selectedItems().at(0)->text();
    else
    {
        // ошибочка
        return;
    }


    SavingDialog dialog(newName, currentLanguage, SavingDialog::Edit);
    dialog.setModal(true);
    dialog.exec();

    bool result = roomDB.updateName(oldName, newName);

    if(result == true)
        ui->table->selectedItems().at(0)->setText(newName);
}

void RoomTable::addToTable(QString &name, int rowIndex)
{
    ui->table->insertRow(rowIndex);
    QTableWidgetItem *item = new QTableWidgetItem(name);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    ui->table->setItem(rowIndex, 0, item);
}

void RoomTable::setUpTable()
{
    QList<QString> rooms = roomDB.loadAll();
    for(int i = 0; i < rooms.size(); i++)
        addToTable(rooms[i], i);
}

void RoomTable::setText()
{
    LanguagesDB dictionary(currentLanguage);

    ui->availableRoomsLabel->setText(dictionary.getValueByKey(ui->availableRoomsLabel->objectName()));
    ui->editButton->setText(dictionary.getValueByKey(ui->editButton->objectName()));
    ui->loadButton->setText(dictionary.getValueByKey(ui->loadButton->objectName()));
    ui->deleteButton->setText(dictionary.getValueByKey(ui->deleteButton->objectName()));
}

void RoomTable::on_loadButton_clicked()
{
    QString name;
    if(ui->table->selectedItems().size() > 0)
        name = ui->table->selectedItems().at(0)->text();
    else
    {
        // ошибочка
        return;
    }

    std::vector<std::vector<int>> room = roomDB.loadRoom(name);
    emit loadRoom(room);
    this->close();
}

void RoomTable::on_deleteButton_clicked()
{
    QTableWidgetItem *item;

    if(ui->table->selectedItems().count() > 0)
        item = ui->table->selectedItems().at(0);
    else
    {
        // ошибочка
        return;
    }

    QString name = item->text();
    roomDB.deleteFile(name);
    ui->table->removeRow(item->row());
}
