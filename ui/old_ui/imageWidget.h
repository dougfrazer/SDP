#include <QWidget>
#include <list>
using namespace std;

static const QSize desiredSize(640*4/5, 480*4/5);
static const QSize imgSize(640, 480);


class ImageWidget : public QWidget
{
    public:
        ImageWidget( QString &file_tr, QString &file_tl, QString &file_bl, QString &file_br, QWidget *parent = 0);
        void drawLine(QLine line);
    
    protected:
        void paintEvent(QPaintEvent *event);
        
    private:
        QPoint imagePos(const QImage &image) const;
        QImage image;
        QString fileName[4];
        list<QLine> lines;
        

};


