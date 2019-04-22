#ifndef SAVINGDIALOG_H
#define SAVINGDIALOG_H

#include <QDialog>
#include "languagesdb.h"

namespace Ui {
class SavingDialog;
}

class SavingDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode{Add, Edit};
    explicit SavingDialog(QString &name, LanguagesDB::languages, Mode = Add, QWidget *parent = 0);
    ~SavingDialog();
    void setMode(Mode mode);
private:
    Ui::SavingDialog *ui;
    QString *newFileName;
    void setText(LanguagesDB::languages currentLanguage, Mode mode);
private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_projectNameLine_textChanged(const QString &arg1);

};
#endif // SAVINGDIALOG_H
