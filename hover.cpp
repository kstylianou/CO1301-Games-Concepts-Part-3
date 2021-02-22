// This is the implementation of the hover class.
//Kyriakos Stylianou 4/9/2020

#include "hover.h"
#include "Vector3.h"
#include <TL-Engine.h>
#include <iostream>


using namespace tle;

CHover::CHover(IMesh* meshCar, IMesh* mBot)
{
	Initialise(meshCar, mBot);
}

void CHover::Initialise(IMesh* meshCar, IMesh* mBot)
{
	m_Hover = meshCar->CreateModel(-5, 0, -20); // Load Model
	m_Bot = mBot->CreateModel(5, 0, -30);

	m_Speed = 0.0f; // Set the speed of the car
	m_botSpeed = 0.0f;
	m_MaxSpeed = 50.0f;
	m_BoostMaxSpeed = 100.0f;
	m_MinSpeed = 25.0f;
	m_BoostMinSpeed = 50.0f;
	m_StepAngle = 1.0f; // The agle when steering
	m_steeringAngle = 0.0f; 
	m_steeringAngleLimit = 90; // The max steering
	m_Radius = 4.0f;
	m_State = STATE_RUNNING;
	countCheckPoints = 0;
	m_MaxRotateX = 40.0f;
	m_MaxRotateXHalf = 20.0f;
	m_RotateAngleX = 0.0f;
	m_boost = false;
	m_overheat = false;
	m_countOverheat = 4.0f;
	m_BotCollision = false;
	m_HoverHelth = 100;
	m_HelthCollision = false;
	coutSinceCollision = 0.0f;
	m_StartCount = false;
	m_Hoverlap = 1;
	botCountCheckPoints = 0;
	botLap = 1;
	m_CarRotateZ = 0.0f;
	m_PlayerTime = 0.0f;
	m_BotTime = 0.0f;
	m_boostStep = 0.5;
	m_StepSpeed = 0.5;
	m_MaxSpeedRotateX = 30.0;
	m_MinSpeedRotateX = 10.0;
	m_RotateXstep = 0.5;
	m_minRotateX = 0.0;
	m_maxRotateZ = 40.0f;
	m_rotateZStep = 0.5f;
	m_RotateZMin = 0.1f;
	m_RotateZDefult = 0.0;
	m_defultOvertimeLimit = 0;
	m_defultOverhitTime = 6.0;
	m_TimeBoostAvailable = 0.4;
	m_SpeedDefult = 0.0;
	m_RotateXDefult = 0.1;
	m_CountingStagesStep = 1;
	m_botStepSpeed = 0.1;
	m_HelthOnHit = 5;
	m_GameOver = 0;
	m_kCross = 5;
	m_CrossScale = 0.2;
	m_MoveYCross = 5.0;
	m_SecondsAfterHit = 2.0f;
	m_countCheckpointsDefult = 0;
	m_defultLenght = 0;
	m_defultYpossition = 0;
	m_steeringAngleDefult = 0.0f;
	m_PlayerNumWin = 1;
	m_DefultBotPossition = 0;

	for (int i = 0; i < m_kStage; i++) {
		m_Bstage[i] = false;
		m_BotStage[i] = false;
		m_crossRemoved[i] = false;
	}
}


