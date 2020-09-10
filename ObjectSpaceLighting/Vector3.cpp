#include "Vector3.h"
//Default Constructor
Vector3::Vector3()
{

}
//Constructor takes three positions x,y,z
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
//Overload Function for +
Vector3 Vector3::operator+(Vector3 vector)
{
	return Vector3(x + vector.x, y + vector.y, z + vector.z);
}
//Overload Function for -
Vector3 Vector3::operator-(Vector3 vector)
{
	return Vector3(x - vector.x, y - vector.y, z - vector.z);
}
//Overload Function for *
Vector3 Vector3::operator*(Vector3 vector)
{
	return Vector3(x * vector.x, y * vector.y, z * vector.z);
}
//Overload Function for * with a float value
Vector3 Vector3::operator*(float value)
{

	return Vector3(x*value, y*value, z*value);
}
//Returns a Dot product
int Vector3::DotProduct(Vector3 vector_1, Vector3 vector_2)
{
	return (vector_1.x*vector_2.x + vector_1.y*vector_2.y + vector_1.z*vector_2.z);
}
//Returns a Cross product of two passed vectors
Vector3 Vector3::CrossProduct(Vector3 vector_1, Vector3 vector_2)
{
	return Vector3(vector_1.y*vector_2.z - vector_1.z*vector_2.y, vector_1.z*vector_2.x - vector_1.x*vector_2.z, vector_1.x*vector_2.y - vector_1.y*vector_2.x);
}
//Normalize the passed vector
Vector3 Vector3::NormalizeVector(Vector3 vector)
{
	float magnitude = (float)Magnitude(vector);
	Vector3 normalVector = Vector3(vector.x / magnitude, vector.y / magnitude, vector.z / magnitude);
	return normalVector;
}
//Magnitude of the passed vector
float Vector3::Magnitude(Vector3 vector)
{
	float magnitude = sqrt(pow((float)vector.x, 2) + pow((float)vector.y, 2) + pow((float)vector.z, 2));
	return magnitude;
}
//Function to turn an angles into radians
float Vector3::radians(float degrees)
{
	return degrees * (0.01745329251994329576923690768489f);
}
//Default Destructor
Vector3::~Vector3()
{
}