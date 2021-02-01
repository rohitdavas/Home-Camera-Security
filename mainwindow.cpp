#include "mainwindow.h"
#include <QMenuBar>
#include <QApplication>
#include <QMessageBox>
#include <QCameraInfo>
#include <QScrollBar>
#include <QList>
#include <QPushButton>
#include <QGridLayout>
#include <string>
#include <QShortcut>
#include "capture_thread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), fileMenu(nullptr), capturer(nullptr)
{
    initUI();
    toggleHideActions(false);
    data_lock = new QMutex();
}

MainWindow::~MainWindow(){

}

void MainWindow::initUI(){
    this->resize(640, 800);

    initUIFileMenu();
    initUICameraMenu();
    initUIStatusBar();
    initUIViewArea();
}

void MainWindow::initUIFileMenu()
{
    // add menu
    fileMenu = menuBar()->addMenu("File");
//    fileToolBar = addToolBar("File");

    // add action exit
    exitAction = new QAction("Exit", this);
    fileMenu->addAction(exitAction);
    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    exitAction->setShortcut(QKeySequence("Ctrl+F4"));
}

void MainWindow::initUICameraMenu()
{
    /*
     * TODO
     * 1.Add actions
     * 2.Add Shortcuts for each
     * 3.Check visibilty : default ON
     * 4.connect to a proper slot
     */
    // add menu and toolbar

    cameraMenu = menuBar()->addMenu("Camera(s)");
    cameraToolBar = addToolBar("Camera(s)");

    // add cameraInfoAction
    cameraInfoAction = new QAction("Camera(s) Info", this);
    cameraMenu->addAction(cameraInfoAction);
    connect(cameraInfoAction, SIGNAL(triggered(bool)), this,SLOT(cameraInfo()));
    cameraInfoAction->setShortcut(QKeySequence("Alt+I"));

    // add cameraOpenAction
    cameraOpenAction = new QAction("Open Camera", this);
    cameraMenu->addAction(cameraOpenAction);
    cameraToolBar->addAction(cameraOpenAction);
    connect(cameraOpenAction, SIGNAL(triggered(bool)), this, SLOT(cameraOpen()));
    cameraOpenAction->setShortcut(QKeySequence("Alt+O"));

    // add stop camera action
    // set visibility off initially
    stopCameraAction = new QAction("Stop", this);
    cameraMenu->addAction(stopCameraAction);
    cameraToolBar->addAction(stopCameraAction);
    connect(stopCameraAction, SIGNAL(triggered(bool)), this, SLOT(stopCamera()));
    stopCameraAction->setShortcut(QKeySequence("Alt+S"));

    // add FPS calculation action
    // set visibility off initially
    fpsCalculationAction = new QAction("FPS", this);
    cameraMenu->addAction(fpsCalculationAction);
    cameraToolBar->addAction(fpsCalculationAction);
    connect(fpsCalculationAction, SIGNAL(triggered(bool)), this, SLOT(calculateFPS()));
    fpsCalculationAction->setShortcut(QKeySequence("Alt+F"));

    // add mirror checkbox
    // set visibility off initially
   cameraMirrorAction = new QAction("Mirror", this);
   cameraMenu->addAction(cameraMirrorAction);
   cameraToolBar->addAction(cameraMirrorAction);
   connect(cameraMirrorAction, SIGNAL(triggered(bool)), this, SLOT(doCameraMirror()));
   cameraMirrorAction->setShortcut(QKeySequence("Alt+M"));

}

