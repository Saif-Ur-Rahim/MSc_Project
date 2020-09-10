#pragma once
#ifndef _VEC3_
#define _VEC3_
#include<cmath>
//Class represents a vector in 3D space
class Vector3
{
public:
	//XYZ position
	float x;
	float y;
	float z;
	//RGB Values if a vertex is represented
	float r = 1.0f;
	float g = 0.0f;
	float b = 0.0f;
	//Default Constructor
	Vector3();
	//Constructor takes three positions x,y,z
	Vector3(float x, float y, float z);
	//Overload Function for +,-,*
	Vector3 operator +(Vector3 vector);
	Vector3 operator -(Vector3 vector);
	Vector3 operator *(Vector3 vector);
	Vector3 operator *(float value);
	//Returns a Dot product
	static int DotProduct(Vector3 vector_1, Vector3 vector_2);
	//Returns a Cross product of two passed vectors
	static Vector3 CrossProduct(Vector3 vector_1, Vector3 vector_2);
	//Normalize the passed vector
	static Vector3 NormalizeVector(Vector3 vector);
	//Magnitude of the passed vector
	static float Magnitude(Vector3 vector);
	//Function to turn an angles into radians
	static float radians(float degrees);
	//Default Destructor
	~Vector3();
};
#endif
