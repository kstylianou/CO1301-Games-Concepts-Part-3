// This is the declaration of the hover class.
//Kyriakos Stylianou 4/9/2020

#pragma once
#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#define STATE_RUNNING   0
#define STATE_COMPLETED 1
#define STATE_GAME_OVER 2

class CHover
{
public:
	//--------------------------------------------------//	
	// 1) Member Functions (a.k.a. "Methods"):			//
	//--------------------------------------------------//	
	// Constructor:
	CHover() {}	// Default constructor
	CHover(IMesh* meshCar, IMesh* mBot);	// User-defined constructor


	// Important Functions:
	void Initialise(IMesh* meshCar, IMesh* mBot);
	void Update(I3DEngine* myEngine, IModel* checkpoint[], IFont* myFont, float dt, IModel* cross[], IMesh* mCross);
	void UpdateHelth(bool collision, float dt);
	void RestartGame(IMesh* meshCar, IModel* cross[], IMesh* mCross, IModel* checkpoint[], IMesh* mBot);
	bool UpdateBotMovements(I3DEngine* myEngine, IModel* checkpoint[], float dt, float x, float y, float z);

	// Private Important Functions:
private:
	void UpdateMovements(I3DEngine* myEngine, float dt, IFont* myFont);
	bool StageCollision(IModel* checkPoint);
	void BotCollision(float dt);
	bool BotStageCollision(IModel* checkpoint);

public:
	//Getters:
	IModel* GetModel() { return m_Hover; }
	float GetSpeed() { return m_Speed; }
	float GetRedius() { return m_Radius; }
	float GetSteeringAngle() { return m_steeringAngle; }
	bool GetCollision() { return m_Bcollision; }
	float GetRotateAngleX() { return m_RotateAngleX; }
	IModel* GetBotModel() { return m_Bot; }
	int GetBotCheckpoints() { return botCountCheckPoints; }
	int GetCheckPoints() { return countCheckPoints; }
	int GetBotLap() { return botLap; }
	int GetLap() { return m_Hoverlap; }
	float GetMaxSpeed() { return m_MaxSpeed; }
	float GetBotTime() { return m_BotTime; }
	float GetPlayerTime() { return m_PlayerTime; }
	int GetState() { return m_State; }
	int GetWinner() { return m_firstPlayer; }
	int GetPlayerNumWin() { return m_PlayerNumWin; }
	int GetDefultBotPossition() { return m_DefultBotPossition; }

	// Setters:
	void SetCollision(bool i) { m_Bcollision = i; }
	void SetSpeed(float i) { m_Speed = i; }
	void SetFirstPlayer(int i) { m_firstPlayer = i; }

private:
	//--------------------------------------------------//	
	// 2) Member Variables:								//
	//--------------------------------------------------//	

	IModel* m_Hover;               // Car model
	IModel* m_Bot;
	float m_Speed;               // Car speed
	float m_MaxSpeed;
	float m_MinSpeed;
	float m_StepAngle;           // Steering speed
	float m_steeringAngle;
	float m_steeringAngleLimit;  // The steering limit
	float m_Radius;              // The radius of the wall + car
	int m_State;                 // The car state
	const int m_kStage = 6;      // CheckPoints Num
	bool m_Bstage[6];            // Bool for every checkpoint
	int countCheckPoints;
	bool m_Bcollision;
	float m_MaxRotateX;
	float m_RotateAngleX;
	float m_BoostMaxSpeed, m_BoostMinSpeed;
	bool m_overheat;
	float m_countOverheat;
	bool m_boost;
	float m_boostStep;
	float m_StepSpeed;
	float m_botSpeed;
	bool m_BotCollision;
	int m_HoverHelth;
	bool m_HelthCollision, m_StartCount;
	float coutSinceCollision;
	int m_Hoverlap;
	int botLap;
	int botCountCheckPoints;
	bool m_BotStage[6];
	bool m_crossRemoved[5];
	float m_CarRotateZ;
	float m_BotTime;
	float m_PlayerTime;
	int m_firstPlayer;
	float m_MaxSpeedRotateX, m_MinSpeedRotateX;
	float m_RotateXstep;
	float m_minRotateX;
	float m_MaxRotateXHalf;
	float m_maxRotateZ;
	float m_rotateZStep;
	float m_RotateZMin;
	float m_RotateZDefult;
	int m_defultOvertimeLimit;
	float m_defultOverhitTime;
	float m_TimeBoostAvailable;
	float m_SpeedDefult;
	float m_RotateXDefult;
	int m_CountingStagesStep;
	float m_botStepSpeed;
	int m_HelthOnHit;
	int m_GameOver;
	int m_kCross;
	float m_CrossScale;
	float m_MoveYCross;
	float m_SecondsAfterHit;
	int m_countCheckpointsDefult;
	int m_defultLenght;
	int m_defultYpossition;
	float m_steeringAngleDefult;
	int m_PlayerNumWin;
	int m_DefultBotPossition;
};

