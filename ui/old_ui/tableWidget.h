#include <QWidget>
#include <list>
using namespace std;

class TableWidget : public QWidget
{
    public:
        TableWidget(QSize setSize, QWidget *parent = 0);

    protected:
        void paintEvent(QPaintEvent *event);
        void fillList();
        
    private:
        QSize size;
        list<QPoint> centerList;
        

};
