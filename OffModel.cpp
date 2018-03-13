#include <iostream>
#include <string>
#include <fstream>
#include "OffModel.h"
#include "vec.h"

using namespace std;
vec3* init_ps(vec3* points, unsigned int num_points);
vec3* init_ts(vec3* triangles, unsigned int num_triangles);
vec3* populatePoints(std::string filename);
vec3* populateTriangles(std::string filename);
fstream file;

void normalize2(vec3 *p, vec3 max, vec3 min, int num_points);

OffModel::OffModel() {
	num_points = 0;
	num_triangles = 0;
	triangles = NULL;
	points = NULL;
}

OffModel::~OffModel(void)
{
	if (num_points > 0) {
		num_points = 0;
		delete[] points;
	}

	if (num_triangles > 0) {
		num_triangles = 0;
		delete[] triangles;
	}
}


bool OffModel::load(std::string filename) {

	setup(filename);
	points = populatePoints(filename);
	triangles = populateTriangles(filename);
	findCenter();
	control = 0;
	reinit = 0;

	std::cout << filename << endl;
	return true;
}

void OffModel::setup(std::string filename) {
	fstream file(filename);
	string off;
	int tmp;
	file >> off;
	file >> num_points >> num_triangles >> tmp;
	//cout << off << num_points << num_triangles << endl;
	file.close();
}

unsigned int findMax(unsigned int a, unsigned int b) {

	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

vec3* populatePoints(std::string filename) {
	vec3* mpoints = NULL;
	int i, num_points, num_triangles;
	string off_word;

	file = fstream(filename);

	if (!file.is_open()) {
		return false;
	}

	file >> off_word;
	//cout << off_word<< endl;
	file >> num_points >> num_triangles >> i;
	mpoints = init_ps(mpoints, num_points);
	for (i = 0; i < num_points; i++)
	{
		file >> mpoints[i].x >> mpoints[i].y >> mpoints[i].z;
	}
	file.close();
	return mpoints;
}

vec3* populateTriangles(std::string filename) {
	vec3* mtriangles = NULL;
	int i, num_points, num_triangles;
	string off_word;

	file = fstream(filename);
	if (!file.is_open()) {
		return false;
	}

	file >> off_word;
	file >> num_points >> num_triangles >> i;

	for (i = 0; i < num_points; i++) {
		GLfloat temp1, temp2, temp3;
		file >> temp1 >> temp2 >> temp3;
		//cout << temp1 << temp2 << temp3 << endl;
	}

	mtriangles = init_ts(mtriangles, num_triangles);

	for (i = 0; i < num_triangles; i++)
	{
		GLfloat tempa;
		file >> tempa >> mtriangles[i][v1] >> mtriangles[i][v2] >> mtriangles[i][v3];
		//cout << tempa << endl;
	}
	file.close();
	return mtriangles;


}

void OffModel::findCenter() {
	int i;
	vec3 max, min;
	vec3 temp_point;
	for (i = 0; i < (int)num_points; i++) {
		temp_point = points[i];

		if (temp_point.x > max.x) {
			max.x = temp_point.x;
		}
		if (temp_point.y > max.y) {
			max.y = temp_point.y;
		}
		if (temp_point.z > max.z) {
			max.z = temp_point.z;
		}
		if (temp_point.x < min.x) {
			min.x = temp_point.x;
		}
		if (temp_point.y < min.y) {
			min.y = temp_point.y;
		}
		if (temp_point.z < min.z) {
			min.z = temp_point.z;
		}

	}
	
	for (i = 0; i < num_points; i++) {
	//	points[i]=normalize(points[i]);
	}

	center = (max + min) / 2.f;
	scale = 1.f / findMax(max.x - min.x, findMax(max.y - min.y, max.z - min.z));



}

vec3* init_ps(vec3* points, unsigned int num_points) {

	if (points) delete[] points;
	points = new vec3[num_points];
	return points;
}

vec3* init_ts(vec3* triangles, unsigned int num_triangles) {
	if (triangles) delete[] triangles;
	triangles = new vec3[num_triangles];
	return triangles;
}

void normalize2(vec3 *p, vec3 max, vec3 min, int num_points) {
	int i;
	vec3 mpoint;
	GLfloat maxx = max.x; 
	GLfloat maxy = max.y;
	GLfloat maxz = max.z;
	GLfloat minx = min.x;
	GLfloat miny = min.y;
	GLfloat minz = min.z;


	for (i = 0; i < num_points; i++) {

		mpoint = p[i];
		mpoint.x = 2 * (mpoint.x - minx) / (maxx - minx) - 1;
		mpoint.y = 2 * (mpoint.y - miny) / (maxy - miny) - 1;
		mpoint.z = 2 * (mpoint.z - minz) / (maxz - minz) - 1;
		p[i] = mpoint;

	}
}