void CHover::Update(I3DEngine* myEngine, IModel* checkpoint[], IFont* myFont, float dt, IModel* cross[], IMesh* mCross) {
	string str;
	int speed = m_Speed;
	int count = 0;
	switch (m_State)
	{
	case STATE_RUNNING:
		// Private Functions Movement and BotCollision
		UpdateMovements(myEngine, dt, myFont);
		BotCollision(dt);

		// Count second for helath after collision
		if (m_StartCount == true) {
			coutSinceCollision += dt;
			if (coutSinceCollision > m_SecondsAfterHit) {
				coutSinceCollision = m_countCheckpointsDefult;
				m_StartCount = false;
			}
		}

		// Remove cross when there is collision with checkpoint
		for (int i = 0; i < m_kStage; i++) {
			if (i < m_kCross) {
				if (StageCollision(checkpoint[i]) == true) {
					m_Bstage[i] = true;
					if (m_crossRemoved[i] == false) {
						cross[i]->DetachFromParent();
						mCross->RemoveModel(cross[i]);
						m_crossRemoved[i] = true;
					}
				}
			}
			else {
				if (m_Bstage[0, 1, 2, 3, 4] == true)
					m_Bstage[i] = StageCollision(checkpoint[0]);
			}

			if (m_Bstage[i]) {
				count += m_CountingStagesStep;
			}
		}


		countCheckPoints = count;

		// Display Checkpoints has passed
		str = "Checkpoint: " + to_string(countCheckPoints) + "/5";
		myFont->Draw(str, 100, 680, kRed, kCentre, kVCentre);

		// Display lap player has been to
		str = "Lap: " + to_string(m_Hoverlap) + "/5";
		myFont->Draw(str, 900, 680, kRed, kCentre, kVCentre);

		// When the all checkpoints reached change lap and create new cross
		if (countCheckPoints == m_kStage) {
			for (int i = 0; i < m_kStage; i++) {
				m_Bstage[i] = false;
				m_crossRemoved[i] = false;
				if (i < m_kCross) {
					cross[i] = mCross->CreateModel();
					cross[i]->AttachToParent(checkpoint[i]);
					cross[i]->Scale(m_CrossScale);
					cross[i]->MoveY(m_MoveYCross);
				}

			}
			countCheckPoints = m_countCheckpointsDefult;
			m_Hoverlap++;
		}

		// when all lap has been passed Completed 
		if (m_Hoverlap == m_kStage) {
			m_State = STATE_COMPLETED;
		}

		// Display the speed of hover
		str = "Speed: " + to_string(speed);
		myFont->Draw(str, 400, 680, kRed, kRight, kVCentre);

		// Display helath of hover
		str = "Health: " + to_string(m_HoverHelth);
		myFont->Draw(str, 1200, 680, kRed, kRight, kVCentre);
		break;

	case STATE_COMPLETED:
		// Display to the user that has completed the game
		str = "Completed!";
		myFont->Draw(str, 200, 680, kRed, kCentre, kVCentre);
		break;


	case STATE_GAME_OVER:
		// Display when game over
		string str = "GAME OVER PRESS (R) TO RESTART ";
		myFont->Draw(str, 500, 580, kRed, kRight, kVCentre);
		break;
	}
}

