#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGui>
#include "imageWidget.h"

ImageWidget::ImageWidget( QString &file_tr, QString &file_tl, QString &file_bl, QString &file_br, QWidget *parent) : QWidget(parent)
{
    setFixedSize(desiredSize.width() * 2.0, desiredSize.height() *2.0);
    setBackgroundRole(QPalette::Base);
    QImage image(desiredSize, QImage::Format_ARGB32_Premultiplied);
    fileName[0] = file_tl;
    fileName[1] = file_tr;
    fileName[2] = file_bl;
    fileName[3] = file_br;
    

}

void ImageWidget::paintEvent(QPaintEvent *)
{
  QRectF desiredRect[4];
  desiredRect[0] = QRectF(  QPointF(desiredSize.width(), desiredSize.height()), 
                            QPointF(2.0*desiredSize.width(), 2.0*desiredSize.height()));
  desiredRect[1] = QRectF(  QPointF(0.0, desiredSize.height()), 
                            QPointF(desiredSize.width(), 2.0*desiredSize.height()));
  desiredRect[2] = QRectF(  QPointF(0.0, 0.0), 
                            QPointF(desiredSize.width(), desiredSize.height()));
  desiredRect[3] = QRectF(  QPointF(desiredSize.width(), 0.0), 
                            QPointF(2.0*desiredSize.width(), desiredSize.height()));

  QPainter painter;
  QPen pen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  
  
  QRectF defaultRect(0.0, 0.0, imgSize.width(), imgSize.height());
  QRectF appRect(0.0, 0.0, desiredSize.width(), desiredSize.height());

  
  for( int i = 0; i < 4; ++i)
  {
    /* load the image */
    image.load(fileName[i]);
    
    painter.begin(&image);
    painter.drawPixmap(imagePos(image), QPixmap::fromImage(image));
    painter.end();
    
    /* get the area ready to draw in */
    painter.begin(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(desiredRect[i], image, defaultRect);
    painter.end();
  }
  
  for (list<QLine>::iterator it = lines.begin(); it != lines.end(); it++)
  {
    painter.begin(this);
    painter.setPen(pen);
    painter.drawLine(*it);
    painter.end();
  }

  
  

}

QPoint ImageWidget::imagePos(const QImage &image) const
{
    return QPoint((imgSize.width() - image.width()) / 2,
                  (imgSize.height() - image.height()) / 2);
}

void ImageWidget::drawLine(QLine line)
{
   lines.insert(lines.begin(), line);
   repaint();
}
