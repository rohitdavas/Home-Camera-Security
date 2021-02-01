#ifndef CAPTURE_THREAD_H
#define CAPTURE_THREAD_H

#include <QString>
#include <QThread>
#include <QMutex>
#include <string>
#include <opencv2/opencv.hpp>

class capture_thread : public QThread
{
    Q_OBJECT;

public:
    capture_thread(std::string camName, QMutex *lock);
    capture_thread(QString videopath, QMutex *lock);
    ~capture_thread();
    void setRunning(bool);
    void startCalcFPS(bool);
    bool isFPSCalculating();
    void setMirror(bool);
    bool isMirror();
    enum VideoSavingStatus{
        STARTING,
        STARTED,
        STOPPING,
        STOPPED
    };
    void setVideoSavingStatus(VideoSavingStatus status);
    VideoSavingStatus getVideoSavingStatus();


private:
    bool generateFrames(cv::VideoCapture &cap, cv::Mat &tmp_frame);

protected:
    void run() override;

private:
    void calculateFPS(cv::VideoCapture &cap, cv::Mat &tmp_frame);
    void startSavingVideo(cv::Mat &firstFrame);
    void stopSavingVideo();

signals:
    void frameCaptured(cv::Mat *data);
    void fpsChanged(float fps, int width, int height);
    void videoRecordStatus(int status, QString saved_video_name);
    void RunComplete(bool);

private:
    bool running;
    bool fps_calculating;
    float fps=0.0;
    bool doMirror=false;

    std::string camname;
    QString videopath;

    QMutex *data_lock;
    cv::Mat frame;

    int frame_width, frame_height;
    VideoSavingStatus video_saving_status;
    QString saved_video_name;
    cv::VideoWriter *video_writer;


};

#endif // CAPTURE_THREAD_H
