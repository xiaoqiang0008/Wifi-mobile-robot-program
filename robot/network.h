#ifndef NETWORK_H
#define NETWORK_H

#include <QByteArray>
#include <QtNetwork>
#include <QMessageBox>
#include <QMainWindow>

class Network: public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = 0);
    bool Connect();  //连接到网络
    void DisConnect();  //断开网络连接
    bool SendData( QByteArray data );  //发送数据 （要发送的数据存在 “data” 里）
    bool isNetworkOn();  //网络连接状态 （“已连接” 、 “未连接”）
    void getMainWindow(QMainWindow* window);  //获取主窗口指针

private:
    void GetData(QByteArray data);//抓包函数
    void getData(QByteArray data);  //抓包函数  //现用
    unsigned char chack_sum(unsigned char* chack,int n);
    QString HostName;   //主机名称
    quint16 TcpPort;    //端口号
    QTcpSocket* tcpSocket;  //
    bool isNetworkon;    //当前网络状态
    QMainWindow* mainwindow;
    QByteArray buff;  //抓包之后的数据缓存

signals:
   void DataReceived(QByteArray data);//接收到数据并处理之后发送信号
   void DataEmpty();//发送的数据为空时 发送信号
   void netWorkerror();   //当前网络连接出现错误时发出信号

private slots:
    void ReadMessage();//数据接收槽函数
    void NetworkDataUpDate(QString* hostname , quint16* port);  //网络连接相关参数发生改变
    void displayError(QAbstractSocket::SocketError);  //当前网络连接出现错误时 显示 操作

protected:

};




#endif
