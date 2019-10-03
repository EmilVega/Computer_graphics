/*
	How to use this code:

	Call load_mesh("bunny.obj") after creating the window using GLUT. This will
	read the mesh data from bunny.obj and populate the arrays vVertex,
	gNormals, and tIndices.

	When rendering, we use a similar convention to the sphere from PA2. In other
	words, for triangle i, define:

		k0 = tIndices[i].indices[0];
		k1 = tIndices[i].indices[1];
		k2 = tIndices[i].indices[2];

	Then the vertices of the triangle are at vVertex[k0], vVertex[k1], and
	vVertex[k2], in that order. The normals of the corresponding vertices are
	at gNormals[k0], gNormals[k1], and gNormals[k2], in that order.

	For the second part of the assignment, you have to copy the vVertex,
	gNormals, and tIndices arrays into GPU memory.
*/

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>
#include <stdlib.h>

struct Vector4
{
	float			x, y, z;
};

struct Triangle2
{
	unsigned int 	indices[3];
};

std::vector<Vector4>	vVertex2;
std::vector<Vector4>	gNormals2;
std::vector<Triangle2>	tIndices2;

void tokenize2(char* string, std::vector<std::string>& tokens, const char* delimiter)
{
	char* token = strtok(string, delimiter);
	while (token != NULL)
	{
		tokens.push_back(std::string(token));
		token = strtok(NULL, delimiter);
	}
}

int face_index2(const char* string)
{
	int length = strlen(string);
	char* copy = new char[length + 1];
	memset(copy, 0, length+1);
	strcpy(copy, string);

	std::vector<std::string> tokens;
	tokenize2(copy, tokens, "/");
	delete[] copy;
	if (tokens.front().length() > 0 && tokens.back().length() > 0 && atoi(tokens.front().c_str()) == atoi(tokens.back().c_str()))
	{
		return atoi(tokens.front().c_str());
	}
	else
	{
		printf("ERROR: Bad face specifier!\n");
		exit(0);
	}
}

void load_mesh2(std::string fileName)
{
	std::ifstream fin(fileName.c_str());
	if (!fin.is_open())
	{
		printf("ERROR: Unable to load mesh from %s!\n", fileName.c_str());
		exit(0);
	}

	float xmin = FLT_MAX;
	float xmax = -FLT_MAX;
	float ymin = FLT_MAX;
	float ymax = -FLT_MAX;
	float zmin = FLT_MAX;
	float zmax = -FLT_MAX;

	while (true)
	{
		char line[1024] = {0};
		fin.getline(line, 1024);

		if (fin.eof())
			break;

		if (strlen(line) <= 1)
			continue;

		std::vector<std::string> tokens;
		tokenize2(line, tokens, " ");

		if (tokens[0] == "v")
		{
			float x = atof(tokens[1].c_str());
			float y = atof(tokens[2].c_str());
			float z = atof(tokens[3].c_str());

			xmin = std::min(x, xmin);
			xmax = std::max(x, xmax);
			ymin = std::min(y, ymin);
			ymax = std::max(y, ymax);
			zmin = std::min(z, zmin);
			zmax = std::max(z, zmax);

			Vector4 position = {x, y, z};
			vVertex2.push_back(position);
		}
		else if (tokens[0] == "vn")
		{
			float x = atof(tokens[1].c_str());
			float y = atof(tokens[2].c_str());
			float z = atof(tokens[3].c_str());
			Vector4 normal = {x, y, z};
			gNormals2.push_back(normal);
		}
		else if (tokens[0] == "f")
		{
			unsigned int a = face_index(tokens[1].c_str());
			unsigned int b = face_index(tokens[2].c_str());
			unsigned int c = face_index(tokens[3].c_str());
			Triangle2 triangle;
			triangle.indices[0] = a - 1;
			triangle.indices[1] = b - 1;
			triangle.indices[2] = c - 1;
			tIndices2.push_back(triangle);
		}
	}

	fin.close();

	//printf("Loaded mesh from %s. (%lu vertices, %lu normals, %lu triangles)\n", fileName.c_str(), vVertex.size(), gNormals.size(), tIndices.size());
	//printf("Mesh bounding box is: (%0.4f, %0.4f, %0.4f) to (%0.4f, %0.4f, %0.4f)\n", xmin, ymin, zmin, xmax, ymax, zmax);
}
