#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    try
    {
        messageBox = NULL;
        result = NULL;
        ui->setupUi(this);
        scene = new MainScene();
        isRunning = false;
        isPaused = true;
        ui->graphicsView->setScene(scene);

        ui->startButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        ui->forwardButton->setEnabled(true);
        ui->skipButton->setEnabled(true);

        ui->menuBar->setEnabled(true);
        ui->resizeButton->setEnabled(true);

        ui->clearAllButton->setEnabled(true);
        ui->clearPathButton->setEnabled(true);
        ui->createMazeButton->setEnabled(true);

        ui->savePathOnlyAsPNG->setEnabled(false);
        ui->saveWithPathAsCSV->setEnabled(false);

        ui->pathLength->setText("-");
        ui->timeDuration->setText("-");
        ui->stepsQuantity->setText("-");
        loadSettings();
        setText();
        QMainWindow::showMaximized();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

MainWindow::~MainWindow()
{   
    saveSettings();

    delete result;
    delete scene;
    delete ui;
    delete messageBox;
}

void MainWindow::getLoadedRoom(std::vector<std::vector<int> > convertedRoom)
{
    try
    {
        scene->setRoom(Presentor::intToGraphicsCell(convertedRoom));
        ui->heightSpinBox->setValue(convertedRoom.size());
        ui->widthSpinBox->setValue(convertedRoom[0].size());
        sceneShrinkToFit();
        unlockScene();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::setCurrentLanguage(LanguagesDB::languages lg)
{
    try
    {
        currentLanguage = lg;
        setText();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_resizeButton_clicked()
{
    try
    {
        int h = ui->heightSpinBox->value();
        int w = ui->widthSpinBox->value();

        if(h != scene->getRoomHeight() || w != scene->getRoomWidth())
        {
            scene->resize(h, w);
            sceneShrinkToFit();
            unlockScene();
        }
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_clearPathButton_clicked()
{
    scene->clearPath();
    unlockScene();
}

void MainWindow::on_clearAllButton_clicked()
{
    scene->clearAll();
    unlockScene();
}

void MainWindow::on_createMazeButton_clicked()
{
    try
    {
        scene->clearPath();
        scene->setRoom(Presentor::boolToGraphicsCell(MazeGenerator::generate(scene->getRoomHeight(), scene->getRoomWidth())));
        unlockScene();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_openMenuButton_triggered()
{
    try
    {
        RoomTableDialog *roomTable = new RoomTableDialog(currentLanguage);
        connect(roomTable, SIGNAL(loadRoom(std::vector<std::vector<int> >)),
                this, SLOT(getLoadedRoom(std::vector<std::vector<int> >)));
        roomTable->setModal(true);
        roomTable->exec();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_saveMenuButton_triggered()
{
    try
    {
        QString name;
        SavingDialog *saveDialog = new SavingDialog(name, currentLanguage);
        saveDialog->setModal(true);
        saveDialog->exec();
        if(name == "")
            return;
        scene->hidePathLine();
        scene->hideProceccedCells();

        QPixmap screenshot = makeScreenShot();

        scene->showPathLine();
        scene->showProceccedCells();
        roomDB.addFile(name, Presentor::sceneToInt(*scene), screenshot);
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_saveAsCSV_triggered()
{
    try
    {
        LanguagesDB dictionary(currentLanguage);
        QFileDialog dialog;
        QString fileName = dialog.getSaveFileName(this, dictionary.getValueByKey("savingTitle") + " (.csv)", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "CSV files (*.csv)");
        if(fileName == "")
            return;
        CSVRoom::saveFile(fileName, Presentor::sceneToInt(*scene));
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_saveAsPNG_triggered()
{
    try
    {
        LanguagesDB dictionary(currentLanguage);
        QFileDialog dialog;
        QString fileName = dialog.getSaveFileName(this, dictionary.getValueByKey("savingTitle") + " (.png)", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "Images (*.png)");
        if(fileName == "")
            return;

        QPixmap screenshot = makeScreenShot();
        QFile file(fileName);
        file.open(QFile::WriteOnly);
        screenshot.save(&file, "PNG");
        file.close();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_savePathOnlyAsPNG_triggered()
{
    try
    {
        LanguagesDB dictionary(currentLanguage);
        QFileDialog dialog;
        QString fileName = dialog.getSaveFileName(this, dictionary.getValueByKey("savingTitle") + " (.png)", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "Images (*.png)");
        if(fileName == "")
            return;

        scene->hideProceccedCells();

        QPixmap screenshot = makeScreenShot();

        scene->showProceccedCells();

        QFile file(fileName);
        file.open(QFile::WriteOnly);
        screenshot.save(&file, "PNG");
        file.close();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_helpMenuButton_triggered()
{
    QString helpPath= QCoreApplication::applicationDirPath();
            helpPath.append("/SPFHelp.chm");
    QDesktopServices::openUrl(QUrl::fromLocalFile(helpPath));
}

void MainWindow::on_openFromFileMenuButton_triggered()
{
    try
    {
        LanguagesDB dictionary(currentLanguage);
        QFileDialog dialog;
        QString fileName = dialog.getOpenFileName(this, dictionary.getValueByKey("openingTitle") + " (.csv)", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "CSV files (*.csv)");
        if(fileName == "")
            return;

        scene->setRoom(Presentor::intToGraphicsCell(CSVRoom::openFile(fileName)));
        sceneShrinkToFit();
        unlockScene();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_settingsMenuButton_triggered()
{
    try
    {
        SettingsDialog *settings = new SettingsDialog(currentLanguage, this);
        connect(settings, SIGNAL(changeLanguage(LanguagesDB::languages)),
                this, SLOT(setCurrentLanguage(LanguagesDB::languages)));
        connect(settings,SIGNAL(sendNewColors(std::vector<QColor>)),scene,SLOT(setNewCellColors(std::vector<QColor>)));
        connect(settings,SIGNAL(sendLineParams(QColor,int)),scene,SLOT(setLineParams(QColor,int)));
        connect(settings,SIGNAL(sendStepDelay(int)),scene,SLOT(setStepDelay(int)));
        settings->setColors(scene->getCellColors());
        settings->setLineParams(scene->getLineColor(), scene->getLineWidth());
        settings->setStepDelay(scene->getStepDelay());
        settings->setModal(true);
        settings->exec();
        delete settings;
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_startButton_clicked()
{
    try
    {
        if(isPaused)
        {
            isPaused = false;
            ui->startButton->setIcon(QIcon(":/icons/pause.png"));
            ui->forwardButton->setEnabled(false);

            if(isRunning)
            {
                if(scene->showPathHistory(result->getPathFinderHistory()))
                    finishWork();
            }
            else
            {
                startWork();
                if(scene->showPathHistory(result->getPathFinderHistory()))
                    finishWork();
            }
        }
        else
        {
            isPaused = true;
            ui->startButton->setIcon(QIcon(":/icons/play.png"));
            scene->interrupt();
            ui->forwardButton->setEnabled(true);
        }
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
    catch(...)
    {
        delete messageBox;
        messageBox = new QMessageBox(QMessageBox::Critical, "Unknown Error!", "Unknown error occured!", QMessageBox::Ok);
        messageBox->show();
    }
}

void MainWindow::on_stopButton_clicked()
{
    try
    {
        scene->interrupt();
        scene->clearPath();
        finishWork();
        unlockScene();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}

void MainWindow::on_forwardButton_clicked()
{
    try
    {
        if(isRunning)
        {
            if(scene->performPathHistoryStep(result->getPathFinderHistory()))
                finishWork();
        }
        else
        {
            startWork();
            if(scene->performPathHistoryStep(result->getPathFinderHistory()))
                finishWork();
        }
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
    catch(...)
    {
        delete messageBox;
        messageBox = new QMessageBox(QMessageBox::Critical, "Unknown Error!", "Unknown error occured!", QMessageBox::Ok);
        messageBox->show();
    }
}

void MainWindow::on_skipButton_clicked()
{
    try
    {
        if(isRunning)
            scene->interrupt();
        else
            startWork();

        scene->skipPathHistory(result->getPathFinderHistory());
        finishWork();
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
    catch(...)
    {
        delete messageBox;
        messageBox = new QMessageBox(QMessageBox::Critical, "Unknown Error!", "Unknown error occured!", QMessageBox::Ok);
        messageBox->show();
    }
}

void MainWindow::startWork()
{
    ui->savePathOnlyAsPNG->setEnabled(true);
    ui->saveWithPathAsCSV->setEnabled(true);
    ui->graphicsView->viewport()->setCursor(Qt::ForbiddenCursor);
    scene->setEditing(false);
    isRunning = true;

    ui->stopButton->setEnabled(true);
    ui->skipButton->setEnabled(true);

    ui->menuBar->setEnabled(false);
    ui->resizeButton->setEnabled(false);

    ui->clearAllButton->setEnabled(false);
    ui->clearPathButton->setEnabled(false);
    ui->createMazeButton->setEnabled(false);

    scene->clearPath();

    PathFinder pf(Presentor::sceneToBool(*scene), scene->startingPoint(), scene->finishingPoint());
    result = new PathFinderResult;

    switch(ui->algorithmComboBox->currentIndex())
    {
        case 0:
            *result = pf.aStarSearchManhattan();
            break;
        case 1:
            *result = pf.aStarSearchEuclidean();
            break;
        case 2:
            *result = pf.dijkstraSearch();
            break;
        case 3:
            *result = pf.breadthFirstSearch();
            break;
        case 4:
            *result = pf.depthFirstSearchFull();
            break;
        case 5:
            *result = pf.depthFirstSearch();
            break;
    }

    scene->setFoundPath(result->getPath());

    if(result->reachedDestination())
        ui->pathLength->setText(QString::number(result->getPath().size() - 1));
    else
        ui->pathLength->setText("-");

    ui->timeDuration->setText(QString::number(result->getTimeTaken()));
    ui->stepsQuantity->setText(QString::number(result->getPathFinderHistory().size()));
}

void MainWindow::finishWork()
{
    isPaused = true;
    ui->startButton->setIcon(QIcon(":/icons/play.png"));
    isRunning = false;

    ui->stopButton->setEnabled(false);
    ui->forwardButton->setEnabled(true);
    ui->skipButton->setEnabled(true);

    ui->menuBar->setEnabled(true);
    ui->resizeButton->setEnabled(true);

    ui->clearAllButton->setEnabled(true);
    ui->clearPathButton->setEnabled(true);
    ui->createMazeButton->setEnabled(true);

    if(result->getPath().size() == 0)
    {
        delete messageBox;
        messageBox = new QMessageBox(QMessageBox::Warning, "Path not found", "Path doesn't exist!", QMessageBox::Ok);
        messageBox->show();
    }

    delete result;
    result = NULL;
}

void MainWindow::saveSettings()
{
    QJsonObject record;
    record.insert("currentLanguage", currentLanguage);
    record.insert("wallColor", scene->getColorOfCell(0).name());
    record.insert("emptyColor", scene->getColorOfCell(1).name());
    record.insert("startColor", scene->getColorOfCell(2).name());
    record.insert("finishColor", scene->getColorOfCell(3).name());
    record.insert("queuedColor", scene->getColorOfCell(4).name());
    record.insert("visitedColor", scene->getColorOfCell(5).name());
    record.insert("lineColor", scene->getLineColor().name());
    record.insert("lineWidth", scene->getLineWidth());
    record.insert("stepDelay", scene->getStepDelay());

    QJsonDocument savedSettings(record);
    QFile file("spf_settings.json");
    if(!file.open(QFile::WriteOnly | QFile::Truncate))
        throw FileException(FileException::ErrorCode::FileNotFoundError);

    file.write(savedSettings.toJson());
    file.close();
}

void MainWindow::unlockScene()
{
    ui->savePathOnlyAsPNG->setEnabled(false);
    ui->saveWithPathAsCSV->setEnabled(false);
    scene->setFoundPath(std::vector<Point>(0));
    ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
    scene->setEditing(true);
    ui->pathLength->setText("-");
    ui->timeDuration->setText("-");
    ui->stepsQuantity->setText("-");
}

void MainWindow::loadSettings()
{
    QFile file("spf_settings.json");

    connect(this, SIGNAL(sendColors(std::vector<QColor>)), scene, SLOT(setNewCellColors(std::vector<QColor>)));
    connect(this, SIGNAL(sendLineParams(QColor,int)), scene, SLOT(setLineParams(QColor,int)));
    connect(this, SIGNAL(sendStepDelay(int)), scene, SLOT(setStepDelay(int)));

    if(!file.exists())
    {
        loadDefaultSettings();
        return;
    }

    if(!file.open(QFile::ReadOnly))
        throw FileException(FileException::FileNotFoundError);

    QJsonDocument settings = QJsonDocument::fromJson(file.readAll());
    file.close();


    currentLanguage = (LanguagesDB::languages)settings.object().value("currentLanguage").toInt();
    std::vector<QColor> colors;
    colors.push_back(QColor(settings.object().value("wallColor").toString()));
    colors.push_back(QColor(settings.object().value("emptyColor").toString()));
    colors.push_back(QColor(settings.object().value("startColor").toString()));
    colors.push_back(QColor(settings.object().value("finishColor").toString()));
    colors.push_back(QColor(settings.object().value("queuedColor").toString()));
    colors.push_back(QColor(settings.object().value("visitedColor").toString()));
    emit sendColors(colors);

    QColor lineColor = QColor(settings.object().value("lineColor").toString());
    int lineWidth = settings.object().value("lineWidth").toInt();
    emit sendLineParams(lineColor, lineWidth);

    int stepDelay = settings.object().value("stepDelay").toInt();
    emit sendStepDelay(stepDelay);
}

void MainWindow::loadDefaultSettings()
{
    currentLanguage = LanguagesDB::languages::Ukrainian;
    std::vector<QColor> colors;
    colors.push_back(QColor::fromRgb(0x00, 0xCC, 0x00));
    colors.push_back(QColor(Qt::red));
    colors.push_back(QColor::fromRgb(0x68, 0x68, 0x68));
    colors.push_back(QColor(Qt::white));
    colors.push_back(QColor::fromRgb(0x7A, 0xFF, 0xA7));
    colors.push_back(QColor::fromRgb(0x00, 0x66, 0xFF));
    emit sendColors(colors);

    QColor lineColor = QColor(Qt::yellow);
    int lineWidth = 3;
    emit sendLineParams(lineColor, lineWidth);

    int stepDelay = 10;
    emit sendStepDelay(stepDelay);
}

QPixmap MainWindow::makeScreenShot()
{
    QTransform temp = ui->graphicsView->transform();
    QRectF tempSceneRect = scene->itemsBoundingRect();


    sceneShrinkToFit();
    QSizeF tempSize = ui->graphicsView->sceneRect().size();
    tempSize.setHeight(tempSize.height() * ui->graphicsView->transform().m22());
    tempSize.setWidth(tempSize.width() * ui->graphicsView->transform().m11());

    tempSize += QSizeF(2, 2);

    QPixmap screenshot = ui->graphicsView->grab(QRect(ui->graphicsView->mapFromScene(0, 0), tempSize.toSize()));

    ui->graphicsView->setTransform(temp);
    scene->setSceneRect(tempSceneRect);

    return screenshot;
}

void MainWindow::showErrorMessageBox(Exception &exception)
{
    delete messageBox;
    messageBox = new QMessageBox(QMessageBox::Critical, "Error!", exception.show(), QMessageBox::Ok);
    messageBox->show();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    sceneShrinkToFit();
    QMainWindow::resizeEvent(event);
}

void MainWindow::sceneShrinkToFit()
{
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->graphicsView->centerOn(0, 0);
    scene->setSceneRect(scene->itemsBoundingRect());
}

void MainWindow::on_quitMenuButton_triggered()
{
    close();
}

void MainWindow::on_fitSizeButton_clicked()
{
    sceneShrinkToFit();
}

void MainWindow::setText()
{
    LanguagesDB dictionary(currentLanguage);

    ui->fileMenuButton->setTitle(dictionary.getValueByKey(ui->fileMenuButton->objectName()));
    ui->openMenuButton->setText(dictionary.getValueByKey(ui->openMenuButton->objectName()));
    ui->openFromFileMenuButton->setText(dictionary.getValueByKey(ui->openFromFileMenuButton->objectName()));
    ui->saveMenuButton->setText(dictionary.getValueByKey(ui->saveMenuButton->objectName()));
    ui->saveAsMenuButton->setTitle(dictionary.getValueByKey(ui->saveAsMenuButton->objectName()));
    ui->saveAsCSV->setText(dictionary.getValueByKey(ui->saveAsCSV->objectName()));
    ui->saveAsPNG->setText(dictionary.getValueByKey(ui->saveAsPNG->objectName()));
    ui->savePathOnlyAsPNG->setText(dictionary.getValueByKey(ui->savePathOnlyAsPNG->objectName()));
    ui->saveWithPathAsCSV->setText(dictionary.getValueByKey(ui->saveWithPathAsCSV->objectName()));
    ui->editMenuButton->setTitle(dictionary.getValueByKey(ui->editMenuButton->objectName()));
    ui->settingsMenuButton->setText(dictionary.getValueByKey("settings"));
    ui->aboutMenuButton->setTitle(dictionary.getValueByKey(ui->aboutMenuButton->objectName()));
    ui->helpMenuButton->setText(dictionary.getValueByKey(ui->helpMenuButton->objectName()));
    ui->quitMenuButton->setText(dictionary.getValueByKey(ui->quitMenuButton->objectName()));
    ui->sizeFrameTitle->setText(dictionary.getValueByKey(ui->sizeFrameTitle->objectName()));
    ui->heightLabel->setText(dictionary.getValueByKey(ui->heightLabel->objectName()));
    ui->widthLabel->setText(dictionary.getValueByKey(ui->widthLabel->objectName()));
    ui->resizeButton->setText(dictionary.getValueByKey(ui->resizeButton->objectName()));
    ui->fitSizeButton->setText(dictionary.getValueByKey(ui->fitSizeButton->objectName()));
    ui->algorithmsFrameTitle->setText(dictionary.getValueByKey(ui->algorithmsFrameTitle->objectName()));
    ui->actionsFrameTitle->setText(dictionary.getValueByKey(ui->actionsFrameTitle->objectName()));
    ui->clearAllButton->setText(dictionary.getValueByKey(ui->clearAllButton->objectName()));
    ui->clearPathButton->setText(dictionary.getValueByKey(ui->clearPathButton->objectName()));
    ui->createMazeButton->setText(dictionary.getValueByKey(ui->createMazeButton->objectName()));
    ui->mediaFrameTitle->setText(dictionary.getValueByKey(ui->mediaFrameTitle->objectName()));
    ui->statsFrameTitle->setText(dictionary.getValueByKey(ui->statsFrameTitle->objectName()));
    ui->pathLengthLabel->setText(dictionary.getValueByKey(ui->pathLengthLabel->objectName()));
    ui->stepsQuantityLabel->setText(dictionary.getValueByKey(ui->stepsQuantityLabel->objectName()));
    ui->timeLabel->setText(dictionary.getValueByKey(ui->timeLabel->objectName()));
}

void MainWindow::on_saveWithPathAsCSV_triggered()
{
    try
    {
        LanguagesDB dictionary(currentLanguage);
        QFileDialog dialog;
        QString fileName = dialog.getSaveFileName(this, dictionary.getValueByKey("savingTitle") + " (.csv)", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "CSV files (*.csv)");
        if(fileName == "")
            return;
        CSVRoom::saveFile(fileName, Presentor::sceneToInt(*scene), scene->getFoundPath());
    }
    catch(Exception &exception)
    {
        showErrorMessageBox(exception);
    }
}


