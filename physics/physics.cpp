/*
 * main.cpp
 *
 *  Created on: Jan 19, 2009
 *      Author: tim
 */

#include <list>
#include <math.h>
#include "types.h"

//sliding friction approx .2
//rolling friction approx .016

const double sliding_friction_constant = .2;
const double rolling_friction_constant = .016;
const double gravity_acceleration = 9.81; // m/s
const double mass_of_ball = 10;  //check these
const double mass_of_cue_stick = 50;
const double radius_of_ball = 27.5; //mm??
const double ratio_of_masses = mass_of_ball / mass_of_cue_stick;

//////////////////////////////////////////////////////////
// Computes the cross product of two vectors
//////////////////////////////////////////////////////////
vector_type crossProduct(vector_type a, vector_type b){
	vector_type return_vector;

	return_vector.i = a.j*b.k - a.k*b.j;
	return_vector.j = a.k*b.i - a.i*b.k;
	return_vector.k = a.i*b.j - a.j*b.i;

	return return_vector;
}

vector_type addVectors(vector_type a, vector_type b){
	vector_type return_vector;

	return_vector.i = a.i + b.i;
	return_vector.j = a.j + b.j;
	return_vector.k = a.k + b.k;

	return return_vector;
}

vector_type subtractVectors(vector_type a, vector_type b){
	vector_type return_vector;

	return_vector.i = a.i - b.i;
	return_vector.j = a.j - b.j;
	return_vector.k = a.k - b.k;

	return return_vector;
}

//////////////////////////////////////////////////////////
// Computes the magnitude of a vector
//////////////////////////////////////////////////////////
double magnitude(vector_type a){
	return sqrt( pow(a.i, 2) + pow(a.j, 2) + pow(a.k, 2));
}

//////////////////////////////////////////////////////////
// Applies the cue stick force to the cue ball
//////////////////////////////////////////////////////////
int applyCueForce(physics_state& current_state){
	list<ball>::iterator ball_iter = current_state.table.ball_positions.begin();

	while(ball_iter != current_state.table.ball_positions.end() && ball_iter->type != cue){
		ball_iter++;
	}

	//will we ever have to check for the end? we should always have a cue ball right?
	if(ball_iter == current_state.table.ball_positions.end()) return;

	//transfer cue stick force to cue ball
	double a = current_state.cue.x_offset;
	double b = current_state.cue.y_offset;
	double c = sqrt( pow(radius_of_ball,2) - pow(a,2) - pow(b,2));

	double top = 2 * mass_of_ball * current_state.cue.velocity;
	double theta = current_state.cue.vertical_angle;
	double bottom = pow(a,2) + (pow(b,2) * pow(cos(theta),2)) + (pow(c,2)*pow(sin(theta),2))
	                - (2*b*c*cos(theta)*sin(theta));

	bottom *= (5/(2*pow(radius_of_ball,2)));
	bottom += ratio_of_masses;
	bottom += 1;

	//force of cue impact
	double force = top/bottom;

	//linear velocity of ball
	ball_iter->velocity.i = 0;
	ball_iter->velocity.j = (-force/mass_of_ball) * cos(theta);
	ball_iter->velocity.k = (-force/mass_of_ball) * sin(theta);

	double moment_of_inertia = 2/5 * mass_of_ball * pow(radius_of_ball,2);

	//angular velocity of ball
	ball_iter->angular_velocity.i = (-c * force * sin(theta) + b * force * cos(theta)) / moment_of_inertia;
	ball_iter->angular_velocity.j = (a * force * sin(theta)) / moment_of_inertia;
	ball_iter->angular_velocity.k = (-a * force * cos(theta)) / moment_of_inertia;


	ball_iter->state = sliding;
}

//////////////////////////////////////////////////////////
// Tests whether any balls are moving
//////////////////////////////////////////////////////////
bool ballsAreMoving(table_state table){
	list<ball>::iterator ball_iter = table.ball_positions.begin();

	while(ball_iter != table.ball_positions.end()){
		if(ball_iter->state != stopped){
			return true;
		}
	}

	return false;
}

typedef enum{
	motionTrans,
	ballCollision,
	railCollision
} event_type;

typedef struct{
	double time;
	int ball1;
	int ball2;
	event_type event;
} event_time;

//////////////////////////////////////////////////////////
// Finds motion transition times for all balls
// returns smallest time
//////////////////////////////////////////////////////////
event_time findMotionTransitions(table_state& table){
	event_time smallest_time;
	smallest_time.event = motionsTrans;
	smallest_time.time = 1000000;
	smallest_time.ball1 = 0;

	list<ball>::iterator ball_iter = table.ball_positions.begin();

	while(ball_iter != table.ball_positions.end()){
		//transition from sliding to rolling
		if(ball_iter->state = sliding){
			double time_start_rolling = 2 * ball_iter->force.velocity /
			                           (7 * sliding_friction_constant * gravity_acceleration);

			if(time_start_rolling < smallest_time.time){
				smallest_time.time = time_start_rolling;
				smallest_time.ball1 = ball_iter->num;
			}
		}
		//transition from rolling to stopped
		else if(ball_iter->state = rolling){
			double time_stop = ball_iter->force.velocity / (rolling_friction_constant * gravity_acceleration);

			if(time_stop < smallest_time.time){
				smallest_time.time = time_stop;
				smallest_time.ball1 = ball_iter->num;
			}
		}
	}

	return smallest_time;
}

//////////////////////////////////////////////////////////
// Finds all ball-ball collision times
// returns smallest time
//////////////////////////////////////////////////////////
event_time findBallCollisions(table_state& table){

}

//////////////////////////////////////////////////////////
// Finds all ball-rail collisions (includes pockets)
// returns smallest time
//////////////////////////////////////////////////////////
event_time findRailCollisions(table_state& table){

}

//////////////////////////////////////////////////////////
// Moves the table to the next event
// Return: time of next event (current time at end of function)
//////////////////////////////////////////////////////////
double moveToNextEvent(table_state& table){
	findMotionTransitions(table);
	findBallCollisions(table);
	findRailCollisions(table);
}

//////////////////////////////////////////////////////////
// Generates the outcome of a shot
//////////////////////////////////////////////////////////
table_state generateOutcome(physics_state current_state){
	applyCueForce(current_state);

	double time = 0;
	//keep looping while balls are moving
	while(ballsAreMoving(current_state.table)){
		time = moveToNextEvent(current_state.table);
	}
}
