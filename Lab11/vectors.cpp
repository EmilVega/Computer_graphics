/*
 *  vectors.c
 *  RayTracer
 *
 *  Created by Patricio Bulic on 5/30/13.
 *  Copyright 2013 University of Ljubljana, Faculty of Computer and Information Science. All rights reserved.
 *
 */

#include "vectors.h"
#include "math.h"


void vec_sub (VECTOR3D *v1, VECTOR3D *v2, VECTOR3D *v3) {

	v1->x = v2->x - v3->x;
	v1->y = v2->y - v3->y;
	v1->z = v2->z - v3->z;
}

void vec_add (VECTOR3D *v1, VECTOR3D *v2, VECTOR3D *v3) {
	
	v1->x = v2->x + v3->x;
	v1->y = v2->y + v3->y;
	v1->z = v2->z + v3->z;
}




void vec_scale (double scale, VECTOR3D *v1, VECTOR3D *v2) {
	
	v1->x = scale * v2->x;
	v1->y = scale * v2->y;
	v1->z = scale * v2->z;
}

double dotproduct (VECTOR3D *v1, VECTOR3D *v2) {
	
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

void normalize_vector (VECTOR3D *v) {
	
	double magnitude;
	
	// 1. calculate the magnitude (lerngth):
	magnitude = sqrt( dotproduct(v, v) );
	
	// 2. normalize the vector:
	v->x = v->x / magnitude;
	v->y = v->y / magnitude;
	v->z = v->z / magnitude;
}