void MainWindow::initUIViewArea()
{
    // grid layout system to divide up the screen.
    QGridLayout *main_layout = new QGridLayout();
    // graphics view
    imageScene1 = new QGraphicsScene(0,0,12,1,this);
    imageView1 = new QGraphicsView(imageScene1);
    imageView1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    imageScene2 = new QGraphicsScene(0,2,12,1,this);
    imageView2 = new QGraphicsView(imageScene2);
    imageView2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    main_layout->addWidget(imageView1, 0, 0, 12, 1);
    main_layout->addWidget(imageView2, 0 ,1, 12, 1);

    // center layout.
    QGridLayout *tools_layout = new QGridLayout();
    main_layout->addLayout(tools_layout, 12, 0, 1, 2);

//    // monitor box
//    monitorCheckBox = new QCheckBox(this);
//    monitorCheckBox->setText("Monitor on/off");
//    tools_layout->addWidget(monitorCheckBox, 0, 0);

    //record button
    recordButtonText = new QList<QString>({"Record", "Stop Record"});
    recordButton = new QPushButton(recordButtonText->at(0), this);
    tools_layout->addWidget(new QLabel(this), 0, 0);
    tools_layout->addWidget(recordButton, 0, 1, Qt::AlignHCenter);
    tools_layout->addWidget(new QLabel(this), 0, 2);
    connect(recordButton, SIGNAL(clicked(bool)), this, SLOT(recordingStartStop()));

//    // list view bottom
//    saved_list = new QListView(this);
//    main_layout->addWidget(saved_list, 13, 0, 4, 1);

    QWidget *widget = new QWidget();
    widget->setLayout(main_layout);
    setCentralWidget(widget);
}

void MainWindow::initUIStatusBar()
{
    mainStatusBar = statusBar();

    mainStatusBarData = new QMap<QString, QString>();
    mainStatusBarData->insert("default", "Ready");
    mainStatusBarData->insert("Camera Name", "");
    mainStatusBarData->insert("Resolution", "");
    mainStatusBarData->insert("FPS", "");

    mainStatusLabel = new QLabel(mainStatusBarData->value("default"));
    mainStatusBar->addWidget(mainStatusLabel);
    updateStatusBar(true);
}

void MainWindow::updateStatusBar(QString key, QString value, bool includeDefault=false){
    mainStatusBarData->insert(key, value);
    QString label = "";
    foreach(QString key, mainStatusBarData->keys()){
        if(key =="default" && !includeDefault)
            continue;

        if (mainStatusBarData->value(key) != "")
        {
            label += mainStatusBarData->value(key) + " | ";
        }
    }
    mainStatusLabel->setText(label);
}

void MainWindow::updateStatusBar(bool includeDefault){

    QString label = "";
    foreach(QString key, mainStatusBarData->keys()){
        if(key =="default" && !includeDefault)
            continue;

        if (mainStatusBarData->value(key) != "")
        {
            label += mainStatusBarData->value(key) + " | ";
        }
    }
    mainStatusLabel->setText(label);
}


void MainWindow::cameraInfo(){
    QMessageBox msgBox;
//    QPushButton *button = new QPushButton("new button", this);

    msgBox.setText("Camera(s) Info");
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    QString info = QString("Available Cameras (%1):\n").arg(cameras.length());
    QString detailed_info;
    if (cameras.empty()){
        QMessageBox::information(this, "Information", "No Camera available.");
        return;
    }

    foreach(QCameraInfo c_info, cameras)
    {
        info += "    - " + c_info.deviceName() + "\n";
        detailed_info += " - " + c_info.description() + "\n";
    }

    msgBox.setInformativeText(info);
    msgBox.setDetailedText(detailed_info);
//    msgBox.addButton(button, QMessageBox::RejectRole);
    msgBox.exec();
}

QString MainWindow::selectCamera(){

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    // no camera available
    if (cameras.empty()){
        QMessageBox::information(this, "Information", "No Camera available.");
        return nullptr;
    }

   // single camera available
   if (cameras.length()==1){
       return cameras.at(0).deviceName();
   }

   // multi cameras available - let user select camera.
    QMessageBox msgBox;
    foreach(QCameraInfo camera, cameras){
        msgBox.addButton(camera.deviceName(), QMessageBox::ActionRole);
    }

    QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
    msgBox.exec();

    if(msgBox.clickedButton()==cancelButton){
        return nullptr;
    }
    else{
        return msgBox.clickedButton()->text();
    }
}

void MainWindow::cameraOpen(){

        if (isCameraOpen){
            QMessageBox::information(this, "Information", "Close the opened camera first");
            return;
        }

        QString camname = selectCamera();
        if (camname == nullptr){
           return;
        }

        // get the camera name and convert to std string
        std::string std_camname = camname.toUtf8().constData();

        // open the concurrent thread object for frame capture.
        capturer = new capture_thread(std_camname, data_lock);
        connect(capturer, &capture_thread::frameCaptured, this, &MainWindow::updateFrame);
        connect(capturer, &capture_thread::fpsChanged, this, &MainWindow::updateFPS);
        connect(capturer, &capture_thread::RunComplete, this, &MainWindow::closeCapturer);
        capturer->start();
        // add the text to status label
        updateStatusBar("Camera Name", camname, false);

        // change the variable indicating camera is open
        isCameraOpen = true;

        // open some buttons as visible.
        toggleHideActions(true);

}

