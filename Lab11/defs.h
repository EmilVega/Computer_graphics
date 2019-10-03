/*
 *  defs.h
 *  RayTracer
 *
 *  Created by Patricio Bulic on 5/30/13.
 *  Copyright 2013 University of Ljubljana, Faculty of Computer and Information Science. All rights reserved.
 *
 */

#ifndef DEFS_H
#define DEFS_H

#define _BLINNPHONG
//#define _LAMBERT
//#define _NOSHADING

//typedef enum {false=0, true=1} BOOLEAN;
typedef enum {down=0, up=1} DIRECTION;

#define CRED 0
#define CGREEN 1
#define CBLUE 2


/* --------------- VECTORS -------------------- */

struct VECTOR3D{
	double x;
	double y;
	double z;
} ;


/* ----------------- VIEWPORT ----------------- */
struct VIEWPORT {
	int xvmin;
	int yvmin;
	int xvmax;
	int yvmax;
};


/* ------------------- PIXEL ------------------ */
struct PIXEL{
	int i;
	int j;
};


/* ---------------- SPHERE -------------------- */

struct SPHERE_INTERSECTION {
	double	lambda_in;
	double	lambda_out;
	VECTOR3D	normal;
	VECTOR3D point;
	bool	valid;
} ;

struct SPHERE {
	VECTOR3D center;
	double radius;
	double kd_rgb[3];
	double ks_rgb[3];
	double ka_rgb[3];
	double shininess;
	bool mirror;
};


/* ------------------- RAY --------------------- */
struct RAY {
	VECTOR3D origin;
	VECTOR3D direction;
};


/* --------------- VECTOR BASIS ---------------- */
struct VEC_BASIS {
	VECTOR3D u;
	VECTOR3D v;
	VECTOR3D n;
};




#endif

