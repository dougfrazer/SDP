#include "poolfiz.h"

int main( int argc, char **argv )
{
	// Print poolfiz version to screen
	string vers = getPoolfizVersion();
	cout << "Initializing " << vers << endl;
	
	// Specify manual or auto shotViz exit
	bool manualExit = true;
	
	// Declare graphical instances
	fizGWindow window(1016, 508, 0.08);
	fizGShot myShot;
	fizGTable myTable;
	fizGTableState myState(window, myTable);
	fizGCue myCue(window, myTable.getLength(), myTable.getWidth());

	// Randomize the table state
	myState.randomize();
	
	// Define shot parameters
	double phi = 315.0;
	double V = 2.0;
	double a = -5.0;
	double b = 10.0;
	double theta = 25.0;
	
	// Check the shot parameters
	int cond1 = myTable.isValidBallPlacement( myState );
	int cond2 = myTable.isPhysicallyPossible( myState, a, b, theta, phi, V );

	// Execute a shot
	ShotStatus result = myShot.execute(myTable, myState, a, b, theta, phi, V, true);
	
	// Visualize the shot using shotViz 
	if (result == 0) {
		/* write the shot data to file */
		system("rm -f shotViz_info.txt");
		system("rm -f shotViz_shot.txt");	
		ofstream info("shotViz_info.txt");
		info << "shotViz_shot.txt ? ? ? ? ? ? ? ? ? ? ? ?" << endl;
		info.close();
		bool savedOk = myShot.save("shotViz_shot.txt", false);
		
		/*  visualize the shot */
		if (savedOk) {
			if (manualExit) system("./shotViz -m &");		
			else system("./shotViz &");		
			cout << "Done." << endl;					
		}
		else cout << "ERROR generating shot preview:  shot save result was bad." << endl;
	}
	else cout << "ERROR generating shot preview:  poolfiz shot result was bad." << endl;	

	return 0;
}
