#include "capture_thread.h"
#include "utilities.h"
#include <string>
#include <QDebug>
#include <QTime>
#include <iostream>
#include <opencv2/highgui.hpp>

capture_thread::capture_thread(std::string camname, QMutex *lock):
    running(false), camname(camname), videopath(""), data_lock(lock)
{
    fps_calculating = false;
    fps = 0.0;

    frame_width=frame_height=0;
    video_writer=nullptr;
    video_saving_status=STOPPED;
}

capture_thread::capture_thread(QString videopath, QMutex *lock):
    running(false), camname("nocam"), videopath(videopath), data_lock(lock)
{
    fps_calculating = false;
    fps = 0.0;

    frame_width=frame_height=0;
    video_writer=nullptr;
    video_saving_status=STOPPED;
    saved_video_name="";

}

capture_thread::~capture_thread()
{

}

void capture_thread::setRunning(bool run){
    data_lock->lock();
    running=run;
    data_lock->unlock();
}

void capture_thread::startCalcFPS(bool start){
    data_lock->lock();
    fps_calculating = start;
    data_lock->unlock();
}

bool capture_thread::isFPSCalculating(){return fps_calculating;}

bool capture_thread::generateFrames(cv::VideoCapture &cap, cv::Mat &tmp_frame){

    cap >> tmp_frame;
    if (tmp_frame.empty()){
        return false;
    }

    cv::cvtColor(tmp_frame, tmp_frame, cv::COLOR_BGR2RGB);
    data_lock->lock();
    cv::flip( tmp_frame, frame, 1);
    data_lock->unlock();
    emit frameCaptured(&frame);
    return true;


}

void capture_thread::run()
{
    // a helpful code snippet
    // https://www.kurokesu.com/main/2020/07/12/pulling-full-resolution-from-a-webcam-with-opencv-windows/
    setRunning(true);
    cv::VideoCapture cap(camname);
    if (!cap.isOpened()){
        CV_Assert("Failed to open camera.");
    }

    cap.set(cv::CAP_PROP_FPS, 30);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

    frame_width=cap.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height=cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    qDebug() << QString("frame width %1 height %2").arg(frame_width).arg(frame_height);
    cv::Mat tmp_frame;
    cv::Mat tmp_frame2;
    // for fps calculation.
    int frame_count=0;
    QTime timer;
    bool first_time=true;
    const int n_frames_to_consider=30;
    qDebug() << QString("running status %1").arg(running);

    while(running){
//        qDebug() <<QString("Video saving status - %1").arg(video_saving_status);
        cap >> tmp_frame;
        if(tmp_frame.empty())
            break;

        if (doMirror)
        {
            cv::flip(tmp_frame, tmp_frame2, 1);
            tmp_frame = tmp_frame2;
        }

        if (fps_calculating)
        {
            if(first_time)
            {
                timer.start();
                first_time=false;
            }

            else if(frame_count != n_frames_to_consider)
                frame_count++;

            else if(frame_count == n_frames_to_consider)
            {
                int elapsed_ms = timer.elapsed();
                fps = frame_count / (elapsed_ms / 1000.0);
                startCalcFPS(false);
                emit fpsChanged(fps, frame_width, frame_height);
                first_time = true;
                frame_count=0;
            }

        }

        if (video_saving_status != STOPPED )
        {
            if(video_saving_status == STARTING)
                startSavingVideo(tmp_frame);

            else if(video_saving_status == STARTED)
                video_writer->write(tmp_frame);

            else if(video_saving_status == STOPPING)
                stopSavingVideo();
        }

        // convert color to visualise at screen
        cv::cvtColor(tmp_frame, tmp_frame, cv::COLOR_BGR2RGB);

        // write and emit frame
        data_lock->lock();
        frame=tmp_frame;
        data_lock->unlock();
        emit frameCaptured(&frame);

    }

    if(video_saving_status != STOPPED)
        stopSavingVideo();
    cv::Mat *mat_cal = new cv::Mat(frame_height, frame_width, CV_8U, 255);
    emit frameCaptured(mat_cal);
    cap.release();
    setRunning(false);

    qDebug()<<"stopped running.";
    emit RunComplete(true);
}

void capture_thread::setVideoSavingStatus(VideoSavingStatus status)
{
    data_lock->lock();
    video_saving_status = status;
    data_lock->unlock();
}

void capture_thread::startSavingVideo(cv::Mat &firstFrame)
{
    saved_video_name = utilities::newSavedVideoName();

    // generate a cover image for video.
    QString cover_path = utilities::getSavedVideoPath(saved_video_name, "jpg");
    cv::imwrite(cover_path.toStdString(), firstFrame);

    // video save path.
    QString video_saved_path = utilities::getSavedVideoPath(saved_video_name, "avi");
    video_writer = new cv::VideoWriter(
                video_saved_path.toStdString(),
                cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                fps? fps:30,
                cv::Size(frame_width, frame_height)
                );
    setVideoSavingStatus(STARTED);
    saved_video_name = video_saved_path;
    emit videoRecordStatus(video_saving_status, saved_video_name);

}

void capture_thread::stopSavingVideo()
{
    setVideoSavingStatus( STOPPED );
    if (video_writer != nullptr )
    {
        video_writer->release();
        delete video_writer;
        video_writer=nullptr;

    }
    emit videoRecordStatus(video_saving_status, saved_video_name);
}

void capture_thread::setMirror(bool mirror)
{
    data_lock->lock();
    doMirror=mirror;
    data_lock->unlock();
}

bool capture_thread::isMirror(){
    return doMirror;
}

capture_thread::VideoSavingStatus capture_thread::getVideoSavingStatus()
{
    return video_saving_status;
}





















