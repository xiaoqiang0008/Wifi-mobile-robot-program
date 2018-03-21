#ifndef MCURVEPAINT_H
#define MCURVEPAINT_H

#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QPaintDevice>
#include <QEvent>

class mCurvePaint : public QObject
{
    Q_OBJECT
public:
    mCurvePaint(QObject* parent = 0);

    void setPainter(QPainter* widget);
    void setDataBuff(char* buff1 , char* buff2);
    void setDataBuff_num(int buff1 , int buff2);

    void setAxes(int x11, int y11, int x22, int y22);
    void setAxescolor(Qt::GlobalColor color);
    void setAxesstyle(Qt::PenStyle penstyle);
    void setAxeswidth(qreal k);
    void setAxespen(QPen pen);

    void setTitle(QString s);
    void setTitlecolor(Qt::GlobalColor color);
    void setTitlestyle(Qt::PenStyle penstyle);
    void setTitlewidth(qreal k);
    void setTitlepen(QPen pen);

    void setRuler(bool r);
    void setRulercolor(Qt::GlobalColor color);
    void setRulerstyle(Qt::PenStyle penstyle);
    void setRulerwidth(qreal k);
    void setRulerpen(QPen pen);

    void setY_axes(int h);
    void setY_axescolor(Qt::GlobalColor color);
    void setY_axesstyle(Qt::PenStyle penstyle);
    void setY_axeswidth(qreal k);
    void setY_axespen(QPen pen);

    void setX_axes(int w);
    void setX_axescolor(Qt::GlobalColor color);
    void setX_axesstyle(Qt::PenStyle penstyle);
    void setX_axeswidth(qreal k);
    void setX_axespen(QPen pen);

    void setCurve1(bool r);
    void setCurve1color(Qt::GlobalColor color);
    void setCurve1style(Qt::PenStyle penstyle);
    void setCurve1width(qreal k);
    void setCurve1pen(QPen pen);

    void setCurve2(bool r);
    void setCurve2color(Qt::GlobalColor color);
    void setCurve2style(Qt::PenStyle penstyle);
    void setCurve2width(qreal k);
    void setCurve2pen(QPen pen);

    void paint();

private:
    QPainter* painter;
    int x1,x2,y1,y2;
    int width,height;
    char* Buffer1;
    char* Buffer2;
    int numbuff1,numbuff2;
    QString title;
    Qt::GlobalColor titlecolor,
                    axescolor;
    QPen titlePen,axesPen,rulerPen,Y_axesPen,X_axesPen,
            curve1pen,curve2pen;
    bool isRuleron,isCurve1on,isCurve2on;

    QPointF myTransform(qreal x , qreal y );

};

#endif // MCURVEPAINT_H
