
#include "ui_chiledialog.h"
#include "mainwindow.h"
chileDialog::chileDialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::chileDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);  //隐藏窗体边框
    this->setAttribute(Qt::WA_TranslucentBackground);  //设置窗体透明效果
    this->installEventFilter(this);
    int wide = width();//获取界面的宽度
    //构建最小化、最大化、关闭按钮
    QToolButton *closeButton= new QToolButton(this);
    //获取最小化、关闭按钮图标
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    //设置最小化、关闭按钮图标
    closeButton->setIcon(closePix);
    //设置最小化、关闭按钮在界面的位置
    closeButton->setGeometry(wide-30,6,20,20);
    //设置鼠标移至按钮上的提示信息
    closeButton->setToolTip(tr("关闭"));
    //设置最小化、关闭按钮的样式
    closeButton->setStyleSheet("background-color:transparent;");
    connect(closeButton,SIGNAL(clicked()),this,SLOT(bt_cancel_clicked()));
    connect(ui->bt_cancel,SIGNAL(clicked()),this,SLOT(bt_cancel_clicked()));
    connect(ui->bt_enter,SIGNAL(clicked()),this,SLOT(bt_enter_clicked()));
}

chileDialog::~chileDialog()
{
    delete ui;
}

void chileDialog::GetMainWindow(MainWindow *w)
{
    mainwindow=w;    //获取主窗口指针
}

void chileDialog::bt_enter_clicked()
{
    QString hostname = ui->hostLineEdit->text();
    QString cameraaddr = ui->cameraLineEdit->text();
    quint16 port = ui->portLineEdit->text().toInt();
    emit SendText(&hostname,&port);
    emit SendCameraText(&cameraaddr);
    this->close();
}

void chileDialog::bt_cancel_clicked()
{
   // QObject::disconnect(this,SIGNAL(SendText(QString,quint16)),mainwindow,SLOT(intnet_enter_clicked(QString,quint16)));
    this->close();
}

void chileDialog::paintEvent(QPaintEvent *)
{
     QPixmap pixmap = QPixmap(":/image/image/9.jpg").scaled(this->size());
     QPainter painter(this);
     painter.drawPixmap(this->rect(), pixmap);

///////////
     QPainterPath path;
     path.setFillRule(Qt::WindingFill);
     path.addRect(2, 30, this->width()-2, this->height()-2);

     QPainter painter1(this);
     painter1.setRenderHint(QPainter::Antialiasing, true);
     painter1.fillPath(path, QBrush(QColor(255, 255, 255,240)));


    QColor color(255, 255, 255, 50);
    for(int i=0; i<1; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(1-i, 1-i, this->width()-(1-i)*2, this->height()-(1-i)*2);
        color.setAlpha(150 - sqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

bool chileDialog::eventFilter(QObject *obj, QEvent *event)
{
  //  if(event->type() == QEvent::WindowActivate)//窗口最小化还原后重绘整个窗口
 //   {
 //       this->repaint();
        //qDebug()<<"repaint";
 //   }
  /*  if(obj == ui->tableView_patModel)//选择患者后出现添加测试按钮。
    {
        if(event->type() == QEvent::FocusIn)
        {
            if(patModel->rowCount()>0)
            {
                addButton->show();
            }
        }
        else if(event->type() == QEvent::FocusOut)
        {
            addButton->hide();
        }
    }*/
   // if(obj == ui->widget_title)
  //  {
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

  //  }

    return QWidget::eventFilter(obj,event);
}


