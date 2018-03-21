#include "network.h"

Network::Network(QObject *parent):
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(ReadMessage()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));
    HostName = "192.168.1.1";
    TcpPort = 2001;
    isNetworkon = false;
}

bool Network::Connect()
{
    tcpSocket->connectToHost( HostName , TcpPort );
    if(tcpSocket->waitForConnected(100))
    {
        isNetworkon = true;
        buff.clear();
        return true;
    }
    else
    {
        isNetworkon = false;
        buff.clear();
        return false;
    }
}

void Network::DisConnect()
{
    tcpSocket->abort();
    buff.clear();
    isNetworkon = false;
}

void Network::ReadMessage()
{
    QByteArray tep= tcpSocket->readAll();
    getData(tep);
}

unsigned char Network::chack_sum(unsigned char* chack,int n)
{
    qDebug(" 开始 jiao yan");
    unsigned char i,sum=0xff;
   // if(*chack==0xff && *(chack+n-1)==0xff)
   // {
        sum=0;
        for(i=0;i<n-1;i++)
        {
            sum=sum+*(chack+i);
        }
        if(sum == 0xff)
        {
            sum=0xfe;
        }
   // }
    return sum;
}

void Network::getData(QByteArray data)
{
    char i=0,k=0,m=0;
    static char j=0,n=0;
    unsigned char* s = (unsigned char*)data.data();
   // qDebug("%d ",data.length());
    if(0xff == *s && j == 0)
    {
        for(i=0;i<data.length();i++)
        {
            if(0xff != *(s+i))
                buff[k++] =*(s+i);
            else if(0xff == *(s+i) && k != 0)
            {
             //   qDebug("emit !");
                //if(*(s+buff.length()-1) == chack_sum(s,buff.length()))
               // {
               //     qDebug("jiao yan 成功");
                    emit DataReceived(buff);
               // }
                buff.clear();
                k=0;
                m++;
                if(m == 2)
                {
                    m=0;
                    break;
                }
            }
            if(0xff != *(s+data.length()-1))
            {
                j=1;
                n=k;
            }
            else
            {
                j=0;
                n=0;
            }
        }
    }
    if(j==1 && 0xff != *s)
    {
        for(i=0;i<data.length();i++)
        {
            if(0xff != *(s+i))
            buff[n++]= *(s+i);
            else
            {
                j=0;
                n=0;
              //  if(*(s+buff.length()-1) == chack_sum(s,buff.length()))
              //  {
                 //   qDebug("jiao yan 成功");
                    emit DataReceived(buff);
              //  }
                buff.clear();
              //  qDebug("emit2 !");
                break;
            }
        }
    }
}

void Network::GetData(QByteArray data)
{
    char i=0,k=0;
    static char j=0,n=0;
    unsigned char* s = (unsigned char*)data.data();
    static QByteArray buff;
    if(0xff == *s && j == 0)
    {
        for(i=0;i<=data.length();i++)
        {
            if(0xff != *(s+i))
            buff[k++] =*(s+i);
            else
            {
                j++;
                if(j==4)
                {
                    j=0;
                    emit DataReceived(buff);
                    break;
                }
                else
                {
                    n=k+1;
                }
            }

        }
    }
    if(j!=0 && 0xff!=*s)
    {
        for(i=0;i<=data.length();i++)
        {
            if(0xff != *(s+i))
            buff[n++]= *(s+i);
            else
            {
                j=0;
                n=0;
                emit DataReceived(buff);
                break;
            }
        }
    }
}

bool Network::SendData(QByteArray data)
{
    char* a;
    a=data.data();
    if(data.length()>0)
    {
        tcpSocket->write(a,data.length());
        if(tcpSocket->waitForBytesWritten())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        emit DataEmpty();  //如果数据为空，则发出信号
        return false;
    }
}

void Network::NetworkDataUpDate(QString* hostname , quint16* port)
{
    HostName = *hostname;
    TcpPort = *port;
    qDebug("NetworkDataUpDate");
  //  qDebug("%s %d",QByteArray(HostName.toLatin1()).toHex() , TcpPort);
}

bool Network::isNetworkOn()
{
    return isNetworkon;
}

void Network::getMainWindow(QMainWindow* window)
{
    mainwindow = window;
}

void Network::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString(); //输出错误信息
    isNetworkon = false;
    buff.clear();
    QMessageBox::information(mainwindow,
                             tr("warning"),
                             tr("An unexpected network error occurred !"));
    emit netWorkerror();
}
