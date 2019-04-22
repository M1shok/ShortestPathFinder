#include "roomtabledialog.h"
#include "ui_roomtabledialog.h"

RoomTableDialog::RoomTableDialog(LanguagesDB::languages lg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomTableDialog)
{
    ui->setupUi(this);
    currentLanguage = lg;
    setText();
    setUpTable();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    messageBox = NULL;
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->loadButton->setEnabled(false);
    ui->previewButton->setEnabled(false);
}

RoomTableDialog::~RoomTableDialog()
{
    delete messageBox;
    delete ui;
}

void RoomTableDialog::on_editButton_clicked()
{
    try
    {
        QString oldName, newName;
        oldName = ui->table->selectedItems().at(0)->text();
        SavingDialog dialog(newName, currentLanguage, SavingDialog::Edit);
        dialog.setWindowIcon(QIcon(":/icons/editIcon.png"));
        dialog.setModal(true);
        dialog.exec();

        roomDB.updateName(oldName, newName);
        ui->table->selectedItems().at(0)->setText(newName);
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void RoomTableDialog::addToTable(QString &name, int rowIndex)
{
    try
    {
        ui->table->insertRow(rowIndex);
        QTableWidgetItem *item = new QTableWidgetItem(name);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->table->setItem(rowIndex, 0, item);
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void RoomTableDialog::setUpTable()
{
    try
    {
        QList<QString> rooms = roomDB.loadAll();
        for(int i = 0; i < rooms.size(); i++)
            addToTable(rooms[i], i);
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void RoomTableDialog::on_loadButton_clicked()
{
    try
    {
        QString name = ui->table->selectedItems().at(0)->text();

        std::vector<std::vector<int> > room = roomDB.loadRoom(name);
        emit loadRoom(room);
        this->close();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void RoomTableDialog::on_deleteButton_clicked()
{
    try
    {
        QTableWidgetItem *item = ui->table->selectedItems().at(0);

        QString name = item->text();
        roomDB.deleteFile(name);
        ui->table->removeRow(item->row());
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void RoomTableDialog::on_cancelButton_clicked()
{
    close();
}

void RoomTableDialog::on_table_itemSelectionChanged()
{
    try
    {
        if(ui->table->selectedItems().size() == 0)
        {
            ui->editButton->setEnabled(false);
            ui->deleteButton->setEnabled(false);
            ui->loadButton->setEnabled(false);
            ui->previewButton->setEnabled(false);
        }
        else
        {
            ui->editButton->setEnabled(true);
            ui->deleteButton->setEnabled(true);
            ui->loadButton->setEnabled(true);
            ui->previewButton->setEnabled(true);
        }
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void RoomTableDialog::setText()
{
    try
    {
        LanguagesDB dictionary(currentLanguage);

        this->setWindowTitle(dictionary.getValueByKey("openingTitle"));
        ui->availableRoomsLabel->setText(dictionary.getValueByKey(ui->availableRoomsLabel->objectName()));
        ui->editButton->setText(dictionary.getValueByKey(ui->editButton->objectName()));
        ui->loadButton->setText(dictionary.getValueByKey(ui->loadButton->objectName()));
        ui->previewButton->setText(dictionary.getValueByKey(ui->previewButton->objectName()));
        ui->cancelButton->setText(dictionary.getValueByKey(ui->cancelButton->objectName()));
        ui->deleteButton->setText(dictionary.getValueByKey(ui->deleteButton->objectName()));
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void RoomTableDialog::showErrorMessageBox(Exception &exception)
{
    delete messageBox;
    messageBox = new QMessageBox(QMessageBox::Critical, "Error!", exception.show(), QMessageBox::Ok);
    messageBox->show();
}

void RoomTableDialog::on_previewButton_clicked()
{
    try
    {
        QString picFileName = roomDB.getPicturePath(ui->table->currentItem()->text());
        QUrl fileUrl = QUrl::fromLocalFile(picFileName);
        if(!QDesktopServices::openUrl(fileUrl))
            throw FileException(FileException::ErrorCode::FileNotFoundError);
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}
