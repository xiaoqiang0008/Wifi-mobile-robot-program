#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>
#include <QLineEdit>
#include <QtNetwork/QUdpSocket>
#include <QProcess>
#include <QFile>
#include <QKeyEvent>    //键盘
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QFrame>
#include <QToolButton>
#include <QBitmap>
#include <QScrollBar>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
//#include <QtWidgets>
#include "network.h"
#include "camera.h"
#include "mcurvepaint.h"

class mCurvePaint;
class Network;
class Camera;

namespace Ui {
class QAction;
class QMenu;
class QDialog;
class Network;
class Camera;
class mCurvePaint;
class chileDialog;
class QPainter;
class QToolButton;
class MainWindow;
class CarDialog;
class ManPuDialog;
class tthersDialog;
class CameraThread;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    #define pi  3.1415925358979323846264

private:
    Ui::MainWindow *ui;
    float Tem,Hd,MQ4,MQ9;       //接收到的当前传感器数据
    char tem_curvebuff[100],    //温度数据缓存
            hd_curvebuff[100],  //湿度数据缓存
            mq4_curvebuff[100], //MQ4 气体传感器数据缓存
            mq9_curvebuff[100]; //MQ9 气体传感器数据缓存
    //QPoint mousePoint;
    QPoint widgetPos;       //鼠标当前位置记录
    QPoint clickPos;        //当前窗口位置记录     用于捕捉鼠标移动事件，实现窗口移动
    Network* network;       //网络连接类 对象
    Camera* camera;         //网络摄像头类 对象
    mCurvePaint* mPainter;  //曲线绘制类 对象
    char temBuffer[100];    //滑动块1 数据缓存    （试验用后期删掉）
    char temBuffer2[100];   //滑动块2 数据缓存    （试验用后期删掉）
    QTimer* temTimer;       //数据获取间隔时间
    QTimer* widgetTimer;    //图像数据显示play_widget显示时间设置

   // CameraThread* camerathread;
    bool step1,step2,step3,step4,step5;
    QTimer* GrabTimer;
    QButtonGroup getThreeFucSelect;

    bool eventFilter(QObject *obj, QEvent *event);   //事件过滤器
    void getdataout(QByteArray *data);               //机械手逆解计算
    void keyPressEvent(QKeyEvent  *event);           //按键按下事件
    void keyReleaseEvent(QKeyEvent  *event);         //按键松开事件
    void datasend(char d);                           //发送数据
    void paintEvent(QPaintEvent *);                  //重绘事件

    void setBuff(int val , char *buff);              //更新数据缓冲区

    void Grab();

signals:
    void upDateButton();                  //需要更新按钮状态时 发送信号

private slots:
    void caculate();       // “计算逆解” 按钮槽函数
    void bt_clear();       //清空状态显示内容
    void quit();           //关闭程序按钮  槽函数
    void btsend();         //计算并发送逆解  按钮槽函数
    void newConnect();     //建立连接或者取消连接 按钮槽函数
    void bt_send_qx();     //抓取物品 发送按钮槽函数
    void bt_reset_qx();    //抓取物品 复位按钮槽函数
    void bt_intnet_button();          //网络设置  按钮槽函数
    void bt_car_button();             //小车相关设置 按钮槽函数
    void bt_manipulator_button();     //机械手相关设置 按钮槽函数
    void bt_others_button();          //其他设置  按钮槽函数
    void updateButton();              //更新按钮状态槽函数     接收upDateButton()信号
    void bt_stopPic();                //网络摄像头显示  停止按钮槽函数
    void bt_show_clicked();           //网络摄像头显示  开始/暂停 按钮槽函数
    void bt_shot_clicked();           //网络摄像头显示  截图 按钮槽函数
    void on_bt_high_clicked();        //高速行走 按钮槽函数
    void on_bt_low_clicked();         //低速行走 按钮槽函数
    void DataReceived(QByteArray buff);            //网络数据接收  槽函数   接收Network类数据接收信号
    void Cam_frameRate(unsigned char frame_rate);  //网络摄像头 帧率更新槽函数  接收Camera类帧率更新信号
    void slot_widgettimeout();        //图像数据显示 play_widget 定时时间到处理函数

    void timesomething();             //更新各数据缓冲区

    void on_bt_full_clicked();        //图像数据显示 全屏/退出全屏 按钮槽函数

    void on_pushButton_clicked();
    void grabtimeout();
    void slot_ScrollBarchanged(int value);
    void slot_lineEditchanged();

    void on_pushButton_2_clicked();

protected:

};

class chileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit chileDialog(QDialog *parent = 0);
    ~chileDialog();
    void GetMainWindow(MainWindow *w);

signals:
    void SendText(QString* hostname , quint16* port);
    void SendCameraText(QString* cameraaddr);

private slots:
    void bt_enter_clicked();
    void bt_cancel_clicked();

private:
    Ui::chileDialog *ui;
    QPoint widgetPos;
    QPoint clickPos;
    MainWindow *mainwindow;
    chileDialog *chiledialog;
    Network* network;
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *obj, QEvent *event);
};

class CarDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CarDialog(QDialog *parent = 0);
    ~CarDialog();
    void GetMainWindow(MainWindow *w);

signals:
   // SendText();

private slots:
   // void bt_enter_clicked();
    void bt_cancel_clicked();

private:
    Ui::CarDialog *ui;
    QPoint widgetPos;
    QPoint clickPos;
    MainWindow *mainwindow;
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *obj, QEvent *event);
};

class ManPuDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ManPuDialog(QWidget *parent = 0);
    ~ManPuDialog();
    void GetMainWindow(MainWindow *w);

signals:
   // SendText();

private slots:
   // void bt_enter_clicked();
    void bt_cancel_clicked();

private:
    Ui::ManPuDialog *ui;
    QPoint widgetPos;
    QPoint clickPos;
    MainWindow *mainwindow;
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *obj, QEvent *event);
};

class tthersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit tthersDialog(QWidget *parent = 0);
    ~tthersDialog();
    void GetMainWindow(MainWindow *w);

signals:
   // SendText();

private slots:
   // void bt_enter_clicked();
    void bt_cancel_clicked();

private:
    Ui::tthersDialog *ui;
    QPoint widgetPos;
    QPoint clickPos;
    MainWindow *mainwindow;
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
