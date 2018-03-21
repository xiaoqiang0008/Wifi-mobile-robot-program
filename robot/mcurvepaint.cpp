#include "mcurvepaint.h"

mCurvePaint::mCurvePaint(QObject *parent):
    QObject(parent)
{
    width = 100;
    height = 40;
    x1 = 20;
    y1 = 10;
    x2 = 300;
    y2 = 160;
    Buffer1 = NULL;
    Buffer2 = NULL;
    numbuff1 = 100;
    numbuff2 = 100;
    title = "Curve";
    titlecolor = Qt::white;
    axescolor = Qt::white;
    titlePen.setColor(Qt::white);
    titlePen.setWidthF(1);
    titlePen.setStyle(Qt::SolidLine);
    axesPen.setColor(Qt::white);
    axesPen.setWidthF(1);
    axesPen.setStyle(Qt::SolidLine);
    rulerPen.setColor(Qt::white);
    rulerPen.setWidthF(1);
    rulerPen.setStyle(Qt::DotLine);
    isRuleron = true;
    Y_axesPen.setColor(Qt::white);
    Y_axesPen.setWidthF(1);
    Y_axesPen.setStyle(Qt::SolidLine);
    X_axesPen.setColor(Qt::white);
    X_axesPen.setWidthF(1);
    X_axesPen.setStyle(Qt::SolidLine);
    curve1pen.setColor(Qt::green);
    curve1pen.setWidthF(1);
    curve1pen.setStyle(Qt::SolidLine);
    curve2pen.setColor(Qt::yellow);
    curve2pen.setWidthF(1);
    curve2pen.setStyle(Qt::SolidLine);
    isCurve1on = true;
    isCurve2on = true;

}

void mCurvePaint::setPainter(QPainter *widget)
{
    painter = widget;
  //  qDebug("mCurvepaint get painter");
}

void mCurvePaint::setAxes(int x11, int y11, int x22, int y22)
{
    x1 = x11;
    y1 = y11;
    x2 = x22;
    y2 = y22;
 //   qDebug("mCurvepaint set axes");
}

void mCurvePaint::setDataBuff(char* buff1 , char* buff2)
{
    Buffer1 = buff1;
    Buffer2 = buff2;
}

void mCurvePaint::setDataBuff_num(int buff1 , int buff2)
{
    numbuff1 = buff1;
    numbuff2 = buff2;
}

void mCurvePaint::setTitle(QString s)
{
    title = s;
}

void mCurvePaint::setTitlecolor(Qt::GlobalColor color)
{
    titlePen.setColor(color);
}

void mCurvePaint::setTitlestyle(Qt::PenStyle penstyle)
{
    titlePen.setStyle(penstyle);
}

void mCurvePaint::setTitlewidth(qreal k)
{
    titlePen.setWidthF(k);
}

void mCurvePaint::setTitlepen(QPen pen)
{
    titlePen = pen;
}

void mCurvePaint::setAxescolor(Qt::GlobalColor color)
{
    axesPen.setColor(color);
}

void mCurvePaint::setAxesstyle(Qt::PenStyle penstyle)
{
    axesPen.setStyle(penstyle);
}

void mCurvePaint::setAxeswidth(qreal k)
{
    axesPen.setWidthF(k);
}

void mCurvePaint::setAxespen(QPen pen)
{
    axesPen = pen;
}

//标尺相关设置
void mCurvePaint::setRuler(bool r)
{
    isRuleron = r;
}

void mCurvePaint::setRulercolor(Qt::GlobalColor color)
{
    rulerPen.setColor(color);
}

void mCurvePaint::setRulerstyle(Qt::PenStyle penstyle)
{
    rulerPen.setStyle(penstyle);
}

void mCurvePaint::setRulerwidth(qreal k)
{
    rulerPen.setWidthF(k);
}

void mCurvePaint::setRulerpen(QPen pen)
{
    rulerPen = pen;
}

//纵坐标相关设置
void mCurvePaint::setY_axes(int h)
{
    height = h;
}

void mCurvePaint::setY_axescolor(Qt::GlobalColor color)
{
    Y_axesPen.setColor(color);
}

void mCurvePaint::setY_axesstyle(Qt::PenStyle penstyle)
{
    Y_axesPen.setStyle(penstyle);
}

void mCurvePaint::setY_axeswidth(qreal k)
{
    Y_axesPen.setWidthF(k);
}

void mCurvePaint::setY_axespen(QPen pen)
{
    Y_axesPen = pen;
}

