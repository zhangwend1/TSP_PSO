#pragma once
#include "velocity.h"
#include "position.h"
#include <climits>


class Particle {
public:
	Particle(double self_trust, double past_trust, double global_trust);

	bool is_intersect(int i, int j); //j>i+1
	void intersect_eliminate();

	void betterway();


	double move();  //Move based on velocity, return best value seen
	double calculate_value();  //Get sum of wieghts of the edges we're using
	void calculate_new_velocity(Position global_best);


	Velocity velocity;
	Position position;
	double best_value;
	Position best_position;

	//These should add up to 1.0
	double self_trust;
	double past_trust;
	double global_trust;
};


