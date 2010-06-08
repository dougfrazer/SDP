#include <QApplication>

#include "imageWidget.h"
#include "circleWidget.h"
#include "tableWidget.h"
#include "window.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    /* Define Style Sheets */
    //qApp->setStyleSheet("QToolButton { border: 0px; margin: 0px; padding: 0px; border-radius: 0px; }");
    
    Window widget;
    
    widget.show();
    
    return app.exec();
}
