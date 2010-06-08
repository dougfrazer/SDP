#ifndef WINDOW_H
#define WINDOW_H

#include <QGroupBox>
#include <QLabel>

class ImageWidget;
class CircleWidget;
class TableWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private:
    void createBody();
    void createHeader();
    QLabel* createLabel(const QString &text);

    QGroupBox *header, *body;
    CircleWidget *ballWidget;
    ImageWidget *imgWidget;
    TableWidget *tableWidget;
    QLabel *currPlayer, *bestShotDesc;
    
//    fizGWindow aWindow;
//	fizGTableState state;

    
};

#endif
