#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include<QMessageBox>
#include <QDir>
#include <QKeyEvent>    //键盘
#include <QToolBar>
#include <QPainter>
#include <QProxyStyle>
#include <QtCore/QCoreApplication>
#include <QByteArray>
#include <qdebug.h>
#include <Qt>

class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 102; // 设置每个tabBar中item的大小
            s.rheight() = 44;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;   //以整数精度定义一个矩形对象
//0x89cfff       f8fcff
                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(Qt::lightGray);
                    painter->setBrush(QBrush(Qt::lightGray));
                    painter->drawRect(allRect.adjusted(2, 0, 0, 0));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(Qt::white); //字体显示白色
                }
                else {
                    painter->setPen(Qt::black);
                }
                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
        if(element == CE_PushButton)
        {
            if (const QStyleOptionButton *pushbutton = qstyleoption_cast<const QStyleOptionButton *>(option)){
                QPixmap pixmap(":/image/7.jpg");
                painter->drawPixmap(pushbutton->rect, pixmap);
                painter->drawText(pushbutton->rect,"set calc");


            }
        }
        if(element == CE_PushButton)
        {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    step1 = false; step2 = false; step3 = false; step4=false; step5=false;
    GrabTimer = new QTimer;
    connect(GrabTimer, SIGNAL(timeout()), this, SLOT(grabtimeout()));


    //设置禁止改变窗口大小
    this->setWindowFlags(0);
    this->setFixedSize(1124,714);

    Tem = 0;
    Hd = 0;
    MQ4 = 0;
    MQ9 = 0;

    temTimer = new QTimer;
    temTimer->setInterval(40);
    temTimer->start();
    connect(temTimer, SIGNAL(timeout()), this, SLOT(timesomething()));
    widgetTimer = new QTimer;

    unsigned char i;
    for(i=0;i<100;i++)
    {
        temBuffer[i] = 20;
        temBuffer2[i] = 10;
        tem_curvebuff[i] =20;
        hd_curvebuff[i] =20;
        mq4_curvebuff[i] =20;
        mq9_curvebuff[i] =20;
    }

    mPainter = new mCurvePaint(this);
    mPainter->setTitlecolor(Qt::white);
    mPainter->setAxespen(QPen(Qt::white,1,Qt::SolidLine));
    mPainter->setCurve1color(Qt::yellow);
    mPainter->setCurve2color(Qt::green);
    mPainter->setDataBuff_num(100,100);
    mPainter->setX_axes(1000);

    network = new Network(this);
    network->getMainWindow(this);


   // camerathread = new CameraThread;
    //QLabel* movie1 = ui->movie_label;
   // camerathread->setmainwindow(network);
   // camerathread->start();

    camera = new Camera(this);
    QLabel* movie = ui->movie_label;
    camera->setWiget(movie);
    camera->getMainWindow(this);

    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px solid #e8f3f9;background:  transparent; }");
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
   // QPushButton mpushbutton ;
  //  mpushbutton.setStyle(new CustomTabStyle);
  //
  //  ui->bt_caculate->setStyle(new CustomTabStyle);

//  SP_BrowserReload      重载

        int wide = width();//获取界面的宽度
        //构建最小化、最大化、关闭按钮
        QToolButton *minButton = new QToolButton(this);
        QToolButton *closeButton= new QToolButton(this);
        QToolButton *maxButton= new QToolButton(this);
        //获取最小化、关闭按钮图标
        QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
        QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
        QPixmap maxPix = style()->standardPixmap(QStyle::SP_MediaVolumeMuted);
        //设置最小化、关闭按钮图标
        minButton->setIcon(minPix);
        closeButton->setIcon(closePix);
        maxButton->setIcon(maxPix);
        //设置最小化、关闭按钮在界面的位置
        minButton->setGeometry(wide-55,15,20,20);
        closeButton->setGeometry(wide-35,15,20,20);
        maxButton->setGeometry(wide-75,15,20,20);
        //设置鼠标移至按钮上的提示信息
        minButton->setToolTip(tr("最小化"));
        closeButton->setToolTip(tr("关闭"));
        maxButton->setToolTip(tr("最大化"));
        //设置最小化、关闭按钮的样式

        minButton->setStyleSheet("background-color:transparent;");
        closeButton->setStyleSheet("background-color:transparent;");

        maxButton->setStyleSheet("background-color:transparent;");

        connect(closeButton, SIGNAL(clicked()), this, SLOT(quit()) );
        connect(minButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
     //   connect(maxButton, SIGNAL(clicked()), this, SLOT(showMaximized()));

    ui->intnet_button->setIcon(QIcon(":/image/int.png"));
   // ui->intnet_button->setIconSize(ui->intnet_button->size());
    QIcon icon=ui->intnet_button->icon();
    ui->intnet_button->setStyleSheet("background-color:transparent;");

    ui->intnet_button->setStyleSheet("QPushButton{border-image:url(:/image/image/int.png);}\
                                  QPushButton:hover{border-image: url(:/image/image/int2.png);}\
                                  QPushButton:pressed{border-image: url(:/image/image/int3.png);}");
    connect(ui->intnet_button,SIGNAL(clicked()),this,SLOT(bt_intnet_button()));

    ui->car_button->setIcon(QIcon(":/image/car.png"));
   // ui->car_button->setIconSize(ui->car_button->size());
     icon=ui->car_button->icon();
    ui->car_button->setStyleSheet("background-color:transparent;");

    ui->car_button->setStyleSheet("QPushButton{border-image:url(:/image/image/car.png);}\
                                  QPushButton:hover{border-image: url(:/image/image/car2.png);}\
                                  QPushButton:pressed{border-image: url(:/image/image/car3.png);}");
    connect(ui->car_button,SIGNAL(clicked()),this,SLOT(bt_car_button()));

    ui->manipulator_button->setIcon(QIcon(":/image/shou.png"));
  //  ui->manipulator_button->setIconSize(ui->manipulator_button->size());
    icon=ui->manipulator_button->icon();
    ui->manipulator_button->setStyleSheet("background-color:transparent;");

    ui->manipulator_button->setStyleSheet("QPushButton{border-image:url(:/image/image/shou.png);}\
                                  QPushButton:hover{border-image: url(:/image/image/shou2.png);}\
                                  QPushButton:pressed{border-image: url(:/image/image/shou3.png);}");
    connect(ui->manipulator_button,SIGNAL(clicked()),this,SLOT(bt_manipulator_button()));


    ui->others_button->setIcon(QIcon(":/image/image/others.png"));
    //  ui->others_button->setIconSize(ui->others_button->size());
    icon=ui->others_button->icon();
    ui->others_button->setStyleSheet("background-color:transparent;");
    ui->others_button->setStyleSheet("QPushButton{border-image:url(:/image/image/others.png);}\
                                        QPushButton:hover{border-image: url(:/image/image/others2.png);}\
                                        QPushButton:pressed{border-image: url(:/image/image/others3.png);}");
    connect(ui->others_button,SIGNAL(clicked()),this,SLOT(bt_others_button()));

    ui->bt_clear->setStyleSheet("background-color:transparent;");
    ui->bt_clear->setStyleSheet("QPushButton{border-image:url(:/button/button/delete1.png);}\
                                  QPushButton:hover{border-image: url(:/button/button/delete1.png);}\
                                  QPushButton:pressed{border-image: url(:/button/button/delete3.png);}");
    ui->bt_clear->setToolTip(tr("清除内容！"));

   // ui->bt_connect->setStyleSheet("border-image:url(:/button/button1.png)");
    ui->bt_connect->setStyleSheet("background-color:transparent;");
    ui->bt_connect->setStyleSheet("QPushButton{border-image:url(:/button/button/button1.png);}\
                               QPushButton:hover{border-image: url(:/button/button/button1.png);}\
                               QPushButton:pressed{border-image: url(:/button/button/button11.png);}");
    ui->bt_connect->setToolTip(tr("未连接，点击建立连接！"));
    connect(this,SIGNAL(upDateButton()),this,SLOT(updateButton()));
  //  connect(this,SIGNAL(disConnect()),this,SLOT(btdisconnect()));

//摄像头图像数据显示相关界面设置
    ui->bt_stop->setStyleSheet("background-color:transparent;");
    ui->bt_stop->setStyleSheet("QPushButton{border-image:url(:/button/button/zanting3.png);}\
                               QPushButton:hover{border-image: url(:/button/button/zanting3.png);}\
                               QPushButton:pressed{border-image: url(:/button/button/zanting3.png);}");
    ui->bt_stop->setToolTip(tr("停止"));
    connect(ui->bt_stop,SIGNAL(clicked()),this,SLOT(bt_stopPic()));

    ui->bt_show->setStyleSheet("background-color:transparent;");
    ui->bt_show->setStyleSheet("QPushButton{border-image:url(:/button/button/zanting2.png);}\
                               QPushButton:hover{border-image: url(:/button/button/zanting2.png);}\
                               QPushButton:pressed{border-image: url(:/button/button/zanting2.png);}");
    ui->bt_show->setToolTip(tr("开始"));

    ui->bt_shot->setStyleSheet("background-color:transparent;");
    ui->bt_shot->setStyleSheet("QPushButton{border-image:url(:/button/button/jietu.png);}\
                               QPushButton:hover{border-image: url(:/button/button/jietu.png);}\
                               QPushButton:pressed{border-image: url(:/button/button/jietu.png);}");
    ui->bt_shot->setToolTip(tr("截图"));

    ui->bt_full->setStyleSheet("background-color:transparent;");
    ui->bt_full->setStyleSheet("QPushButton{border-image:url(:/button/button/quanping1.png);}\
                               QPushButton:hover{border-image: url(:/button/button/quanping1.png);}\
                               QPushButton:pressed{border-image: url(:/button/button/quanping1.png);}");
    ui->bt_full->setToolTip(tr("全屏"));

    ui->bt_shot->hide();
    ui->bt_show->hide();
    ui->bt_stop->hide();
    ui->bt_full->hide();
    ui->play_widget->setStyleSheet("#play_widget{background-color: rgba(0, 0, 0, 0)}");
    ui->play_widget->installEventFilter(this);
    ui->movie_label->resize(ui->widget_2->size());
    ui->play_widget->resize(ui->movie_label->width(),41);

    ui->widget_title->installEventFilter(this);//设置widget_title title 触发事件
    ui->title->installEventFilter(this);
    ui->tem->installEventFilter(this);
    ui->hd->installEventFilter(this);
    ui->MQ4->installEventFilter(this);
    ui->MQ9->installEventFilter(this);
    ui->tem_widget->installEventFilter(this);
    ui->hd_widget->installEventFilter(this);
    ui->MQ4_widget->installEventFilter(this);
    ui->MQ9_widget->installEventFilter(this);

    ui->tem_widget->setBackgroundRole(QPalette::Shadow);
    ui->tem_widget->setAutoFillBackground(true);
    ui->hd_widget->setBackgroundRole(QPalette::Shadow);
    ui->hd_widget->setAutoFillBackground(true);
    ui->MQ4_widget->setBackgroundRole(QPalette::Shadow);
    ui->MQ4_widget->setAutoFillBackground(true);
    ui->MQ9_widget->setBackgroundRole(QPalette::Shadow);
    ui->MQ9_widget->setAutoFillBackground(true);

    this->setWindowFlags(Qt::FramelessWindowHint);  //隐藏窗体边框
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗体透明效果

    ui->mq4_label->setText("wiating data");
    ui->mq9_label->setText("wiating data");
    ui->tem_label->setText("wiating data");
    ui->hd_label->setText("wiating data");

    connect(ui->bt_show, SIGNAL(clicked()), this, SLOT(bt_show_clicked()));
    connect(ui->bt_shot, SIGNAL(clicked()), this, SLOT(bt_shot_clicked()));

    connect(ui->bt_caculate,SIGNAL(clicked()),this,SLOT(caculate()));
    connect(ui->bt_clear,SIGNAL(clicked()),this,SLOT(bt_clear()));
    connect(ui->bt_send,SIGNAL(clicked()),this,SLOT(btsend()));
    connect(ui->bt_connect,SIGNAL(clicked()),this,SLOT(newConnect()));
    connect(ui->send_qx,SIGNAL(clicked()),this,SLOT(bt_send_qx()));
    connect(ui->reset_qx,SIGNAL(clicked()),this,SLOT(bt_reset_qx()));

    connect(ui->verticalScrollBar_q1,SIGNAL(valueChanged(int )),
            this,SLOT(slot_ScrollBarchanged(int )));
    connect(ui->verticalScrollBar_q2,SIGNAL(valueChanged(int )),
            this,SLOT(slot_ScrollBarchanged(int )));
    connect(ui->verticalScrollBar_q3,SIGNAL(valueChanged(int )),
            this,SLOT(slot_ScrollBarchanged(int )));
    connect(ui->verticalScrollBar_q4,SIGNAL(valueChanged(int )),
            this,SLOT(slot_ScrollBarchanged(int )));
    connect(ui->verticalScrollBar_q5,SIGNAL(valueChanged(int )),
            this,SLOT(slot_ScrollBarchanged(int )));
    connect(ui->verticalScrollBar_q6,SIGNAL(valueChanged(int )),
            this,SLOT(slot_ScrollBarchanged(int )));
    connect(ui->verticalScrollBar_q_all,SIGNAL(valueChanged(int )),
            this,SLOT(slot_ScrollBarchanged(int )));

    connect(ui->lineEdit_q1_2,SIGNAL(editingFinished()),this,SLOT(slot_lineEditchanged()));
    connect(ui->lineEdit_q2_2,SIGNAL(editingFinished()),this,SLOT(slot_lineEditchanged()));
    connect(ui->lineEdit_q3_2,SIGNAL(editingFinished()),this,SLOT(slot_lineEditchanged()));
    connect(ui->lineEdit_q4_2,SIGNAL(editingFinished()),this,SLOT(slot_lineEditchanged()));
    connect(ui->lineEdit_q5_2,SIGNAL(editingFinished()),this,SLOT(slot_lineEditchanged()));
    connect(ui->lineEdit_q6_2,SIGNAL(editingFinished()),this,SLOT(slot_lineEditchanged()));
    connect(ui->lineEdit_q_all,SIGNAL(editingFinished()),this,SLOT(slot_lineEditchanged()));


    ui->radioButton->setChecked(true);
    getThreeFucSelect.addButton(ui->radioButton,0);
    getThreeFucSelect.addButton(ui->radioButton_2,1);
}
MainWindow::~MainWindow()
{
    delete temTimer;
    delete widgetTimer;
    delete mPainter;
    delete network;
    delete camera;
    delete ui;
   // camerathread->stop();
  //  camerathread->quit();
  //  camerathread->wait();
   // camerathread->exit();
   // camerathread->wait();

}
void MainWindow::paintEvent(QPaintEvent *)
{
     QPixmap pixmap = QPixmap(":/image/image/9.jpg").scaled(this->size());
     QPainter painter(this);
     painter.drawPixmap(this->rect(), pixmap);
///////////
     QPainterPath path;
     path.setFillRule(Qt::WindingFill);
     path.addRect(2, 100, this->width()-4, this->height()-4);

     QPainter painter1(this);
     painter1.setRenderHint(QPainter::Antialiasing, true);
     painter1.fillPath(path, QBrush(QColor(255, 255, 255,240)));
        /////
     QPainterPath path1;
     path1.setFillRule(Qt::WindingFill);
     path1.addRect(2, 630, this->width()-4, this->height()-4);

     painter1.setRenderHint(QPainter::Antialiasing, true);
     painter1.fillPath(path1, QBrush(QColor(255, 255, 255,255)));

     QPainterPath path2;
     path2.setFillRule(Qt::WindingFill);
     path2.addRect(100, 100, 1, 530);

     painter1.setRenderHint(QPainter::Antialiasing, true);
     painter1.fillPath(path2, QBrush(QColor(192, 192, 192,255)));

    QColor color(255, 255, 255, 50);
    for(int i=0; i<2; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(2-i, 2-i, this->width()-(2-i)*2, this->height()-(2-i)*2);
        color.setAlpha(150 - sqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::WindowActivate)//窗口最小化还原后重绘整个窗口
    {
        this->update();
        //qDebug()<<"update";
    }
    if(obj == ui->widget_title || obj == ui->title)
    {
        if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove)
        {
            QMouseEvent *mouEvent = (QMouseEvent *)event;
            if(event->type() == QEvent::MouseButtonPress)
            {
                clickPos = mapToGlobal(mouEvent->pos());//当前鼠标位置
                widgetPos = mapToGlobal(QPoint(0,0));//当前窗口位置
                //qDebug()<<widgetPos;
            }
            if(event->type() == QEvent::MouseMove)
            {
                this->move(widgetPos + mouEvent->globalPos() - clickPos);//窗口位置+鼠标移动位置之差=窗口新位置
            }
        }

    }

    if(obj == ui->play_widget)
    {
        if(event->type() == QEvent::Enter)
        {
            ui->play_widget->setStyleSheet("#play_widget{background-color: rgba(0, 0, 0, 100)}");
            ui->bt_shot->show();
            ui->bt_show->show();
            ui->bt_stop->show();
            ui->bt_full->show();
            widgetTimer->stop();

        }
        else if(event->type() == QEvent::Leave)
        {
            widgetTimer->start(1000);
            connect(widgetTimer,SIGNAL(timeout()),this,SLOT(slot_widgettimeout()));

        }
    }

    if(obj == ui->tem && event->type() == QEvent::Paint)
    {
        QPainter painter(ui->tem);
        unsigned char i;
        painter.setPen(Qt::red);//设置画笔样式
        painter.setRenderHint(QPainter::Antialiasing, true);
        //painter.setBrush(Qt::red);//设置填充样式
        for(i=0;i<8;i++)
        {
            painter.drawLine(QPointF((ui->tem->width())/2 + i,
                                     ui->tem->height()-15),
                             QPointF((ui->tem->width())/2 + i,
                                     (ui->tem->height()-15)*((100-Tem)/100)));
        }
       // painter.drawRect(40,130,9,131*(Num/100));
    }
    if(obj == ui->hd && event->type() == QEvent::Paint)
    {
        QPainter painter(ui->hd);
        unsigned char i;
        painter.setPen(Qt::red);//设置画笔样式
        painter.setRenderHint(QPainter::Antialiasing, true);
      //  painter.setBrush(Qt::red);//设置填充样式
        for(i=0;i<8;i++)
        {
            painter.drawLine(QPointF((ui->hd->width())/2 + i,
                                     ui->hd->height()-15),
                             QPointF((ui->hd->width())/2 + i,
                                     (ui->hd->height()-15)*((100-Hd)/100)));
        }
       // painter.drawRect(40,130,9,131*(Num/100));
    }
    if(obj == ui->MQ4 && event->type() == QEvent::Paint)
    {
        QPainter painter(ui->MQ4);
        unsigned char i;
        painter.setPen(Qt::red);//设置画笔样式
        painter.setRenderHint(QPainter::Antialiasing, true);
       // painter.setBrush(Qt::red);//设置填充样式
        for(i=0;i<8;i++)
        {
            painter.drawLine(QPointF((ui->MQ4->width())/2 + i,
                                     ui->MQ4->height()-15),
                             QPointF((ui->MQ4->width())/2 + i,
                                     (ui->MQ4->height()-15)*((100-MQ4)/100)));
        }
       // painter.drawRect(40,130,9,131*(Num/100));
    }
    if(obj == ui->MQ9 && event->type() == QEvent::Paint)
    {
        QPainter painter(ui->MQ9);
        unsigned char i;
        painter.setPen(Qt::red);//设置画笔样式
        painter.setRenderHint(QPainter::Antialiasing, true);
       // painter.setBrush(Qt::red);//设置填充样式
        for(i=0;i<8;i++)
        {
            painter.drawLine(QPointF((ui->MQ9->width())/2 + i,
                                     ui->MQ9->height()-15),
                             QPointF((ui->MQ9->width())/2 + i,
                                     (ui->MQ9->height()-15)*((100-MQ9)/100)));
        }
       // painter.drawRect(40,130,9,131*(Num/100));
    }

    if(obj == ui->tem_widget && event->type() == QEvent::Paint)
    {
        QPainter painter(ui->tem_widget);
        mPainter->setAxes(20,30,ui->MQ9_widget->width()-10,
                          ui->MQ9_widget->height()-20);
        mPainter->setCurve2(false);
        mPainter->setY_axes(40);
        mPainter->setDataBuff(tem_curvebuff,temBuffer2);
        mPainter->setTitle("Temperature");
        mPainter->setPainter(&painter);
        mPainter->paint();
    }
    if(obj == ui->hd_widget && event->type() == QEvent::Paint)
    {
        QPainter painter(ui->hd_widget);
        mPainter->setTitle("Humidity ");
        mPainter->setY_axes(40);
        mPainter->setDataBuff(hd_curvebuff,temBuffer2);
        mPainter->setPainter(&painter);
        mPainter->paint();
    }
    if(obj == ui->MQ4_widget && event->type() == QEvent::Paint)
    {
        QPainter painter(ui->MQ4_widget);
        mPainter->setTitle("MQ4");
        mPainter->setY_axes(40);
        mPainter->setDataBuff(mq4_curvebuff,temBuffer);
        mPainter->setPainter(&painter);
        mPainter->paint();
    }
    if(obj == ui->MQ9_widget && event->type() == QEvent::Paint)
    {
        QPainter painter(ui->MQ9_widget);
        mPainter->setDataBuff(mq9_curvebuff,mq9_curvebuff);
        mPainter->setTitle("Hcsr04");
        mPainter->setY_axes(80);
        mPainter->setPainter(&painter);
        mPainter->paint();
    }

    return QWidget::eventFilter(obj,event);
}
void MainWindow::slot_widgettimeout()
{
    ui->bt_shot->hide();
    ui->bt_show->hide();
    ui->bt_stop->hide();
    ui->bt_full->hide();
    ui->play_widget->setStyleSheet("#play_widget{background-color: rgba(0, 0, 0, 0)}");
    widgetTimer->stop();
}
void MainWindow::datasend(char d)
{
    QByteArray data;
    data[0]=0xff;
    data[1]=0x00;
    data[2]=d;
    data[3]=0x00;
    data[4]=data[1]+data[2]+data[3];
    data[5]=0xff;
    if(network->SendData(data)==true)
    {
        ui->textBr_mess->append(" ddd  data send successful");
    }
    else
    {
        ui->textBr_mess->append(" ddd  data send failed");
        network->SendData(data);
    }
}
void MainWindow::keyPressEvent(QKeyEvent  *event)
{
    if(event->key()==Qt::Key_W)
    {
       datasend(0x01);
       ui->textBr_mess->append("there is a keyPressEvent Q");
    }
    else if(event->key()==Qt::Key_S)
    {
       datasend(0x02);
       ui->textBr_mess->append("there is a keyPressEvent Q");
    }
    else if(event->key()==Qt::Key_A)
    {
       datasend(0x04);
       ui->textBr_mess->append("there is a keyPressEvent Q");
    }
    else if(event->key()==Qt::Key_D)
    {
       datasend(0x03);
       ui->textBr_mess->append("there is a keyPressEvent Q");

    }
    else if(event->key() == Qt::Key_F11)
    {
        ui->curve_widget->setWindowFlags(Qt::Dialog);
        ui->curve_widget->showFullScreen();
        //this->activateWindow();
    }
    else if(event->key() == Qt::Key_Escape)
    {
     //   ui->textBr_mess->append("there is a keyPressEvent ESC");
        if(ui->widget_2->isFullScreen() == true)
        {
            ui->widget_2->showNormal();
            ui->widget_2->setWindowFlags(Qt::SubWindow);
            ui->widget_2->resize(371,331);
            ui->widget_2->move(730,130);

            ui->movie_label->resize(ui->widget_2->size());
            ui->play_widget->resize(ui->movie_label->width(),41);
            ui->play_widget->move(0,ui->movie_label->height()-ui->play_widget->height());
            ui->bt_full->move(ui->play_widget->width()-40,6);
            ui->widget_2->show();
            emit upDateButton();
        }
        if(ui->curve_widget->isFullScreen() == true)
        {
            ui->curve_widget->setWindowFlags(Qt::SubWindow);
            ui->curve_widget->resize(591,481);
            ui->curve_widget->move(20,10);
            ui->curve_widget->show();
        }
    }


}
void MainWindow::keyReleaseEvent(QKeyEvent  *event)
{
    if(event->key()==Qt::Key_W)
    {
      if(event->isAutoRepeat()) return;
      datasend(0x00);
      ui->textBr_mess->append("there is a keyReleaseEvent Q");
    }
    if(event->key()==Qt::Key_S)
    {
      if(event->isAutoRepeat()) return;
      datasend(0x00);
      ui->textBr_mess->append("there is a keyReleaseEvent Q");
    }
    if(event->key()==Qt::Key_A)
    {
      if(event->isAutoRepeat()) return;
      datasend(0x00);
      ui->textBr_mess->append("there is a keyReleaseEvent Q");
    }
    if(event->key()==Qt::Key_D)
    {
      if(event->isAutoRepeat()) return;
      datasend(0x00);
      ui->textBr_mess->append("there is a keyReleaseEvent Q");
    }
}

void MainWindow::bt_show_clicked()
{
    if(false == camera->isCameraOn())
    {
      //  QObject::connect(camera,SIGNAL(CameraDataReceived(QPixmap*)),this,SLOT(CameraDataReceived(QPixmap*)));
        camera->Show();
        QObject::connect(camera,SIGNAL(signal_frameRrate(unsigned char)),
                         this,SLOT(Cam_frameRate(unsigned char)));
        QObject::connect(camera,SIGNAL(cameraError()),this,SLOT(bt_stopPic()));
        ui->bt_show->setStyleSheet("background-color:transparent;");
        ui->bt_show->setStyleSheet("QPushButton{border-image:url(:/button/button/zanting1.png);}\
                                   QPushButton:hover{border-image: url(:/button/button/zanting1.png);}\
                                   QPushButton:pressed{border-image: url(:/button/button/zanting1.png);}");
        ui->bt_show->setToolTip(tr("暂停"));

    }
    else if(true == camera->isCameraOn())
    {
        camera->Stop();
        ui->bt_show->setStyleSheet("background-color:transparent;");
        ui->bt_show->setStyleSheet("QPushButton{border-image:url(:/button/button/zanting2.png);}\
                                   QPushButton:hover{border-image: url(:/button/button/zanting2.png);}\
                                   QPushButton:pressed{border-image: url(:/button/button/zanting2.png);}");
        ui->bt_show->setToolTip(tr("开始"));
    }
}

void MainWindow::bt_stopPic()
{
    camera->Stop();
    ui->bt_show->setStyleSheet("background-color:transparent;");
    ui->bt_show->setStyleSheet("QPushButton{border-image:url(:/button/button/zanting2.png);}\
                               QPushButton:hover{border-image: url(:/button/button/zanting2.png);}\
                               QPushButton:pressed{border-image: url(:/button/button/zanting2.png);}");
    ui->bt_show->setToolTip(tr("开始"));
    QPixmap pixmap = QPixmap(":/image/image/9.png");
    ui->movie_label->setPixmap(pixmap);
    ui->movie_label->setScaledContents(true);
}
/************************************
    属性：槽函数
    功能：图像数据帧率接收
    参数： frame_rate 当前图像帧率
*************************************/
void MainWindow::Cam_frameRate(unsigned char frame_rate)
{
    ui->frame_label->setNum((double)frame_rate);
}

void MainWindow::bt_shot_clicked()
{
    camera->shot();
}

void MainWindow::quit()
{
    QApplication* app;
    app->quit();
}

void MainWindow::bt_reset_qx()
{
    ui->lineEdit_q1->setText("0");
    ui->lineEdit_q2->setText("0");
    ui->lineEdit_q3->setText("0");
    ui->lineEdit_q4->setText("0");
    ui->lineEdit_q5->setText("0");
    ui->lineEdit_q6->setText("0");
}

void MainWindow::bt_send_qx()
{
    char q0,q00,q1,q2,q3,q4,q5,q6,q7;
    char q10,q20,q30,q40,q50,q60;
    unsigned char sum;
    int radio_num;
    QByteArray data;
     QString str = (ui->lineEdit_q1->text()).toUtf8();
     ui->textBr_mess->append(str);
     q1=(char)str.toFloat();
     str.clear();
     str = (ui->lineEdit_q2->text()).toUtf8();
     q2=(char)str.toFloat();
     str.clear();
     str = (ui->lineEdit_q3->text()).toUtf8();
     q3=(char)str.toFloat();
     str.clear();
     str = (ui->lineEdit_q4->text()).toUtf8();
     q4=(char)str.toFloat();
     str.clear();
     str = (ui->lineEdit_q5->text()).toUtf8();
     q5=(char)str.toFloat();
     str.clear();
     str = (ui->lineEdit_q6->text()).toUtf8();
     q6=(char)str.toFloat();
     str.clear();

     q0=0xff;
     q00=0xaa;
     q7=0xff;
     radio_num = getThreeFucSelect.checkedId();
     if(1 == radio_num)
     {
         qDebug("radioButton_2 isdown");
         q10=ui->verticalScrollBar_q1->value();
         q20=ui->verticalScrollBar_q2->value();
         q30=ui->verticalScrollBar_q3->value();
         q40=ui->verticalScrollBar_q4->value();
         q50=ui->verticalScrollBar_q5->value();
         q60=ui->verticalScrollBar_q6->value();
     }
     else if(0 == radio_num)
     {
         qDebug("radioButton isdown");
         q10=ui->verticalScrollBar_q_all->value();
         q20=ui->verticalScrollBar_q_all->value();
         q30=ui->verticalScrollBar_q_all->value();
         q40=ui->verticalScrollBar_q_all->value();
         q50=ui->verticalScrollBar_q_all->value();
         q60=ui->verticalScrollBar_q_all->value();
     }
     else
     {
         q10=30;
         q20=30;
         q30=30;
         q40=30;
         q50=30;
         q60=30;
     }
     sum=q00+q1+q10+q2+q20+q3+q30+q4+q40+q5+q50+q6+q60;
     if(sum==0xff)
     {
         sum=0xfe;
     }
     data.append(q0);
     data.append(q00);
     data.append(q1);
     data.append(q10);
     data.append(q2);
     data.append(q20);
     data.append(q3);
     data.append(q30);
     data.append(q4);
     data.append(q40);
     data.append(q5);
     data.append(q50);
     data.append(q6);
     data.append(q60);
     data.append(sum);
     data.append(q7);
     if(network->SendData(data)==true)
     {
         ui->textBr_mess->append("data send successful");
       //  ui->textBr_mess->append("data lingth:"+QString("%1").arg(data.length()));
         ui->textBr_mess->append("data:"+data.toHex());
     }
     else
     {
         ui->textBr_mess->append("data send failed");
     }

     step1=false;
     step2=false;
     step3=false;
     step4=false;
     step5=false;

}

void MainWindow::Grab()
{
    QByteArray wdata;
    unsigned char data[10];

    if(step1)
    {
        data[0]=0xff; data[1]=0xaa; data[2]=20; data[3]=0; data[4]=30; data[5]=110;
        data[6]=0; data[7]=0;
        data[8]=data[1]+data[2]+data[3]+data[4]+data[5]+data[6]+data[7];
        if(data[8]==0xff) data[8]=0xfe;
        data[9]=0xff;
        wdata.append(data[0]);
        wdata.append(data[1]);
        wdata.append(data[2]);
        wdata.append(data[3]);
        wdata.append(data[4]);
        wdata.append(data[5]);
        wdata.append(data[6]);
        wdata.append(data[7]);
        wdata.append(data[8]);
        wdata.append(data[9]);
        network->SendData(wdata);
        GrabTimer->start(13000);
        qDebug("Grab A");
    }
    //抓
    else if(step2)
    {
        data[0]=0xff; data[1]=0xaa; data[2]=20; data[3]=0; data[4]=30; data[5]=110;
        data[6]=0; data[7]=70;
        data[8]=data[1]+data[2]+data[3]+data[4]+data[5]+data[6]+data[7];
        if(data[8]==0xff) data[8]=0xfe;
        data[9]=0xff;
        wdata.append(data[0]);
        wdata.append(data[1]);
        wdata.append(data[2]);
        wdata.append(data[3]);
        wdata.append(data[4]);
        wdata.append(data[5]);
        wdata.append(data[6]);
        wdata.append(data[7]);
        wdata.append(data[8]);
        wdata.append(data[9]);
        network->SendData(wdata);
        GrabTimer->start(1500);
        qDebug("Grab B");
    }
    else if(step3)
    {
        data[0]=0xff; data[1]=0xaa; data[2]=20; data[3]=0; data[4]=30; data[5]=110;
        data[6]=180; data[7]=60;
        data[8]=data[1]+data[2]+data[3]+data[4]+data[5]+data[6]+data[7];
        if(data[8]==0xff) data[8]=0xfe;
        data[9]=0xff;
        wdata.append(data[0]);
        wdata.append(data[1]);
        wdata.append(data[2]);
        wdata.append(data[3]);
        wdata.append(data[4]);
        wdata.append(data[5]);
        wdata.append(data[6]);
        wdata.append(data[7]);
        wdata.append(data[8]);
        wdata.append(data[9]);
        network->SendData(wdata);
        GrabTimer->start(2500);
        qDebug("Grab C");
    }
    else if(step4)
    {
        data[0]=0xff; data[1]=0xaa; data[2]=20; data[3]=180; data[4]=130; data[5]=0;
        data[6]=30; data[7]=60;
        data[8]=data[1]+data[2]+data[3]+data[4]+data[5]+data[6]+data[7];
        if(data[8]==0xff) data[8]=0xfe;
        data[9]=0xff;
        wdata.append(data[0]);
        wdata.append(data[1]);
        wdata.append(data[2]);
        wdata.append(data[3]);
        wdata.append(data[4]);
        wdata.append(data[5]);
        wdata.append(data[6]);
        wdata.append(data[7]);
        wdata.append(data[8]);
        wdata.append(data[9]);
        network->SendData(wdata);
        GrabTimer->start(15000);
        qDebug("Grab D");
    }
    //放
    else if(step5)
    {
        data[0]=0xff; data[1]=0xaa; data[2]=20; data[3]=180; data[4]=130; data[5]=0;
        data[6]=30; data[7]=0;
        data[8]=data[1]+data[2]+data[3]+data[4]+data[5]+data[6]+data[7];
        if(data[8]==0xff) data[8]=0xfe;
        data[9]=0xff;
        wdata.append(data[0]);
        wdata.append(data[1]);
        wdata.append(data[2]);
        wdata.append(data[3]);
        wdata.append(data[4]);
        wdata.append(data[5]);
        wdata.append(data[6]);
        wdata.append(data[7]);
        wdata.append(data[8]);
        wdata.append(data[9]);
        network->SendData(wdata);
        step4=false;
        qDebug("Grab E");
        //GrabTimer->start(15000);
    }
}

void MainWindow::grabtimeout()
{
    if(step1)
    {
        step1=false;
        step2=true;
        step3=false;
        step4=false;
        step5=false;
    }
    else if(step2)
    {
        step1=false;
        step2=false;
        step3=true;
        step4=false;
        step5=false;
    }
    else if(step3)
    {
        step1=false;
        step2=false;
        step3=false;
        step4=true;
        step5=false;
    }
    else if(step4)
    {
        step1=false;
        step2=false;
        step3=false;
        step4=false;
        step5=true;
    }
    Grab();
}

void MainWindow::bt_clear()
{
    ui->textBr_mess->clear();
}

void MainWindow::caculate()
{
    QByteArray data;
    getdataout(&data);
}

void MainWindow::DataReceived(QByteArray buff)
{
    unsigned char* s = (unsigned char*)buff.data();
    if(0xa1 == *s)
    {
        ui->mq4_label->setNum(buff[1]);
        ui->mq9_label->setNum(buff[2]);
        MQ4 = buff[1];
        MQ9 = buff[2];
        ui->MQ4->update();
        ui->MQ9->update();
    }
    else if(0xa2 == *s)
    {
        ui->tem_label->setNum(buff[1]);
        ui->hd_label->setNum(buff[2]);
        Tem = buff[1];
        Hd = buff[2];
        ui->tem->update();
        ui->hd->update();
    }
    else if(0xa3 == *s)
    {
        ui->label_q1->setNum(*(s+1));
        ui->label_q2->setNum(*(s+2));
        ui->label_q3->setNum(*(s+3));
        ui->label_q4->setNum(*(s+4));
        ui->label_q5->setNum(*(s+5));
        ui->label_q6->setNum(*(s+6));
    }
    else
    {
       // ui->mq4_label->setText("wiating data");
       // ui->mq9_label->setText("wiating data");
       // ui->tem_label->setText("wiating data");
       // ui->hd_label->setText("wiating data");
    }
   // ui->textBr_mess->append("received data: "+buff.toHex());
}

void MainWindow::btsend()
{
    QByteArray data;
    char q0,q00,q1,q2,q3,q4,q5,q6,q7;
    char q10,q20,q30,q40,q50,q60;
    unsigned char sum;
    int radio_num;

    getdataout(&data);//获取用户输入的数据
    q1=data[0];
    q2=data[1];
    q3=data[2];
    q4=data[3];
    q5=data[4];
    q6=data[5];

    q0=0xff;
    q00=0xaa;
    q7=0xff;
    radio_num = getThreeFucSelect.checkedId();
    if(1 == radio_num)
    {
        qDebug("radioButton_2 isdown");
        q10=ui->verticalScrollBar_q1->value();
        q20=ui->verticalScrollBar_q2->value();
        q30=ui->verticalScrollBar_q3->value();
        q40=ui->verticalScrollBar_q4->value();
        q50=ui->verticalScrollBar_q5->value();
        q60=ui->verticalScrollBar_q6->value();
    }
    else if(0 == radio_num)
    {
        qDebug("radioButton isdown");
        q10=ui->verticalScrollBar_q_all->value();
        q20=ui->verticalScrollBar_q_all->value();
        q30=ui->verticalScrollBar_q_all->value();
        q40=ui->verticalScrollBar_q_all->value();
        q50=ui->verticalScrollBar_q_all->value();
        q60=ui->verticalScrollBar_q_all->value();
    }
    else
    {
        q10=30;
        q20=30;
        q30=30;
        q40=30;
        q50=30;
        q60=30;
    }
    sum=q00+q1+q10+q2+q20+q3+q30+q4+q40+q5+q50+q6+q60;
    if(sum==0xff)
    {
        sum=0xfe;
    }
    data.append(q0);
    data.append(q00);
    data.append(q1);
    data.append(q10);
    data.append(q2);
    data.append(q20);
    data.append(q3);
    data.append(q30);
    data.append(q4);
    data.append(q40);
    data.append(q5);
    data.append(q50);
    data.append(q6);
    data.append(q60);
    data.append(sum);
    data.append(q7);
    if(network->SendData(data)==true)
    {
        ui->textBr_mess->append("data send successful");
      //  ui->textBr_mess->append("data lingth:"+QString("%1").arg(data.length()));
        ui->textBr_mess->append("data:"+data.toHex());
    }
    else
    {
        ui->textBr_mess->append("data send failed");
    }
}

void MainWindow::newConnect()
{
    if(false == network->isNetworkOn())
    {
        if(network->Connect()==true)
        {
            ui->textBr_mess->append("connect sucessful!");
            connect(network,SIGNAL(netWorkerror()),this,SLOT(updateButton()));
            connect(network,SIGNAL(DataReceived(QByteArray)),this,SLOT(DataReceived(QByteArray)));
         //   isConnect=true;
        }
        else
        {
            ui->textBr_mess->append("connect failed !");
         //   isConnect=false;
        }
    }
    else if(true == network->isNetworkOn())
    {
        network->DisConnect();
        ui->textBr_mess->append("abort connect!");
      //  isConnect=false;
    }
    emit upDateButton();
}

void MainWindow::updateButton()
{
    if(true == network->isNetworkOn())
    {
       ui->bt_connect->setStyleSheet("QPushButton{border-image:url(:/button/button/button3.png);}\
                          QPushButton:hover{border-image: url(:/button/button/button3.png);}\
                          QPushButton:pressed{border-image: url(:/button/button/button31.png);}" );
       ui->bt_connect->setToolTip(tr("已连接，点击取消连接！"));
    }
    else if(false == network->isNetworkOn())
    {

       ui->bt_connect->setStyleSheet("QPushButton{border-image:url(:/button/button/button1.png);}\
                          QPushButton:hover{border-image: url(:/button/button/button1.png);}\
                          QPushButton:pressed{border-image: url(:/button/button/button11.png);}");
       ui->bt_connect->setToolTip(tr("未连接，点击建立连接！"));

       ui->mq4_label->setText("wiating data");
       ui->mq9_label->setText("wiating data");
       ui->tem_label->setText("wiating data");
       ui->hd_label->setText("wiating data");
       Tem = 0;
       Hd = 0;
       MQ4 = 0;
       MQ9 = 0;
      // ui->tem->update();
      // ui->hd->update();
      // ui->MQ4->update();
      // ui->MQ9->update();
    }
    if(true == ui->widget_2->isFullScreen())
    {
        ui->bt_full->setStyleSheet("background-color:transparent;");
        ui->bt_full->setStyleSheet("QPushButton{border-image:url(:/button/button/quanping2.png);}\
                                   QPushButton:hover{border-image: url(:/button/button/quanping2.png);}\
                                   QPushButton:pressed{border-image: url(:/button/button/quanping2.png);}");
        ui->bt_full->setToolTip(tr("退出全屏"));
        qDebug("fullscreen");
    }
    else if(false == ui->widget_2->isFullScreen())
    {
        ui->bt_full->setStyleSheet("background-color:transparent;");
        ui->bt_full->setStyleSheet("QPushButton{border-image:url(:/button/button/quanping1.png);}\
                                   QPushButton:hover{border-image: url(:/button/button/quanping1.png);}\
                                   QPushButton:pressed{border-image: url(:/button/button/quanping1.png);}");
        ui->bt_full->setToolTip(tr("全屏"));
        qDebug("quit fullscreen");
    }
}
/*
void MainWindow::caculate_Inverse()
{

}
*/
//获取用户输入的将要发送的数据，并转换为需要的数据格式 返回到指针 data 中。
void MainWindow::getdataout(QByteArray *data)
{
    QByteArray px1,py1,pz1,qn,qo,qa;
    char q01,q001,q11,q21,q31,q41,q51,q61,q511;
    float a2,a3,
            px,py,pz,gn,go,ga,
            c3,s3,
            q1,q2,q3,q4,q5,q234,
            ax,ay,az,ox,oy,oz,nx,ny,nz;
    QString sstr;
    data->clear();
    QString str = (ui->lineEdit_px->text().toUtf8());
    px1=str.toUtf8();
    sstr.append("px:"+px1+" ");
    px=str.toFloat();
    str.clear();
    str = (ui->lineEdit_py->text()).toUtf8();
    py1=str.toUtf8();
    sstr.append("py:"+py1+" ");
    py=str.toFloat();
    str.clear();
    str = (ui->lineEdit_pz->text()).toUtf8();
    pz1=str.toUtf8();
    sstr.append("pz:"+pz1+" ");
    pz=str.toFloat();
    str.clear();
    str = (ui->lineEdit_qn->text()).toUtf8();
    qn=str.toUtf8();
    sstr.append("qn:"+qn+" ");
    gn=str.toFloat();
    str.clear();
    str = (ui->lineEdit_qo->text()).toUtf8();
    qo=str.toUtf8();
    sstr.append("qo:"+qo+" ");
    go=str.toFloat();
    str.clear();
    str = (ui->lineEdit_qa->text()).toUtf8();
    qa=str.toUtf8();
    sstr.append("qa:"+qa+" ");
    ga=str.toFloat();
    str.clear();

    ui->textBr_mess->append(sstr);
    sstr.clear();

    //在此处添加逆解相关程序 以及显示程序
    //////////////////////////////////////////////////////////////////////////////////////
        //计算逆解过程
        a2 = 100;
        a3 = 150;
        // RPY转换
        nx = cos(ga)*cos(go);
        ny = sin(ga)*cos(go);
        nz = -sin(go);
        ox = cos(ga)*sin(go)*sin(gn) - sin(ga)*cos(gn);
        oy = sin(ga)*sin(go)*sin(gn) + cos(ga)*cos(gn);
        oz = cos(go)*sin(gn);
        ax = cos(ga)*sin(go)*cos(gn) + sin(ga)*sin(gn);
        ay = sin(ga)*sin(go)*cos(gn) - cos(ga)*sin(gn);
        az = cos(go)*cos(gn);

        //逆解求解

       q1=atan2(py,px)*(180/pi);
       q234=atan2(az,ax*cos(q1)+ay*sin(q1))*(180/pi);
       c3=((px*cos(q1)+py*sin(q1))*(px*cos(q1)+py*sin(q1))
           +pz*pz-a2*a2-a3*a3)/(2*a2*a3);
       if(c3>1 || c3<-1)
       {
           ui->textBr_mess->append("逆解不存在，返回");
           qDebug("%.3f",c3);
           return;
       }
       s3=-sqrt(1-c3*c3);
       q3=atan2(s3,c3)*(180/pi);
       q2=atan2(((pz)*(a3*c3+a2)-(a3*s3*(px*cos(q1)+py*sin(q1)))),
                 ((px*cos(q1)+py*sin(q1))*(a3*c3+a2)+a3*s3*(pz)))*(180/pi);
       q4=q234-q2-q3;
       q5=atan2((nx*sin(q1)-ny*cos(q1)),(ox*sin(q1)-oy*cos(q1)))*(180/pi);


   // q01=0xff;
  //  q001=0xaa;
    q11=(char)q1;
    q21=(char)q2;
    q31=(char)q3;
    q41=(char)q4;
    q51=(char)q5;
    q61=30;
   // q61=0xff;

  //  data->append(q01);
  //  data->append(q001);
    data->append(q11);
    sstr.append("q1: "+QString::number(q11).toUtf8());
    data->append(q21);
    sstr.append(" q2: "+QString::number(q21));
    data->append(q31);
    sstr.append(" q3: "+QString::number(q31));
    data->append(q41);
    sstr.append(" q4: "+QString::number(q41));
    data->append(q51);
    sstr.append(" q5: "+QString::number(q51));
    data->append(q61);
    ui->textBr_mess->append(sstr);
    sstr.clear();
}

/*  chiledialog 相关函数     */
void MainWindow::bt_intnet_button()
{
    chileDialog dialog;
    QObject::connect(&dialog,SIGNAL(SendCameraText(QString*)),camera,SLOT(CameraDataUpDate(QString*)));
    QObject::connect(&dialog,SIGNAL(SendText(QString*,quint16*)),network,SLOT(NetworkDataUpDate(QString*,quint16*)));
    dialog.exec();
}

/*  Cardialog 相关函数     */
void MainWindow::bt_car_button()
{
    CarDialog dialog;
    dialog.GetMainWindow(this);
   // QObject::connect(&dialog,SIGNAL(SendText(QString,quint16)),this,SLOT(intnet_enter_clicked(QString,quint16)));
    dialog.exec();
}

/*  ManPuDialog 相关函数     */
void MainWindow::bt_manipulator_button()
{
    ManPuDialog dialog;
    dialog.GetMainWindow(this);
   // QObject::connect(&dialog,SIGNAL(SendText(QString,quint16)),this,SLOT(intnet_enter_clicked(QString,quint16)));
    dialog.exec();
}

/*  tthersDialog 相关函数     */
void MainWindow::bt_others_button()
{
    tthersDialog dialog;
    dialog.GetMainWindow(this);
   // QObject::connect(&dialog,SIGNAL(SendText(QString,quint16)),this,SLOT(intnet_enter_clicked(QString,quint16)));
    dialog.exec();
}

void MainWindow::on_bt_high_clicked()
{
    QByteArray data;
    data[0]=0xff;
    data[1]=0x02;
    data[2]=0x01;
    data[3]=0x0a;
    data[4]=0xff;
    if(network->SendData(data)==true)
    {
        ui->textBr_mess->append(" speed  data send successful");
    }
    else
    {
        ui->textBr_mess->append(" speed  data send failed");
        network->SendData(data);
    }
}

void MainWindow::on_bt_low_clicked()
{
    QByteArray data;
    data[0]=0xff;
    data[1]=0x02;
    data[2]=0x01;
    data[3]=0x00;
    data[4]=0xff;
    if(network->SendData(data)==true)
    {
        ui->textBr_mess->append(" speed  data send successful");
    }
    else
    {
        ui->textBr_mess->append(" speed  data send failed");
        network->SendData(data);
    }
}

void MainWindow::timesomething()
{
    setBuff((int)MQ4 , mq4_curvebuff);
    setBuff((int)MQ9 , mq9_curvebuff);
    setBuff((int)Tem , tem_curvebuff);
    setBuff((int)Hd , hd_curvebuff);
    ui->MQ4_widget->update();
    ui->MQ9_widget->update();
    ui->tem_widget->update();
    ui->hd_widget->update();
}

void MainWindow::setBuff(int val , char *buff)
{
    unsigned char i;
    if(val != 0)
    {
        for(i=99;i>0;i--)
        {
            buff[i] = buff[i-1];
        }
        buff[0] = val;
    //    ui->tem_widget->update();
    }
}

void MainWindow::on_bt_full_clicked()
{
    if(ui->widget_2->isFullScreen() == false)
    {

     ui->widget_2->setWindowFlags(Qt::Dialog);
     ui->widget_2->showFullScreen();

     ui->movie_label->resize(ui->widget_2->size());
     ui->play_widget->resize(ui->movie_label->width(),41);
     ui->play_widget->move(0,ui->movie_label->height()-ui->play_widget->height());
     ui->bt_full->move(ui->play_widget->width()-40,12);
     emit upDateButton();
   //   this->activateWindow();
    }
    else
    {
        ui->widget_2->showNormal();
        ui->widget_2->setWindowFlags(Qt::SubWindow);
        ui->widget_2->resize(371,331);
        ui->widget_2->move(730,130);

        ui->movie_label->resize(ui->widget_2->size());
        ui->play_widget->resize(ui->movie_label->width(),41);
        ui->play_widget->move(0,ui->movie_label->height()-ui->play_widget->height());
        ui->bt_full->move(ui->play_widget->width()-40,12);
        ui->widget_2->show();
        emit upDateButton();
    }
}

void MainWindow::on_pushButton_clicked()
{
    step1 = true;

    Grab();

}

void MainWindow::slot_ScrollBarchanged(int value)
{
    //qDebug("slot_ScrollBarchanged");
    ui->lineEdit_q1_2->setText(QString::number(ui->verticalScrollBar_q1->value(),10));
    ui->lineEdit_q2_2->setText(QString::number(ui->verticalScrollBar_q2->value(),10));
    ui->lineEdit_q3_2->setText(QString::number(ui->verticalScrollBar_q3->value(),10));
    ui->lineEdit_q4_2->setText(QString::number(ui->verticalScrollBar_q4->value(),10));
    ui->lineEdit_q5_2->setText(QString::number(ui->verticalScrollBar_q5->value(),10));
    ui->lineEdit_q6_2->setText(QString::number(ui->verticalScrollBar_q6->value(),10));
    ui->lineEdit_q_all->setText(QString::number(ui->verticalScrollBar_q_all->value(),10));
}

void MainWindow::slot_lineEditchanged()
{
   // qDebug("slot_lineEditchanged");
    ui->verticalScrollBar_q1->setValue(ui->lineEdit_q1_2->text().toUtf8().toInt());
    ui->verticalScrollBar_q2->setValue(ui->lineEdit_q2_2->text().toUtf8().toInt());
    ui->verticalScrollBar_q3->setValue(ui->lineEdit_q3_2->text().toUtf8().toInt());
    ui->verticalScrollBar_q4->setValue(ui->lineEdit_q4_2->text().toUtf8().toInt());
    ui->verticalScrollBar_q5->setValue(ui->lineEdit_q5_2->text().toUtf8().toInt());
    ui->verticalScrollBar_q6->setValue(ui->lineEdit_q6_2->text().toUtf8().toInt());
    ui->verticalScrollBar_q_all->setValue(ui->lineEdit_q_all->text().toUtf8().toInt());
}


void MainWindow::on_pushButton_2_clicked()
{
    qDebug("hahahahahahaah ahahaah !");
}
