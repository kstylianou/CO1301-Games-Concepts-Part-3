// Declaration of the CVector3 class
// Kyriakos Stylianou 9/4/2020

#pragma once

#include <math.h>

//TODO
// 2) Do dot product function
class CVector3
{
public:
	// 1) Member methods:
	
	// Constructors:
	CVector3(){}	// Default constructor
	CVector3(float x, float y, float z);

	float Length() { return sqrt(m_x * m_x + m_y * m_y + m_z * m_z); }
	float Dot(CVector3 a, CVector3 b) { return a.GetX() * m_x + a.GetY() * m_y + a.GetZ() * m_z; }

	// Operators:
	CVector3 operator-(CVector3 a);
	CVector3 operator*(float a);
	CVector3 operator/(float a);

	// Getters:
	float GetX() { return m_x; }
	float GetY() { return m_y; }
	float GetZ() { return m_z; }

	// Setters:
	void SetX(float x) { m_x = x; }
	void SetY(float y) { m_y = y; }
	void SetZ(float z) { m_z = z; }

private:
	// 2) Member data:
	float m_x, m_y, m_z;
};

