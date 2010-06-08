#include <math.h>
#include <cstdlib>

extern float ranf();

float box_muller(float m, float s)
{
	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if (use_last) /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
		x1 = 2.0 * ranf() - 1.0;
		x2 = 2.0 * ranf() - 1.0;
		w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}


