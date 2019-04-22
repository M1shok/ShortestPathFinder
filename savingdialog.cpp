#include "savingdialog.h"
#include "ui_savingdialog.h"

SavingDialog::SavingDialog(QString &name, LanguagesDB::languages currentLanguage, Mode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SavingDialog)
{
    ui->setupUi(this);

    setText(currentLanguage, mode);

    newFileName = &name;
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->okButton->setEnabled(false);
}

SavingDialog::~SavingDialog()
{
    delete ui;
}

void SavingDialog::on_okButton_clicked()
{
    *newFileName = ui->projectNameLine->text();
    this->close();
}


void SavingDialog::on_cancelButton_clicked()
{
    this->close();
}

void SavingDialog::setText(LanguagesDB::languages currentLanguage, SavingDialog::Mode mode)
{
    LanguagesDB dictionary(currentLanguage);

    switch(mode)
    {
    case Add:
        this->setWindowTitle(dictionary.getValueByKey("savingTitle"));
        break;
    case Edit:
        this->setWindowTitle(dictionary.getValueByKey("editingTitle"));
        break;
    }

    ui->projectNameLabel->setText(dictionary.getValueByKey(ui->projectNameLabel->objectName()));
    ui->okButton->setText(dictionary.getValueByKey(ui->okButton->objectName()));
    ui->cancelButton->setText(dictionary.getValueByKey(ui->cancelButton->objectName()));
}

void SavingDialog::on_projectNameLine_textChanged(const QString &arg1)
{
    if(arg1 != "")
        ui->okButton->setEnabled(true);
    else
        ui->okButton->setEnabled(false);
}
