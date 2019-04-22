#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(LanguagesDB::languages lg,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(ui->wallColorButton, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(ui->emptyColorButton, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(ui->startColorButton, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(ui->finishColorButton, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(ui->visitedColorButton, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(ui->queuedColorButton, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(ui->lineColorButton, SIGNAL(clicked()), this, SLOT(colorChanged()));

    currentLanguage = lg;
    setRadioButtons();
    setText();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setColors(std::vector<QColor> col)
{
    colors = col;
    setButtonColor(ui->startColorButton, colors.at(0));
    setButtonColor(ui->finishColorButton, colors.at(1));
    setButtonColor(ui->wallColorButton, colors.at(2));
    setButtonColor(ui->emptyColorButton, colors.at(3));
    setButtonColor(ui->visitedColorButton, colors.at(4));
    setButtonColor(ui->queuedColorButton, colors.at(5));
}

void SettingsDialog::setLineParams(QColor c, int w)
{
    lineColor = c;
    lineWidth = w;
    setButtonColor(ui->lineColorButton, lineColor);
    ui->lineWidthSlider->setValue(lineWidth);
    ui->lineWidthSpinBox->setValue(lineWidth);
}

void SettingsDialog::setStepDelay(int value)
{
    stepDelay = value;
    ui->stepDelaySlider->setValue(stepDelay);
    ui->stepDelaySpinBox->setValue(stepDelay);
}

void SettingsDialog::colorChanged()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QColor color = QColorDialog::getColor(button->palette().button().color(), this, "Color Picker");
    if(color.isValid())
        setButtonColor(button, color);
}

void SettingsDialog::setButtonColor(QPushButton* b, QColor c)
{
    QString qss = QString("background-color: %1").arg(c.name()) + "; border: 0px;";
    b->setStyleSheet(qss);

}

void SettingsDialog::setRadioButtons()
{
    switch (currentLanguage)
    {
    case LanguagesDB::English:
        ui->englishRadioButton->setChecked(true);
        break;
    case LanguagesDB::Ukrainian:
        ui->ukrainianRadioButton->setChecked(true);
        break;
    case LanguagesDB::Russian:
        ui->russianRadioButton->setChecked(true);
        break;
    }
}

void SettingsDialog::on_englishRadioButton_clicked()
{
    currentLanguage = LanguagesDB::English;
}

void SettingsDialog::on_ukrainianRadioButton_clicked()
{
    currentLanguage = LanguagesDB::Ukrainian;
}

void SettingsDialog::on_russianRadioButton_clicked()
{
    currentLanguage = LanguagesDB::Russian;
}

void SettingsDialog::setText()
{
    LanguagesDB dictionary(currentLanguage);

    this->setWindowTitle(dictionary.getValueByKey("settings"));
    ui->colorGroupBox->setTitle(dictionary.getValueByKey(ui->colorGroupBox->objectName()));
    ui->wallElementLabel->setText(dictionary.getValueByKey(ui->wallElementLabel->objectName()));
    ui->emptyElementLabel->setText(dictionary.getValueByKey(ui->emptyElementLabel->objectName()));
    ui->startElementLabel->setText(dictionary.getValueByKey(ui->startElementLabel->objectName()));
    ui->finishElementLabel->setText(dictionary.getValueByKey(ui->finishElementLabel->objectName()));
    ui->queuedElementLabel->setText(dictionary.getValueByKey(ui->queuedElementLabel->objectName()));
    ui->visitedElementLabel->setText(dictionary.getValueByKey(ui->visitedElementLabel->objectName()));
    ui->pathElementLabel->setText(dictionary.getValueByKey(ui->pathElementLabel->objectName()));
    ui->otherGroupBox->setTitle(dictionary.getValueByKey(ui->otherGroupBox->objectName()));
    ui->lineWidthLabel->setText(dictionary.getValueByKey(ui->lineWidthLabel->objectName()));
    ui->stepDelayLabel->setText(dictionary.getValueByKey(ui->stepDelayLabel->objectName()));
    ui->languageGroupBox->setTitle(dictionary.getValueByKey(ui->languageGroupBox->objectName()));
    ui->okButton->setText(dictionary.getValueByKey("okButton"));
    ui->cancelButton->setText(dictionary.getValueByKey("cancelButton"));
    ui->applyButton->setText(dictionary.getValueByKey(ui->applyButton->objectName()));
}

void SettingsDialog::on_okButton_clicked()
{
    colors.at(0) = ui->startColorButton->palette().button().color();
    colors.at(1) = ui->finishColorButton->palette().button().color();
    colors.at(2) = ui->wallColorButton->palette().button().color();
    colors.at(3) = ui->emptyColorButton->palette().button().color();
    colors.at(4) = ui->visitedColorButton->palette().button().color();
    colors.at(5) = ui->queuedColorButton->palette().button().color();
    lineColor = ui->lineColorButton->palette().button().color();
    lineWidth = ui->lineWidthSlider->value();
    stepDelay = ui->stepDelaySlider->value();
    setText();
    emit sendNewColors(colors);
    emit sendLineParams(lineColor, lineWidth);
    emit sendStepDelay(stepDelay);
    emit changeLanguage(currentLanguage);
    close();
}

void SettingsDialog::on_applyButton_clicked()
{
    colors.at(0) = ui->startColorButton->palette().button().color();
    colors.at(1) = ui->finishColorButton->palette().button().color();
    colors.at(2) = ui->wallColorButton->palette().button().color();
    colors.at(3) = ui->emptyColorButton->palette().button().color();
    colors.at(4) = ui->visitedColorButton->palette().button().color();
    colors.at(5) = ui->queuedColorButton->palette().button().color();
    lineColor = ui->lineColorButton->palette().button().color();
    lineWidth = ui->lineWidthSlider->value();
    stepDelay = ui->stepDelaySlider->value();
    setText();
    emit sendNewColors(colors);
    emit sendLineParams(lineColor, lineWidth);
    emit sendStepDelay(stepDelay);
    emit changeLanguage(currentLanguage);
}

void SettingsDialog::on_cancelButton_clicked()
{
    close();
}
