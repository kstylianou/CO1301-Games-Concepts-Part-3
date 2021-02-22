#include "camera.h"
#include <iostream>
#include <TL-Engine.h>

using namespace tle;

CCamera::CCamera(I3DEngine* myEngine, IModel* car)
{
	Initialise(myEngine, car);
}

void CCamera::Initialise(I3DEngine* myEngine, IModel* car) {
	m_Cam = myEngine->CreateCamera(kManual);
	m_Cam->AttachToParent(car);

	m_State = 3;
	m_Step = 10;
	m_FirstPerson = 1;
	m_FreeMoving = 2;
	m_ThirdPerson = 3;
	m_Surveillance = 4;
	m_camPositionY = 0.0f;
	m_firstPersonY = 6.0f;
	m_ThirtPersonX = 0.0f;
	m_ThirtPersonY = 10.0f;
	m_ThirdPresonZ = -30.0f;
	m_SurveillanceX = -60.0f;
	m_SurveillanceY = 60.0f;
	m_SurveillanceZ = 60.0f;
	m_ThirtPersonPositionY = 10.0f;
}

void CCamera::Update(I3DEngine* myEngine, IModel* car, float dt) {
	CamMovements(myEngine, dt);

	if (myEngine->KeyHit(Key_1)) {
		m_Cam->AttachToParent(car);
		m_Cam->SetPosition(car->GetLocalX(), car->GetLocalY(), car->GetLocalZ());
		m_Cam->MoveY(m_firstPersonY);
		m_Cam->ResetOrientation();
		m_State = m_FirstPerson;
		m_camPositionY = m_Cam->GetY();
	}

	if (myEngine->KeyHit(Key_2)) {
		m_State = m_FreeMoving;
		m_Cam->DetachFromParent();
		m_camPositionY = m_Cam->GetY();
	}

	if (myEngine->KeyHit(Key_3)) {
		m_Cam->AttachToParent(car);
		m_Cam->SetPosition(car->GetLocalX(), car->GetLocalY(), car->GetLocalZ());
		m_Cam->Move(m_ThirtPersonX, m_ThirtPersonY, m_ThirdPresonZ);
		m_State = m_ThirdPerson;
		m_camPositionY = m_Cam->GetLocalY();
	}

	if (myEngine->KeyHit(Key_4)) {
		m_State = m_Surveillance;
		m_Cam->SetPosition(m_SurveillanceX, m_SurveillanceY, m_SurveillanceZ);
		m_Cam->LookAt(car);
		m_camPositionY = m_Cam->GetY();
	}

	if (m_State == m_Surveillance) {
		m_Cam->LookAt(car);
	}
	if (myEngine->KeyHit(Key_C)) {
		if (m_State == m_FirstPerson) {
			m_Cam->AttachToParent(car);
			m_Cam->SetPosition(car->GetLocalX(), car->GetLocalY(), car->GetLocalZ());
			m_Cam->MoveY(m_firstPersonY);
			m_Cam->ResetOrientation();
		}
		if (m_State == m_ThirdPerson) {
			m_Cam->AttachToParent(car);
			m_Cam->DetachFromParent();
			m_Cam->SetPosition(car->GetLocalX(), car->GetLocalY(), car->GetLocalZ());
			m_Cam->Move(m_ThirtPersonX, m_ThirtPersonY, m_ThirdPresonZ);
			m_Cam->LookAt(car);
			m_camPositionY = m_Cam->GetLocalY();
		}
		
		if (m_State == m_Surveillance) {
			m_Cam->SetPosition(m_SurveillanceX, m_SurveillanceY, m_SurveillanceZ);
			m_Cam->LookAt(car);
			
		}
		m_camPositionY = m_Cam->GetY();
	}
	if (m_State == m_ThirdPerson) {
		m_Cam->SetY(m_ThirtPersonPositionY);
		m_Cam->ResetOrientation();

	}
}	

void CCamera::CamMovements(I3DEngine* myEngine , float dt) {
	// Keys Control Movement:
	// The ‘UP’ key causes the camera to move forwards
	if (myEngine->KeyHeld(Key_Up))
		m_Cam->MoveLocalZ(m_Step * dt);

	// The ‘DOWN’ key causes the camera to move backwards.
	if (myEngine->KeyHeld(Key_Down))
		m_Cam->MoveLocalZ(-m_Step * dt);

	// The ‘Left’ key causes the camera to move leftwards.
	if (myEngine->KeyHeld(Key_Left))
		m_Cam->MoveLocalX(-m_Step * dt);

	// The ‘Right’ key causes the camera to move rightwards.
	if (myEngine->KeyHeld(Key_Right))
		m_Cam->MoveLocalX(m_Step * dt);

	//Left-Right Rotations:
	m_AngleLR = myEngine->GetMouseMovementX();
	m_Cam->RotateY(m_AngleLR);

	//Up-Down Rotation:
	m_AngleUD = myEngine->GetMouseMovementY();
	m_Cam->RotateLocalX(m_AngleUD);

}

void CCamera::RestartGame(I3DEngine* myEngine, IModel* car, float dt) {
	float countdown = 1.0f;
	countdown -= dt;
	if(countdown < 0)
		Initialise(myEngine, car);
}