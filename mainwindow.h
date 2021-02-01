#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QCheckBox>
#include <QPushButton>
#include <QListView>
#include <QMutex>
#include <string>
#include "capture_thread.h"
#include <opencv2/opencv.hpp>

class MainWindow: public QMainWindow
{
    Q_OBJECT;

public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();

private:
    // UI area dividing.
    void initUI();
    void initUIFileMenu();
    void initUICameraMenu();
    void initUIViewArea();
    void initUIStatusBar();
    void toggleHideActions(bool);
    QString selectCamera();
    void updateStatusBar(QString, QString, bool);
    void updateStatusBar(bool);

private slots:
	void cameraInfo();
	void cameraOpen();
    void doCameraMirror();
    void stopCamera();
    void calculateFPS();
    void updateFrame(cv::Mat *mat);
    void updateFPS(float fps, int width, int height);
    void recordingStartStop();
    void updateVideoRecordStatus(int, QString );
    void closeCapturer(bool);

private:
    //------------------------
    // initial UI variables
    //------------------------

    // file Menu and its action
    QMenu *fileMenu;
    QMenu *cameraMenu;
    QToolBar *fileToolBar;
    QToolBar *cameraToolBar;

    QAction *cameraInfoAction;
    QAction *cameraOpenAction;
    QAction *exitAction;
    QAction *stopCameraAction;
    QAction *fpsCalculationAction;
    QAction *cameraMirrorAction;
    bool isCameraOpen = false;

    // graphic scene and view needed as image handling
    QGraphicsScene *imageScene1;
    QGraphicsView *imageView1;
    QGraphicsScene *imageScene2;
    QGraphicsView *imageView2;

    // status bar and label.
    QStatusBar *mainStatusBar;
    QMap<QString, QString> *mainStatusBarData;
    QLabel *mainStatusLabel;

    // as per the GUI plan
    QCheckBox *monitorCheckBox;
    QPushButton *recordButton;
    QList<QString> *recordButtonText;
    bool clickedRecord=false;
    QListView *saved_list;

    cv::Mat currentframe;
    QMutex *data_lock;
    capture_thread *capturer;

};

#endif // MAINWINDOW_H
