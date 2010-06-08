#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGui>
#include "circleWidget.h"

CircleWidget::CircleWidget(float d, QWidget *parent) : QWidget(parent)
{
    diameter = d;
    setFixedSize(d, d);
    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hitLocation = new QPointF(diameter/2.0, diameter/2.0);
}

void CircleWidget::paintEvent(QPaintEvent *)
{
    QRadialGradient radialGrad(QPoint(diameter/2.0, diameter/2.0), diameter/2.0);
    radialGrad.setColorAt(0, Qt::white);
    radialGrad.setColorAt(0.5, Qt::white);
    radialGrad.setColorAt(1, Qt::lightGray);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setBrush( QBrush(radialGrad) );
    painter.setPen( QPen(Qt::lightGray, 2) );
    painter.drawEllipse(QRectF(0.0, 0.0, diameter, diameter));
    painter.setBrush( QBrush(Qt::red) );
    painter.setPen( QPen(Qt::red, 1) );
    painter.drawEllipse( QRectF(*hitLocation-QPointF(3.0,3.0),*hitLocation+QPointF(3.0,3.0)) );
    painter.end();
}
