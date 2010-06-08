#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGui>
#include "tableWidget.h"
#include "circleFinder.h"

#define WOOD_BORDER_WIDTH 20
#define POCKET_SIZE 50
#define BALL_DIAMETER 20

using namespace std;

TableWidget::TableWidget(QSize setSize, QWidget *parent) : QWidget(parent)
{
  size = setSize;
  setFixedSize(size);
  fillList();
}

void TableWidget::paintEvent(QPaintEvent *)
{
  QRectF outerRect = QRectF(
                      QPointF(0.0, 0.0), 
                      QPointF(size.width(), size.height()) );
  QRectF tableRect = QRectF(
                      QPointF(WOOD_BORDER_WIDTH, WOOD_BORDER_WIDTH), 
                      QPointF(size.width() - WOOD_BORDER_WIDTH, size.height() - WOOD_BORDER_WIDTH));

  QPainter painter;

  painter.begin(this);
  painter.setBrush( QColor(139,69,19) );
  painter.setCompositionMode(QPainter::CompositionMode_Destination);
  painter.drawRect(outerRect);

  painter.setBrush( QColor(0,100,0) );
  painter.setCompositionMode(QPainter::CompositionMode_Destination);
  painter.drawRect(tableRect);

  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.setBrush(Qt::black);

  // top pockets (left to right)
  painter.drawPie( QRectF(QPointF(-WOOD_BORDER_WIDTH/2.0, -WOOD_BORDER_WIDTH/2.0), 
                          QPointF(POCKET_SIZE, POCKET_SIZE)),  
                          0, -90*16);
  painter.drawPie( QRectF(QPointF( (size.width()/2.0)-(POCKET_SIZE*.8)/2.0,0.0), 
                          QPointF( (size.width()/2.0)+(POCKET_SIZE*.8)/2.0, (POCKET_SIZE*.8))),  
                          0, -180*16);
  painter.drawPie( QRectF(QPointF( size.width()-POCKET_SIZE, -WOOD_BORDER_WIDTH/2.0), 
                          QPointF( size.width()+WOOD_BORDER_WIDTH/2.0,POCKET_SIZE)),  
                          -90*16, -90*16);

  // bottom pockets (left to right)
  painter.drawPie( QRectF(QPointF(-WOOD_BORDER_WIDTH/2.0, size.height()+WOOD_BORDER_WIDTH/2.0), 
                          QPointF(POCKET_SIZE, size.height()-POCKET_SIZE)),  
                          0, 90*16);
  painter.drawPie( QRectF(QPointF( (size.width()/2.0)-(POCKET_SIZE*.8)/2.0, size.height()+0.0), 
                          QPointF( (size.width()/2.0)+(POCKET_SIZE*.8)/2.0, size.height()-(POCKET_SIZE*.8))),  
                          0, 180*16);
  painter.drawPie( QRectF(QPointF( size.width()-POCKET_SIZE, size.height()-POCKET_SIZE), 
                          QPointF( size.width()+WOOD_BORDER_WIDTH/2.0,size.height()+WOOD_BORDER_WIDTH/2.0)),  
                          90*16, 90*16);

  painter.setBrush(Qt::white);

  for (list<QPoint>::iterator it = centerList.begin(); it != centerList.end(); it++)
  {
    
    int height = it->x();
    int width = it->y();
    painter.drawEllipse(QRectF( QPointF(width-BALL_DIAMETER/2.0, height-BALL_DIAMETER/2.0),
                                QPointF(width+BALL_DIAMETER/2.0, height+BALL_DIAMETER/2.0) ));
    
  }

  painter.end();
}


void TableWidget::fillList()
{
  string fileName = "pic6.pnm";
  CircleFinder *cf = new CircleFinder();
  vector<ball> v = cf->getLocations(fileName);
  for(vector<ball>::iterator iter = v.begin(); iter != v.end(); iter++)
  {
    centerList.insert(centerList.begin(), QPoint(iter->y/2.0 + WOOD_BORDER_WIDTH, iter->x/2.0 + WOOD_BORDER_WIDTH));
  }

}
