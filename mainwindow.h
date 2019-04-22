#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainscene.h"
#include "graphicscell.h"
#include "languagesdb.h"
#include "roomdb.h"
#include "presentor.h"
#include "pathfinder.h"
#include "roomtabledialog.h"
#include "savingdialog.h"
#include "settingsdialog.h"
#include "fileexception.h"
#include "mazegenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    LanguagesDB::languages currentLanguage;
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    void startWork();
    void finishWork();
    void saveSettings();
    void unlockScene();
    void loadSettings();
    void loadDefaultSettings();
    void setText();
    void sceneShrinkToFit();
    QPixmap makeScreenShot();
    void showErrorMessageBox(Exception &exception);
    Ui::MainWindow *ui;
    MainScene *scene;
    PathFinderResult *result;
    QMessageBox *messageBox;
    bool isRunning;
    bool isPaused;
    RoomDB roomDB;
public slots:
    void getLoadedRoom(std::vector<std::vector<int> > convertedRoom);
    void setCurrentLanguage(LanguagesDB::languages lg);
private slots:
    void on_startButton_clicked();
    void on_resizeButton_clicked();
    void on_clearPathButton_clicked();
    void on_clearAllButton_clicked();
    void on_stopButton_clicked();
    void on_createMazeButton_clicked();
    void on_openMenuButton_triggered();
    void on_saveMenuButton_triggered();
    void on_settingsMenuButton_triggered();
    void on_forwardButton_clicked();
    void on_skipButton_clicked();
    void on_quitMenuButton_triggered();
    void on_fitSizeButton_clicked();
    void on_saveAsCSV_triggered();
    void on_saveAsPNG_triggered();
    void on_openFromFileMenuButton_triggered();
    void on_savePathOnlyAsPNG_triggered();
    void on_saveWithPathAsCSV_triggered();
    void on_helpMenuButton_triggered();
signals:
    void sendColors(std::vector <QColor>);
    void sendLineParams(QColor, int);
    void sendStepDelay(int);
};

#endif // MAINWINDOW_H