void MainWindow::stopCamera()
{
    if (!isCameraOpen){
        return;
    }

    // stop the thread.
    capturer->setRunning(false);

    // close the variable also.
    isCameraOpen = false;
    toggleHideActions(false);

    //mainStatuslevel text reset
    foreach(QString key, mainStatusBarData->keys())
    {
        if(key=="default")
            continue;
        mainStatusBarData->insert(key, "");
    }

    mainStatusLabel->setText(mainStatusBarData->value("default"));

}

void MainWindow::updateFrame(cv::Mat *mat)
{    
    data_lock->lock();
    currentframe = *mat;
    data_lock->unlock();

    QImage frame(
                currentframe.data,
                currentframe.cols,
                currentframe.rows,
                currentframe.step,
                QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(frame);

    imageScene1->clear();
    imageScene2->clear();

    imageView1->resetMatrix();
    imageView2->resetMatrix();

    if(!image.isNull())
    {
        QPixmap image1 = image.scaledToWidth(imageView1->width());
        QPixmap image2 = image.scaledToWidth(imageView2->width());
        imageScene1->addPixmap(image1);
        imageScene2->addPixmap(image2);

        imageScene1->update();
        imageScene2->update();

        imageView1->setSceneRect(image1.rect());
        imageView2->setSceneRect(image2.rect());
    }
}

void MainWindow::calculateFPS()
{
    if (capturer != nullptr ){

        if(capturer->isFPSCalculating()){
            QMessageBox::information(this, "Information", "FPS is being calculated.");
        }
        else{
            capturer->startCalcFPS(true);
            mainStatusBarData->insert("Resolution", "...Calculating");
            updateStatusBar("FPS" ,"...Calculating");
        }
    }
}

void MainWindow::updateFPS(float fps, int width, int height)
{
    QString fps_update = mainStatusLabel->text();
    if (capturer != nullptr && capturer->isRunning())
    {
       mainStatusBarData->insert("Resolution", QString("%1(w) x %2(h)").arg(width).arg(height));
       updateStatusBar( "FPS", QString("%1").arg(fps));

    }
}

void MainWindow::doCameraMirror()
{
    if (capturer!=nullptr)
        capturer->setMirror( !capturer->isMirror());
}

void MainWindow::toggleHideActions(bool show)
{
    // close some buttons from visible.
    stopCameraAction->setVisible(show);
    fpsCalculationAction->setVisible(show);
    cameraMirrorAction->setVisible(show);
}

void MainWindow::recordingStartStop()
{
    if(capturer == nullptr)
    {
        // clear record.
        if (mainStatusLabel->text() != mainStatusBarData->value("default"))
            updateStatusBar("Record Status", "", true);

        return;
    }

   clickedRecord = !clickedRecord;

   if (clickedRecord)
   {
       recordButton->setText(recordButtonText->at(1));
       capturer->setVideoSavingStatus(capture_thread::STARTING);
       connect(capturer, &capture_thread::videoRecordStatus, this, &MainWindow::updateVideoRecordStatus);
   }

   else
   {
       recordButton->setText(recordButtonText->at(0));
       capturer->setVideoSavingStatus(capture_thread::STOPPING);
   }

}

void MainWindow::updateVideoRecordStatus(int status, QString saved_video_name)
{
    QList<QString> textualStatus =  QList<QString> ( {"Starting", "Started", "Stopping", "Stopped"} );
    updateStatusBar("Record Status", textualStatus.at(status) + "@" + saved_video_name);
    if (status==3)
    {
        recordButton->setText(recordButtonText->at(0));
        disconnect(capturer, &capture_thread::videoRecordStatus, this, &MainWindow::updateVideoRecordStatus);
    }

}

void MainWindow::closeCapturer(bool runClosed)
{

    delete capturer;
    capturer = nullptr;
    qDebug() <<"Closed the thread.";
    clickedRecord=false;
}
