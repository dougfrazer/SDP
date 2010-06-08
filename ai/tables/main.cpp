#include "../poolfiz.h"
#include "../velocitytable.h"
#include "../probtable.h"
//#include "railtable.h"

int main( int argc, char **argv )
{
	ProbTable pt;
	VelocityTable vt;
	//RailTable rt;

	double length = 2.4384;               //the length of the table in metres
	double width = 1.2192;                //the width of the table in metres
	double cornerPocketWidth = 0.1061;    //the width of corner pockets (horn to horn) in metres
	double sidePocketWidth = 0.15;        //the width of the side pockets (horn to horn) in metres
	double muS = 0.2;                     //the coefficient of sliding friction (dimensionless)
	double muR = 0.01;                    //the coefficient of rolling friction (dimensionless)
	double muSp = 0.044;                  //the coefficient of spinning friction (dimensionless)
	double railHeight = 0.05715;          //the height of the top of the rail above the table in metres
	double cueLength = 1.0;               //the length of the cue in metres
	double railVelDamping = 0.6;          //damping factor of the banks
	double railSpinDamping = 0.1;         //spin damping factor of the banks
	double railZSpinAngleAdj = 0.0005;    // z-spin angle of deflection factor of the banks
	double railVelAngleAdj = 0.001;       //velocity deflection factor of the banks
	//onRobot indicates whether the game is being played on the robotic table (optional, default = false)

	fizGShot myShot;
	fizGTable myTable(length,
						width,
						cornerPocketWidth,
						sidePocketWidth,
						muS,
						muR,
						muSp,
						railHeight,
						cueLength,
						railVelDamping,
						railSpinDamping,
						railZSpinAngleAdj,
						railVelAngleAdj);

	string s;
	s = getPoolfizVersion();
	cout << s;

	printf("length %f width %f headstring %f\n",myTable.getLength(),myTable.getWidth(),myTable.getHeadString());

	//computeBreak(myShot, myTable);

	if(!vt.load("velocity2.tbl"))
	{
		vt.fill(myTable, myShot);
		vt.save("velocity2.tbl");
	}

	pt.fill(&vt, myTable, myShot);
	pt.save("probtable2.tbl");


	/*
	rt.fill(myTable,myShot);
	rt.save("railtable.tbl");
	*/
	return 0;
}

