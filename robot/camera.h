#ifndef CAMERA_H
#define CAMERA_H

#include <QTimer>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QLabel>
#include <QMainWindow>
#include <QDir>
#include <QMessageBox>
#include <QDateTime>
#include <QThread>
#include <iostream>
#include <QMainWindow>
#include "network.h"

class Camera: public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = 0);
    void Show();     //关闭摄像头
    void Stop();     //打开摄像头
    bool isCameraOn();     //摄像头是否开启 （摄像头 “开启” 的前提是 “可用” ）
    bool isCameraAvailable();    //摄像头当前是否可用
    void setWiget(QLabel* movielabel); //设置图像显示位置
    void getMainWindow(QMainWindow* main_window);//获取主窗口指针
    void shot();    //截图

private:
    QString PU;     //视频流地址
    QPixmap pix;    //缓存接收到的一帧图像数据
    QLabel* movie_label;  //图像显示位置 形参
    QTimer *showTime;   //用于发送图像接收请求
    QTimer *countTime;  //用于检查图像数据是否正常接收
    QTimer* frameTime;  //帧率计算
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QMainWindow* mainwindow;
    bool isCameraon;   //摄像头开启状态（“开启” 、 “关闭”）
    bool isCameraok;   //摄像头能否接收到数据 （）
    bool isCameraavailable;  //摄像头是否可用
    unsigned char frame_rate; //帧率统计
private slots:
    void flushPic();  //发送图像数据请求
    void CameraDataUpDate(QString* pu);  //摄像头相关网络设置发生改变
    void CameraError();  //摄像头网络出现错误时 要进行的操作
    void slot_frameRate();  //更新帧率


public slots:
    void slot_replyFinished(QNetworkReply* reply);  //接收图像数据并显示



signals:
    void cameraError();  //摄像头网络出现错误时发送信号
    void signal_frameRrate(unsigned char frame_rate);//发送当前图像帧率


};


class CameraThread: public QThread
{
    Q_OBJECT
public:
    CameraThread();
    ~CameraThread();
    void stop();
    void Start();
    void setmainwindow(Network* mainwindow);

private:
    QTimer* cameratime;
    Camera* camera;
    //Network* window;
    Network* network;
    volatile bool stopped;
    bool timeroutt;
    bool begin;
    QByteArray data;
    char q0,q00,q1,q2,q3,q4,q5,q6,q7;

    void Grab();

private slots:
    void timerout();

public slots:


signals:


protected:
    void run();
};

#endif // CAMERA_H