void CHover::UpdateMovements(I3DEngine* myEngine, float dt, IFont* myFont) {
	//Update car movements

	//Start Player Time;
	m_PlayerTime += dt;

	// Velocity of hover Speed with timer
	float ds = m_Speed * dt;
	string str;
	//When key held W  pushes the car forward
	if (myEngine->KeyHeld(Key_W)) {
		
		// when boost is off go slower and step is smaller
		if (m_Speed < m_MaxSpeed && m_boost == false) {
			m_Speed += m_StepSpeed;
		}

		// when boost is on it goes faster and with bigger step
		if (m_Speed < m_BoostMaxSpeed && m_boost == true) {
			m_Speed += m_boostStep;
		}

		// when overheat is on it goes fast on normal speed
		if (m_overheat == true) {
			if (m_Speed > m_MaxSpeed) {
				m_Speed -= m_boostStep;
			}
		}
		// or when no key held space and overheat is false
		else if (m_boost == false) {
			if (m_Speed > m_MaxSpeed) {
				m_Speed -= m_boostStep;
			}
		}

		// move the hove and rotate it
		m_Hover->MoveLocalZ(ds);
		m_Hover->RotateY(m_steeringAngle * dt);
		
		//make the hover to gently wobble up and down
		// when the speed is more than 30 the car is watching to the ground
		if (m_Speed > m_MaxSpeedRotateX) {
			if (m_RotateAngleX < m_MaxRotateX) {
				m_RotateAngleX += m_RotateXstep;
				if (m_RotateAngleX < m_minRotateX)
					m_Hover->RotateLocalX(-m_RotateAngleX * dt);
				else
					m_Hover->RotateLocalX(m_RotateAngleX * dt);
			}
		}
		// when the speed is more than 10 and less tha 30 the car is watching to the sky
		else if(m_Speed > m_MinSpeedRotateX) {
			if (m_RotateAngleX > -m_MaxRotateXHalf) {
				m_RotateAngleX -= m_RotateXstep;
				if (m_RotateAngleX > m_minRotateX)
					m_Hover->RotateLocalX(-m_RotateAngleX * dt);
				else
					m_Hover->RotateLocalX(m_RotateAngleX * dt);
			}
		}
	}

	//When key held S  pushes the car backward
	if (myEngine->KeyHeld(Key_S)) {
		// when the boost is off it goes slower and smaller step
		if (m_Speed > -m_MaxSpeed && m_boost == false) {
			m_Speed -= m_boostStep;
		}

		// when boost is on it goes faster and with bigger step
		if (m_Speed > -m_BoostMinSpeed && m_boost == true) {
			m_Speed -= m_StepSpeed;
		}

		// when overheat is on it goes fast on normal speed
		if (m_overheat == true) {
			if (m_Speed < -m_MaxSpeed) {
				m_Speed += m_boostStep;
			}
		}
		// or when no key held space and overheat is false
		else if (m_boost == false) {
			if (m_Speed < -m_MaxSpeed) {
				m_Speed += m_boostStep;
			}
		}

		// move the hove and rotate it
		m_Hover->MoveLocalZ(ds);
		m_Hover->RotateY(-m_steeringAngle * dt);
	
		//make the hover to gently wobble up and down
		if (m_RotateAngleX > -m_MaxRotateXHalf) {
			m_RotateAngleX -= m_RotateXstep;
			if (m_RotateAngleX > m_minRotateX)
				m_Hover->RotateLocalX(-m_RotateAngleX * dt);
			else
				m_Hover->RotateLocalX(m_RotateAngleX * dt);
		}
	}

	//When key held D  rotate the car clockwise
	if (myEngine->KeyHeld(Key_D)) {
		//Steering hover 
		if (m_steeringAngle <= m_steeringAngleLimit)
			m_steeringAngle += m_StepAngle;

		// make the hover lean into the bends as it turns
		if (m_CarRotateZ > -m_maxRotateZ) {
			m_CarRotateZ -= m_rotateZStep;
			if (m_CarRotateZ > m_RotateZDefult)
				m_Hover->RotateLocalZ(-m_CarRotateZ * dt);
			else
				m_Hover->RotateLocalZ(m_CarRotateZ * dt);
		}
	}

	//When key held A  rotate the car anti-clockwise
	if (myEngine->KeyHeld(Key_A)) {
		//Steering hover 
		if (m_steeringAngle >= -m_steeringAngleLimit)
			m_steeringAngle -= m_StepAngle;

		// make the hover lean into the bends as it turns
		if (m_CarRotateZ < m_maxRotateZ) {
			m_CarRotateZ += m_rotateZStep;
			if(m_CarRotateZ < m_RotateZDefult)
				m_Hover->RotateLocalZ(-m_CarRotateZ * dt);
			else
				m_Hover->RotateLocalZ(m_CarRotateZ * dt);
		}
	}

	// when key held space boost is active if not over heated
	if (myEngine->KeyHeld(Key_Space) && m_overheat == false) {
		m_boost = true;
		m_countOverheat -= dt;
		int count = m_countOverheat;
		// if the time is equal to 0 OVERHEAT
		if (count == m_defultOvertimeLimit) {
			m_boost = false;
			m_overheat = true;
			m_countOverheat = m_defultOverhitTime;
		}
		//Display the time to overheat
		str = "Overhit in: " + to_string(count);
		myFont->Draw(str, 500, 680, kRed, kCentre, kVCentre);
	}

	// when not space is key held boost is off 
	if (!myEngine->KeyHeld(Key_Space) && m_boost == true) 
		m_boost = false;
	
	// display boost ready if is ready
	if (!myEngine->KeyHeld(Key_Space) && m_boost == false && m_overheat == false) {
		str = "Boost Ready!";
		myFont->Draw(str, 600, 680, kRed, kCentre, kVCentre);
	}

	// when overheat start counting whhen is 0 boost is available
	if (m_overheat == true) {
		m_countOverheat -= dt;
		int count = m_countOverheat;
		if (count <= m_defultOvertimeLimit) {
			m_overheat = false;
			m_countOverheat = m_TimeBoostAvailable;
		}

		// Display seconds boost to be ready
		str = "Boost available in: " + to_string(count);
		myFont->Draw(str, 600, 680, kRed, kCentre, kVCentre);
	}


	// if not steering set to 0 to not keep steering
	if (!myEngine->KeyHeld(Key_D) && !myEngine->KeyHeld(Key_A)) {
		m_steeringAngle = m_steeringAngleDefult;
		
		// when no steering is not bending
		double rotateZ = m_CarRotateZ;
		if (rotateZ != m_RotateZDefult) {
			if (rotateZ < -m_RotateZMin) {
				m_CarRotateZ += m_rotateZStep;
				m_Hover->RotateLocalZ(-m_CarRotateZ * dt);
			}
			else if (rotateZ > m_RotateZMin) {
				m_CarRotateZ -= m_rotateZStep;
				m_Hover->RotateLocalZ(-m_CarRotateZ * dt);
			}
			else {
				m_CarRotateZ = m_RotateZDefult;
			}
		}
	}

	// When not key press to move the hover
	if (!myEngine->KeyHeld(Key_W) && !myEngine->KeyHeld(Key_S)) {
		int speed = m_Speed; // Convert to int to check the equality float is never equal
		// Set the speed to defult
		if (speed == m_SpeedDefult) {
			m_Speed = m_SpeedDefult;
		}
		if (m_Speed > m_SpeedDefult) {
			m_Speed -= m_StepSpeed;
			m_Hover->MoveLocalZ(m_Speed * dt);
		}
		if (m_Speed < m_SpeedDefult) {
			m_Speed += m_StepSpeed;
			m_Hover->MoveLocalZ(m_Speed * dt);
		}
		

		double rotateX = m_RotateAngleX; // Convert to double to check the equality float is never equal
		// Set rotateX to defult when is not moving
		if (rotateX != m_minRotateX) {
			if (m_RotateAngleX > m_RotateXDefult) {
				m_RotateAngleX -= m_RotateXstep;
				m_Hover->RotateLocalX(-m_RotateAngleX * dt);
			}
			else if (m_RotateAngleX < -m_RotateXDefult) {
				m_RotateAngleX += m_RotateXstep;
				m_Hover->RotateLocalX(-m_RotateAngleX * dt);
			}
			else {
				m_RotateAngleX = m_minRotateX;
			}
		}
	}

	//keep the hover on the ground when rotate local
	m_Hover->SetY(m_defultYpossition);
}

