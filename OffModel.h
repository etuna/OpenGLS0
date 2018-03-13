#pragma once
#include "vec.h"

#define v1 0
#define v2 1
#define v3 2
#define v4 3

class OffModel {

public: 
	
	//num of points, num of triangles,scale
	unsigned int num_points;
	unsigned int num_triangles;
	float scale;
	int control;
	int reinit;
	//Vecs
	vec3* triangles;
	vec3* points;
	vec3 center;


	//functions
	OffModel(void);
	~OffModel(void);
	bool load(std::string filename);
	void findCenter();
	void setup(std::string filename);
};