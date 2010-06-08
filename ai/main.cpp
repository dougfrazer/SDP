#include "AI.h"
#include "poolfiz.h"


int main(int argc, char** argv){
	AI theAI;

	fizGWindow aWindow;
	fizGTableState state(aWindow, *(theAI.theTable), ballRadius);
	fizPoint coor = theAI.theTable->getPocketCenter(SE_POCKET);

	state.randomize();
	//state.setBall(CUE, STATIONARY, .5, .6);
	//state.setBall(TWO, STATIONARY, .1, .1);
	//state.setBall(TEN, STATIONARY, .2, .2);
	//state.setBall(THREE, STATIONARY, 1, 1.2);  //misses shot
	//state.setBall(THREE, STATIONARY, .3, 2.3);  //wtf?? problem finding phi, should be fixed

	vec2 p1(0,0);
	vec2 p2(3,3);
	vec2 p3(3,2);
	double testDist = pointToLineDistance(p1, p2, p3);
	
	printf("%f\n", testDist);
	/*
	double x = coor.x;
	double y = coor.y;

	fizGShot loadedShot;

	if(argc > 1){
		double a,b, phi, theta, V;
		loadedShot.load(argv[1], state, a, b, phi, theta, V);
	}

	fizGShot theShot;
	shot bestShot;
	double score = theAI.chooseShot(state, 1, bestShot, solids);

	//bestShot.phi = 90;
	//bestShot.theta = 20;

	ShotStatus result = theShot.execute(*(theAI.theTable), state, bestShot.a, bestShot.b, bestShot.theta, bestShot.phi, bestShot.v);

	// write the shot data to file
	system("rm -f shotViz_info.txt");
	system("rm -f shotViz_shot.txt");
	ofstream info("shotViz_info.txt");
	info << "shotViz_shot.txt ? ? ? ? ? ? ? ? ? ? ? ?" << endl;
	info.close();
	bool savedOk = theShot.save("shotViz_shot.txt", false);

	// Visualize the shot using shotViz
	if (result == 0) {


		bool manualExit = true;
		//  visualize the shot
		if (savedOk) {
			if (manualExit) system("./shotVizOld -m &");
			else system("./shotVizOld &");
			cout << "Done." << endl;
		}
		else cout << "ERROR generating shot preview:  shot save result was bad." << endl;
	}*/

	return 0;
}
