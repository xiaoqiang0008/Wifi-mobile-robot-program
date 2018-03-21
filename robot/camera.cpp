#include "camera.h"
#include <QThread>
#include <QMainWindow>

Camera::Camera(QObject *parent):
    QObject(parent)
{
    showTime=new QTimer;
    //发送请求
    showTime->setInterval(40);
    connect(showTime, SIGNAL(timeout()), this, SLOT(flushPic()));
    countTime=new QTimer;
    countTime->setInterval(2000);
    connect(countTime, SIGNAL(timeout()), this, SLOT(CameraError()));
    frameTime=new QTimer;
    connect(frameTime, SIGNAL(timeout()), this, SLOT(slot_frameRate()));
    frameTime->setInterval(1000);

    //显示图像
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slot_replyFinished(QNetworkReply*)));

    PU="http://192.168.1.1:8080/?action=snapshot";
    isCameraon = false;
    isCameraok = false;
    isCameraavailable = false;
    frame_rate = 0;
}

void Camera::Show()
{
    countTime->start();
    showTime->start();
    frameTime->start();
    isCameraon=true;
    qDebug("camera show");
}

void Camera::Stop()
{
    showTime->stop();
    countTime->stop();
    frameTime->stop();
    isCameraok=false;
    isCameraon=false;
}

bool Camera::isCameraOn()
{
    return isCameraon;
}

bool Camera::isCameraAvailable()
{
    return isCameraavailable;
}

void Camera::getMainWindow( QMainWindow* main_window)
{
    mainwindow=main_window;
}

void Camera::shot()
{
    if(!QDir("photo").exists())
    {
        QDir dir;
        dir.mkpath("photo");
    }
    QString str_time,path;
    QDateTime time = QDateTime::currentDateTime();
    str_time = time.toString("yyyy-MM-dd_hh-mm-ss");
    path="./photo/"+str_time+".jpg";
    pix.save(path);
    QMessageBox::information(mainwindow,
                             tr("save"),
                             tr("PICTURE has been saved to folder 'photo!'"));
}

void Camera::setWiget(QLabel* movielabel)
{
    movie_label = movielabel;
}

void Camera::slot_replyFinished(QNetworkReply* reply)
{
    isCameraok = true;
    isCameraavailable = true;
    frame_rate++;
    QByteArray data = reply->readAll();
    pix.loadFromData(data, "JPEG");
    if(NULL != movie_label)
    {
        movie_label->setPixmap(pix);
        movie_label->setScaledContents(true);
    }
    else { }
}

void Camera::flushPic()
{
    request.setUrl(QUrl(PU));
    manager->get(request);
  //  qDebug("send request");
}

void Camera::CameraDataUpDate(QString* pu)
{
    PU=*pu;
    qDebug("CameraDataUpDate");
  //  qDebug("%s", PU);
}

void Camera::CameraError()
{
    if(false == isCameraok)
    {
        showTime->stop();
        countTime->stop();
        isCameraavailable = false;
        QMessageBox::information(mainwindow,
                                 tr("warning"),
                                 tr("Camera open failed !"));
        emit cameraError();
    }
    else
    {
        isCameraavailable = true;
    }
    isCameraok=false;

}

void Camera::slot_frameRate()
{
    emit signal_frameRrate(frame_rate);
    frame_rate=0;
}



CameraThread::CameraThread()
{
    //cameratime = new QTimer;
    //cameratime->setInterval(1000);
    //connect(cameratime, SIGNAL(timeout()), this, SLOT(timerout()),Qt::QueuedConnection);
    //cameratime->start();
    qDebug("camerathread is bulid");
    stopped = false;
    begin = false;



}

CameraThread::~CameraThread()
{
   // delete cameratime;
    delete network;
}

void CameraThread::setmainwindow(Network* mainwindow)
{
    network = mainwindow;

}

void CameraThread::stop()
{
    stopped = true;

}

void CameraThread::Start()
{
    begin = ~begin;
}

void CameraThread::Grab()
{
    QByteArray data;
    char q0,q00,q1,q2,q3,q4,q5,q6,q7;
    q0=0xff;
    q00=0xaa; q7=0xff; q1 = 20; q2 = 0; q3 = 30;
    q4 = 110; q5 = 0; q6 = 0;
    data.clear();
    data.append(q0);
    data.append(q00);
    data.append(q1);
    data.append(q2);
    data.append(q3);
    data.append(q4);
    data.append(q5);
    data.append(q6);
    data.append(q7);
    if(network->SendData(data))
    {
        qDebug("thread send data sucsfful");
    }
    else
    {
        qDebug("thread send data flaid");
    }
    this->sleep(15);
    data[7] = 60;
    network->SendData(data);
    this->sleep(3);
    data[3] = 15; data[4] = 40; data[5] = 160; data[6] = 160;
    network->SendData(data);
    this->sleep(15);
    data[0] = 0;
    network->SendData(data);
    this->stop();
}


void CameraThread::timerout()
{
    timeroutt = true;
    qDebug()<<"mainthread ID:"<<QThread::currentThreadId();
}

void CameraThread::run()
{

  //  camera->setWiget(movielabel);
   // camera->Show();
    qDebug("camerathread is running");
    qDebug()<<"camerathread ID:"<<QThread::currentThreadId();
    QByteArray data;
    char q0,q00,q1,q2,q3,q4,q5,q6,q7;
   // this->exec();
    while(!stopped)
    {
        qDebug("start");
        if(begin == true )
        {
      qDebug("start 2");
      q0=0xff;
      q00=0xaa; q7=0xff; q1 = 20; q2 = 0; q3 = 30;
      q4 = 110; q5 = 0; q6 = 0;
      data.clear();
      data.append(q0);
      data.append(q00);
      data.append(q1);
      data.append(q2);
      data.append(q3);
      data.append(q4);
      data.append(q5);
      data.append(q6);
      data.append(q7);
      if(network->SendData(data))
      {
          qDebug("thread send data sucsfful");
      }
      else
      {
          qDebug("thread send data flaid");
      }
      this->sleep(15);
      data[7] = 60;
      network->SendData(data);
      this->sleep(3);
      data[3] = 15; data[4] = 40; data[5] = 160; data[6] = 160;
      network->SendData(data);
      this->sleep(15);
      data[0] = 0;
      network->SendData(data);
      //this->stop();
        }
    }
    stopped = true;

}




