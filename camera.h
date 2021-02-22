// This is the declaration of the camera class.
//Kyriakos Stylianou 4/9/2020

#pragma once

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

class CCamera
{
public:
	//--------------------------------------------------//	
	// 1) Member Functions (a.k.a. "Methods"):			//
	//--------------------------------------------------//	
	// Constructor:
	CCamera() {}	// Default constructor
	CCamera(I3DEngine* myEngine, IModel* car);	// User-defined constructor


	// Important Functions:
	void Initialise(I3DEngine* myEngine, IModel* car);
	void Update(I3DEngine* myEngine, IModel* Car, float dt);
	void CamMovements(I3DEngine* myEngine , float dt);
	void RestartGame(I3DEngine* myEngine, IModel* car, float dt);



	// Setters
	void SetrXpossition(float i) { m_rXpossition = i; };

private:
	//--------------------------------------------------//	
	// 2) Member Variables:								//
	//--------------------------------------------------//	
	ICamera* m_Cam; // Camera
	int m_State; // Camera state
	float m_Step; // Camera Step
	float m_AngleLR; // Camera rotation Left, Right
	float m_AngleUD; // Camera rotation UP, Down
	int m_FirstPerson, m_ThirdPerson, m_FreeMoving, m_Surveillance;
	float m_camPositionY;
	float m_CamPositionX;
	float m_rXpossition;
	float m_firstPersonY;
	float m_ThirtPersonX , m_ThirtPersonY, m_ThirdPresonZ;
	float m_SurveillanceX, m_SurveillanceY, m_SurveillanceZ;
	float m_ThirtPersonPositionY;
};

