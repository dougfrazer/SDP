#include <math.h>
#include <stdio.h>
#include <list>
#include "linefinder.h"

int main(int argc, char** argv)
{
    list<line_T> theLines;
    findLines(argv[1], theLines, 0);
}