//横坐标相关设置
void mCurvePaint::setX_axes(int w)
{
    width = w;
}

void mCurvePaint::setX_axescolor(Qt::GlobalColor color)
{
    X_axesPen.setColor(color);
}

void mCurvePaint::setX_axesstyle(Qt::PenStyle penstyle)
{
    X_axesPen.setStyle(penstyle);
}

void mCurvePaint::setX_axeswidth(qreal k)
{
    X_axesPen.setWidthF(k);
}

void mCurvePaint::setX_axespen(QPen pen)
{
    X_axesPen = pen;
}

//曲线1相关设置
void mCurvePaint::setCurve1(bool r)
{
    isCurve1on = r;
}
void mCurvePaint::setCurve1color(Qt::GlobalColor color)
{
    curve1pen.setColor(color);
}
void mCurvePaint::setCurve1style(Qt::PenStyle penstyle)
{
    curve1pen.setStyle(penstyle);
}
void mCurvePaint::setCurve1width(qreal k)
{
    curve1pen.setWidthF(k);
}
void mCurvePaint::setCurve1pen(QPen pen)
{
    curve1pen = pen;
}

//曲线2相关设置
void mCurvePaint::setCurve2(bool r)
{
    isCurve2on = r;
}
void mCurvePaint::setCurve2color(Qt::GlobalColor color)
{
    curve2pen.setColor(color);
}
void mCurvePaint::setCurve2style(Qt::PenStyle penstyle)
{
    curve2pen.setStyle(penstyle);
}
void mCurvePaint::setCurve2width(qreal k)
{
    curve2pen.setWidthF(k);
}
void mCurvePaint::setCurve2pen(QPen pen)
{
    curve2pen = pen;
}

QPointF mCurvePaint::myTransform(qreal x , qreal y )
{
    QPointF point;
    qreal mx,my;
    mx = x*(x2-x1)/width+x1;
    my = y*(y2-y1)/height+y1;
    my = y2+y1-my;
    point.setX(mx);
    point.setY(my);
    return point;
}

void mCurvePaint::paint()
{
    int i=0;
    //坐标系设置
        painter->setPen(axesPen);//设置画笔样式
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawLine(x1,y1,x1,y2);
        painter->drawLine(x1,y2,x2,y2);

        //标题设置
        painter->setPen(titlePen);
        QRectF rect = QRectF(0,0,x2,y1);
        painter->drawText(rect,Qt::AlignVCenter|Qt::AlignHCenter,title);

        //绘制标尺
        if(isRuleron)
        {
        painter->setPen(rulerPen);
        for(i=1 ; i <= height/5 ; i++)
        {
            painter->drawLine(myTransform(0,i*5),
                             myTransform(width,i*5));
        }
        }

        //绘制纵坐标
        painter->setPen(Y_axesPen);
        for(i=1 ; i <= height/5 ; i++)
        {
            QRectF rect = QRectF(0,myTransform(-3*2,i*5+1).y(),
                                 x1,myTransform(0,i*5-5).y());
            QString s = QString::number(i*5, 10);
            painter->drawText(rect,Qt::AlignHCenter,s);
        }
        //绘制横坐标
        painter->setPen(X_axesPen);
        for(i=0;i<6;i++)
        {
            QRectF rect = QRectF(myTransform(i*(width/5),2*(-0.5)).x()-15 , y2+3,
                                 myTransform(i*(width/5),2*(-3.5)).x()+15 , y2+10);
            QString s = QString::number(i*width/5, 10);
            painter->drawText(rect,Qt::AlignLeft,s);
        }
        //绘制曲线1
        if(isCurve1on)
        {
        painter->setPen(curve1pen);
        if(Buffer1 != NULL)
        {
        for(i=0;i<numbuff1-1;i++)
        {
            if(Buffer1[i]>height)
                Buffer1[i] = height;
            painter->drawLine(myTransform(i*(width/100),Buffer1[i]),
                              myTransform((i+1)*(width/100),Buffer1[i+1]));
        }
        }
        }
        //绘制曲线2
        if(isCurve2on)
        {
        painter->setPen(curve2pen);
        if(Buffer2 != NULL)
        {
        for(i=0;i<numbuff2-1;i++)
        {
            if(Buffer2[i]>height)
                Buffer2[i] = height;
            painter->drawLine(myTransform(i*(width/100),Buffer2[i]),
                              myTransform((i+1)*(width/100),Buffer2[i+1]));
        }
        }
        }

}