bool CHover::UpdateBotMovements(I3DEngine* myEngine, IModel* checkpoint[], float dt, float x , float y, float z) {
	int count = 0;

	// count the checkpoint the bot go through and lab
	for (int i = 0; i < m_kStage; i++) {
		if (i < m_kStage - m_CountingStagesStep) {
			if (BotStageCollision(checkpoint[i]) == true) {
				m_BotStage[i] = true;
			}
		}
		else {
			if(m_BotStage[0, 1 , 2, 3 , 4] == true)
				m_BotStage[i] = BotStageCollision(checkpoint[0]); // Repeat checkpoint 0 to complete the lap
		}
		if (m_BotStage[i]) {
			count += m_CountingStagesStep;
		}
	}

	botCountCheckPoints = count;

	// when all stages are complete chenge lap
	if (botCountCheckPoints == m_kStage ) {
		if (m_BotStage[0] == true)
			botLap++;
		// set everything to false to repeat the stages again
		for (int i = 0; i < m_kStage; i++) {
			m_BotStage[i] = false;
		}
		
	}

	// while bot lap is no finished count time since start
	if (botLap < m_kStage) {
		m_BotTime += dt;
	}
	
	// Speed up slowly when start and on collision
	if (m_botSpeed < m_MaxSpeed) {
		m_botSpeed += m_botStepSpeed;
	}

	// bot is looking at dummy when moving
	m_Bot->LookAt(x, m_Bot->GetY(), z);	

	//  1) Calculate vector from bot to dummy:
	CVector3 h(x, y, z);
	CVector3 b(m_Bot->GetX(), m_Bot->GetY(), m_Bot->GetZ() + 0.1);
	CVector3 c = h - b;

	//  3) Calculate sDir:
	float sDirx = c.GetX() / c.Length();
	float sDiry = c.GetY() / c.Length();
	float sDirz = c.GetZ() / c.Length();


	// 4) Apply movement to bot:
	float ds = m_botSpeed * dt;
	if(m_BotCollision == false)
		m_Bot->Move(sDirx*ds, sDiry*ds, sDirz*ds);

	//Return the lenght to int to move on next dummy when lenght is 0
	int lenght = c.Length();
	return (lenght == m_defultLenght);
	
}

