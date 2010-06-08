#include <QWidget>

class CircleWidget : public QWidget
{
   public:
    CircleWidget(float d = 40, QWidget *parent = 0);
    
   protected:
    void paintEvent(QPaintEvent *event);
    
   private:
    float diameter;
    QPointF *hitLocation;

};

