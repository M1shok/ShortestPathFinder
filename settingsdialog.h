#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "languagesdb.h"
#include <QColorDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(LanguagesDB::languages currentLanguage = LanguagesDB::English, QWidget *parent = 0);
    ~SettingsDialog();
    void setColors(std::vector<QColor> col);
    void setLineParams(QColor c, int w);
    void setStepDelay(int value);
private:
    void setButtonColor(QPushButton* b, QColor c);
    void setText();
    void setRadioButtons();
    Ui::SettingsDialog *ui;
    std::vector<QColor> colors;
    QColor lineColor;
    int lineWidth;
    int stepDelay;
    LanguagesDB::languages currentLanguage;
private slots:
    void colorChanged();
    void on_englishRadioButton_clicked();
    void on_ukrainianRadioButton_clicked();
    void on_russianRadioButton_clicked();
    void on_applyButton_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();
signals:
    void sendNewColors(std::vector<QColor>);
    void sendLineParams(QColor, int);
    void sendStepDelay(int);
    void changeLanguage(LanguagesDB::languages);
};

#endif // SETTINGSDIALOG_H