// Stage collision for the hover
bool CHover::StageCollision(IModel* checkpoint) {
	//  1) Calculate vector from hover to checkpoint:
	CVector3 x(m_Hover->GetX(), m_Hover->GetY(), m_Hover->GetZ());
	CVector3 y(checkpoint->GetX(), checkpoint->GetY(), checkpoint->GetZ());
	CVector3 z = x - y;
	// Return true when lenght is less than radius
	return (z.Length() < m_Radius + m_Radius);
}

// Stage collision for the bot
bool CHover::BotStageCollision(IModel* checkpoint) {
	//  1) Calculate vector from bot to checkpoint:
	CVector3 x(m_Bot->GetX(), m_Bot->GetY(), m_Bot->GetZ());
	CVector3 y(checkpoint->GetX(), checkpoint->GetY(), checkpoint->GetZ());
	CVector3 z = x - y;
	// Return true when lenght is less than radius
	return (z.Length() < m_Radius);
}

// Collision with bot and hover
void CHover::BotCollision(float dt) {
	//  1) Calculate vector from bot and hover:
	CVector3 x(m_Hover->GetX(), m_Hover->GetY(), m_Hover->GetZ());
	CVector3 y(m_Bot->GetX(), m_Bot->GetY(), m_Bot->GetZ());
	CVector3 z = x - y;

	// Change the speed to - or + to make it bounce whe collision
	if (z.Length() < m_Radius + m_Radius) {
		m_botSpeed = -m_botSpeed;
		m_Speed = -m_Speed;
		UpdateHelth(true, dt); // Update helth of the hover 
	}
}

// Update hover helth on collision
void CHover::UpdateHelth(bool collision, float dt) {
	
	// Count time since the collision to not keep remove helth on collision
	if (collision == true && m_StartCount == false) {
		m_HoverHelth -= m_HelthOnHit;
		m_StartCount = true;
	}
	// when helth is 0 game over
	if (m_HoverHelth <= m_GameOver) {
		m_State = STATE_GAME_OVER;
	}
}

// When (R) restart game
void CHover::RestartGame(IMesh* meshCar, IModel* cross[], IMesh* mCross, IModel* checkpoint[], IMesh* mBot) {
	// Remove Models (Bot/hover)
	meshCar->RemoveModel(m_Hover);
	mBot->RemoveModel(m_Bot);

	// Remove exist cross and create new ones
	for (int i = 0; i < m_kCross; i++) {
		if (m_crossRemoved[i] == false){
			cross[i]->DetachFromParent();
			mCross->RemoveModel(cross[i]);
		}
	
		cross[i] = mCross->CreateModel();
		cross[i]->AttachToParent(checkpoint[i]);
		cross[i]->Scale(m_CrossScale);
		cross[i]->MoveY(m_MoveYCross);

	}

	//Initialise everything to defult
	Initialise(meshCar, mBot);
}



