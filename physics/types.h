/*
 * types.h
 *
 *  Created on: Jan 19, 2009
 *      Author: tim
 */

#ifndef TYPES_H_
#define TYPES_H_

#include<list>

typedef enum{
	stripe,
	solid,
	cue
} ball_type;

typedef enum{
	sliding,
	rolling,
	stopped
} state_type;

typedef struct{
	double i;
	double j;
	double k;
	//coordinate system layed out on table
	//short side is i axis, long is j, vertical is k
	//cue ball start position is down, rack position is up
	//coordinates start at bottom left corner of table
} vector_type;

typedef struct{
	int number;
	ball_type type;
	state_type state;
	vector_type position;
	vector_type velocity;
	vector_type relative_velocity;
	vector_type angular_velocity;
} ball;

typedef struct{
	list<ball> ball_positions;
} table_state;

typedef struct{
	double lateral_angle;
	double vertical_angle;
	double x_offset;
	double y_offset;
	double velocity;
} cue_state;

typedef struct{
	table_state table;
	cue_state cue;
} physics_state;

#endif /* TYPES_H_ */
