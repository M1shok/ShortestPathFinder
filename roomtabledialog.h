#ifndef ROOMTABLEDIALOG_H
#define ROOMTABLEDIALOG_H

#include <QTableWidgetItem>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include "roomdb.h"
#include "savingdialog.h"
#include "exception.h"

namespace Ui {
class RoomTableDialog;
}

class RoomTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomTableDialog(LanguagesDB::languages lg, QWidget *parent = 0);
    ~RoomTableDialog();
private:
    Ui::RoomTableDialog *ui;
    RoomDB roomDB;
    LanguagesDB::languages currentLanguage;
    QMessageBox *messageBox;
    void addToTable(QString &name, int rowIndex);
    void setUpTable();
    void setText();
    void showErrorMessageBox(Exception &exception);
private slots:
    void on_editButton_clicked();
    void on_loadButton_clicked();
    void on_deleteButton_clicked();
    void on_cancelButton_clicked();
    void on_table_itemSelectionChanged();
    void on_previewButton_clicked();
signals:
    void loadRoom(std::vector<std::vector<int> > convertedRoom);
};

#endif // ROOMTABLEDIALOG_H
