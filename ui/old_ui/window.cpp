
#include <QtGui>
#include <QPainter>
#include <QImage>
#include "window.h"
#include "circleWidget.h"
#include "imageWidget.h"
#include "tableWidget.h"
#include "AI.h"

Window::Window()
{ 
	AI ai;
	fizGWindow aWindow;
	fizGTableState state(aWindow, *(ai.theTable), ballRadius);
    
    createBody();
    createHeader();
    
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(header);
    mainLayout->addWidget(body);
    setLayout(mainLayout);
    
   // imgWidget->drawLine(QLine(QPoint(200,200), QPoint(400,400)));
    
    setWindowTitle(tr("Pool Table Analyzer"));
}

void Window::createHeader()
{
    header = new QGroupBox(tr("Header Information"));
    QHBoxLayout *layout = new QHBoxLayout;
    bestShotDesc = new QLabel("Hit the ball in the direction of the line\nThe cue will provide feedback for your position relative to the cue.");
    ballWidget = new CircleWidget(120.0);
    layout->setSpacing(1);
    layout->addWidget(bestShotDesc);
    layout->addWidget(createLabel("Current Player:\nStripes"));
    layout->addWidget(ballWidget);
    header->setLayout(layout);
}

QLabel* Window::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont font("Times", 20);
    lbl->setFont(font);
    return lbl;
 }

/*
void Window::createBody()
{
    body = new QGroupBox(tr("Pool Table"));
    
    QString fileName_tl = "img3.jpg";
    QString fileName_tr = "img3.jpg";
    QString fileName_bl = "img3.jpg";
    QString fileName_br = "img3.jpg";
   
    imgWidget = new ImageWidget(fileName_tr, fileName_tl, fileName_bl, fileName_br);
    
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->addWidget(imgWidget, 0, 0);
    body->setLayout(layout);
}*/
void Window::createBody()
{
/*    body = new QGroupBox(tr("Pool Table"));
    state.randomize();
    
	fizGShot theShot;
	shot bestShot;
	double score = theAI.chooseShot(state, 2, bestShot);

	ShotStatus result = theShot.execute(*(theAI.theTable), state, bestShot.a, bestShot.b, bestShot.theta, bestShot.phi, bestShot.v);

	// Visualize the shot using shotViz
	if (result == 0) {
		 write the shot data to file 
		system("rm -f shotViz_info.txt");
		system("rm -f shotViz_shot.txt");
		ofstream info("shotViz_info.txt");
		info << "shotViz_shot.txt ? ? ? ? ? ? ? ? ? ? ? ?" << endl;
		info.close();
		bool savedOk = theShot.save("shotViz_shot.txt", false);

		bool manualExit = true;
		  visualize the shot 
		if (savedOk) {
			if (manualExit) system("./shotViz -m &");
			else system("./shotViz &");
			cout << "Done." << endl;
		}
		else cout << "ERROR generating shot preview:  shot save result was bad." << endl;
	}
    
    
    //1905x952.5
    tableWidget = new TableWidget( QSize(1905/2.0, 952.5/2.0) );
    
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->addWidget(tableWidget,0,0);
    body->setLayout(layout);     */

}
