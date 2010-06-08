#include "poolfiz.h"

int main( int argc, char **argv )
{
	// Print poolfiz version to screen
	string vers = getPoolfizVersion();
	cout << "Initializing " << vers << endl;
	
	// Declare non-graphical instances
	fizShot myShot;
	fizTable myTable;
	fizTableState myState(myTable);

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
	myShot.execute(myTable, myState, a, b, theta, phi, V, true);

	return 0;
}
