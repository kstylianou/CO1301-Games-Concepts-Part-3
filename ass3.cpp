// ass3.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iostream>
#include "hover.h"
#include "camera.h"
#include "Vector3.h"

#define STATE_STEDY         0
#define STATE_READY         1
#define STATE_GO            2
#define STATE_GAME_COMPLETE 3

using namespace tle;
using namespace std;

//functios that creates a matrix for direction
void getMatrix(IModel* hover, float& x, float& y, float& z)
{
	float matrix[16];
	hover->GetMatrix(matrix);
	x = matrix[8];
	y = matrix[9];
	z = matrix[10];
}

void GamerPosition(CHover& hover, IFont* myFont, IModel* checkpoint[]) {
	float x, y, z; // Get matrix for direction
	getMatrix(hover.GetModel(), x, y, z);


	int checkPointArr = hover.GetBotCheckpoints();
	if (checkPointArr == 6 || 5) {
		checkPointArr = 0;
	}
	//  1) Calculate vector from bot to checkpoint and player to checkpoint:
	CVector3 checkpointP(checkpoint[checkPointArr]->GetX(), checkpoint[checkPointArr]->GetY(), checkpoint[checkPointArr]->GetY());
	CVector3 hoverP(hover.GetModel()->GetLocalX(), hover.GetModel()->GetLocalY(), hover.GetModel()->GetLocalZ());
	CVector3 checkH = checkpointP - hoverP;
	CVector3 bot(hover.GetBotModel()->GetLocalX(), hover.GetBotModel()->GetLocalY(), hover.GetBotModel()->GetLocalZ());
	CVector3 checkB = checkpointP - bot;
	CVector3 c = bot - hoverP;
	CVector3 p(x, y, z);

	string str; // for myFont
	if (hover.GetLap() > hover.GetBotLap()) {
		str = "1st";
		hover.SetFirstPlayer(1);
	}
	else if (hover.GetLap() < hover.GetBotLap()) {
		str = "2nd";
		hover.SetFirstPlayer(2);
	}
	else if (hover.GetLap() == hover.GetBotLap() && hover.GetBotCheckpoints() < hover.GetCheckPoints()) {
		str = "1st";
		hover.SetFirstPlayer(1);
	}
	else if (hover.GetLap() == hover.GetBotLap() && hover.GetBotCheckpoints() > hover.GetCheckPoints()) {
		str = "2nd";
		hover.SetFirstPlayer(2);
	}
	else if (hover.GetLap() == hover.GetBotLap() && hover.GetBotCheckpoints() == hover.GetCheckPoints() && checkH.Dot(p, c) < checkB.Dot(p, c)) {
		str = "1st";
		hover.SetFirstPlayer(1);
	}
	else if (hover.GetLap() == hover.GetBotLap() && hover.GetBotCheckpoints() == hover.GetCheckPoints() && checkH.Dot(p, c) > checkB.Dot(p, c)) {
		str = "2nd";
		hover.SetFirstPlayer(2);
	}
	// Display player possition
	myFont->Draw(str, 50, 50, kRed, kCentre, kVCentre);
}

void collisionWall(CHover& hover, IModel* wall, float dt , float radius) {
	// Get the position of the wall + radius
	float xMax = wall->GetX() + radius;
	float xMin = wall->GetX() - radius;
	float yMax = wall->GetY() + radius;
	float yMin = wall->GetY() - radius;
	float zMax = wall->GetZ() + radius;
	float zMin = wall->GetZ() - radius;

	// get hover possition
	float x = hover.GetModel()->GetX();
	float y = hover.GetModel()->GetY();
	float z = hover.GetModel()->GetZ();

	//Collision detection
	// when is true it set the speed for ex +10 to - 10 or -10 + 10 to make it bounce
	if (x > xMin && x < xMax && y >= yMin && y< yMax && z > zMin && z < zMax) {
		hover.SetSpeed(-hover.GetSpeed());
		hover.UpdateHelth(true, dt);
	}
}

void collision(CHover& hover, IModel* wall, float dt, float radius, CVector3 vector) {
	// calculate the lenght of the hove and wall
	CVector3 x(hover.GetModel()->GetX(), hover.GetModel()->GetY(), hover.GetModel()->GetZ());
	CVector3 y(wall->GetX(), wall->GetY(), wall->GetZ());
	CVector3 z = x - y;

	// when is true it set the speed for ex +10 to - 10 or -10 + 10 to make it bounce
	if (z.Length() < hover.GetRedius() + radius) {
		hover.SetSpeed(-hover.GetSpeed());
		hover.UpdateHelth(true, dt);
	}
}


void updateGame(I3DEngine* myEngine, IFont* myFont, int& state, float& countdown, float& dt, IMesh* mCar, CHover& hover, CCamera& cam, IModel* cross[], IMesh* mCross, IModel* checkpoint[], int& botPosition, IMesh* mBot) {
	int time; // convert float to int to display 3,2,1
	string str; // String for the font
	int delayGo = 1;
	int countdownGo = 0;
	float countdownDefult = 3.0f;

	// if hover is on state completed game is finished
	if (hover.GetState() == STATE_COMPLETED) {
		state = STATE_GAME_COMPLETE;
	}

	switch (state)
	{
	case STATE_STEDY:
		// When user hit space is state is ready for countdown
		str = "Hit Space to Start";
		myFont->Draw(str, 650, 300, kRed, kCentre, kVCentre);
		if (myEngine->KeyHit(Key_Space)) {
			state = STATE_READY;
		}
		break;
	case STATE_READY:
		// Display 3, 2 ,1
		time = countdown + delayGo;
		str = to_string(time);
		countdown -= dt;
		myFont->Draw(str, 650, 300, kRed, kCentre, kVCentre);

		// when is 0 the player can move the hover 
		if (countdown <= countdownGo)
			state = STATE_GO;


		break;
	case STATE_GO:

		// it shows for about 1 second GO! aftr start
		if (countdown <= delayGo) {
			countdown += dt;
			str = "GO!";
			myFont->Draw(str, 650, 300, kRed, kCentre, kVCentre);

		}
		break;

	case STATE_GAME_COMPLETE:
		// When the game is completed display the winner and time player is able to restart the game (r)
		if (hover.GetWinner() == hover.GetPlayerNumWin()) {
			string str = "Player: 1st  Time:" + to_string(hover.GetPlayerTime()) + "\nBot: 2nd  Time: " + to_string(hover.GetBotTime());
			myFont->Draw(str, 650, 300, kRed, kCentre, kVCentre);
		}
		else {
			string str = "Bot: 1st  Time: " + to_string(hover.GetBotTime()) + "\nPlayer: 2nd  Time:" + to_string(hover.GetPlayerTime());
			myFont->Draw(str, 650, 300, kRed, kCentre, kVCentre);
		}
		break;
	}
	//Start tiemer for every frame
	dt = myEngine->Timer();

	// Stop the game
	if (myEngine->KeyHit(Key_Escape)) {
		myEngine->Stop();
	}

	// Restart the game
	if (myEngine->KeyHit(Key_R)) {
		hover.RestartGame(mCar, cross, mCross, checkpoint, mBot);
		countdown = countdownDefult;
		state = STATE_STEDY;
		cam.RestartGame(myEngine, hover.GetModel(), dt);
		botPosition = hover.GetDefultBotPossition();
	}
}

void updateScene(I3DEngine* myEngine, CHover& hover, CVector3& vector, IModel* isle[], IModel* wall[], float& dt, int& state, float& countdown, IFont* myFont, IFont* carFont, IModel* checkpoint[] , CCamera& cam, int kIsle, int kWall , IModel* tank[] , int kTank, IModel* tribune[], int Ktribune, IModel* tribune2 , IModel* tribune3, IModel* garage, IModel* dummy[], int kDummy, IModel* botDummy[], int kBotDummy, int& botPosition, IMesh* mCar, IModel* cross[], IMesh* mCross, IMesh* mBot, int isleRaduis, int wallRadius, int tankRadius, int tribuneRadius, int dummyRadius, int tribune2Radius, int tribune3Radius, int garageRadius) {

	//Update Game
	updateGame(myEngine, myFont, state, countdown, dt, mCar, hover, cam, cross, mCross, checkpoint, botPosition, mBot);

	//Update Camera
	cam.Update(myEngine, hover.GetModel(), dt);
	
	if (state == STATE_GO) {
		//Update Car
		hover.Update(myEngine, checkpoint, carFont, dt, cross, mCross);
		GamerPosition(hover, myFont, checkpoint);

		if (hover.UpdateBotMovements(myEngine, checkpoint, dt, botDummy[botPosition]->GetX(), botDummy[botPosition]->GetY(), botDummy[botPosition]->GetZ()) == true) {
			// calculate the lenght of the bot and dummy
			CVector3 b(hover.GetBotModel()->GetX(), hover.GetBotModel()->GetY(), hover.GetBotModel()->GetZ() + 0.1);
			CVector3 d(botDummy[botPosition]->GetX(), botDummy[botPosition]->GetY(), botDummy[botPosition]->GetZ());
			CVector3 l = d - b;
			int lenght = l.Length();
			// when lenght is 0 bot is moving to the next dummy if the next dummy is the 0 
			if (lenght == hover.GetDefultBotPossition()) {
				if (botPosition < kBotDummy - 1)
					botPosition++;
				else
					botPosition = hover.GetDefultBotPossition();
			}
		}
		
		// check fot collision hover and isle
		for (int i = 0; i < kIsle; i++) {
			collisionWall(hover, isle[i], dt, isleRaduis);
		}

		// check for collision hover and wall
		for (int i = 0; i < kWall; i++) {
			collisionWall(hover, wall[i], dt, wallRadius);
		}

		// check for collision hover and tank
		for (int i = 0; i < kTank; i++) {
			collision(hover, tank[i], dt, tankRadius, vector);
		}
	
		// check for collision hover and tribune
		for (int i = 0; i < Ktribune; i++) {
			collision(hover, tribune[i], dt, tribuneRadius, vector);
		}

		// check for collision hove and dummy (checkpoint legs)
		for (int i = 0; i < kDummy; i++) {
			collision(hover, dummy[i], dt, dummyRadius, vector);
		}

		// check for collision hover and tribune2
		collision(hover, tribune2, dt, tribune2Radius, vector);

		// check for collision hover and tribune3
		collision(hover, tribune3, dt, tribune3Radius, vector);

		// check for collision hover and garage
		collision(hover, garage, dt, garageRadius, vector);
	}
	

}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();
	//myEngine->StartFullscreen();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "./media" );

	/**** Set up your scene here ****/
	// Meshes:
	IMesh* mCar = myEngine->LoadMesh("race2.x");
	IMesh* mBot = myEngine->LoadMesh("race2-edit.x");
	IMesh* mSkybox = myEngine->LoadMesh("Skybox 07.x");
	IMesh* mGround = myEngine->LoadMesh("ground.x");
	IMesh* mCheckpoint = myEngine->LoadMesh("Checkpoint.x");
	IMesh* mIsle = myEngine->LoadMesh("IsleStraight.x");
	IMesh* mWall = myEngine->LoadMesh("Wall.x");
	IMesh* mTank = myEngine->LoadMesh("TankSmall2.x");
	IMesh* mCross = myEngine->LoadMesh("Cross.x");
	IMesh* mTribune = myEngine->LoadMesh("Tribune1.x");
	IMesh* mTribune2 = myEngine->LoadMesh("Tribune2.x");
	IMesh* mTribune3 = myEngine->LoadMesh("Tribune3.x");
	IMesh* mInterstellar = myEngine->LoadMesh("Interstellar.x");
	IMesh* mFlare = myEngine->LoadMesh("Flare.x");
	IMesh* mWalkway = myEngine->LoadMesh("Walkway.x");
	IMesh* mLamp = myEngine->LoadMesh("Lamp.x");
	IMesh* mGarage = myEngine->LoadMesh("GarageLarge.x");
	IMesh* mDummy = myEngine->LoadMesh("Dummy.x");
	IMesh* mSphere = myEngine->LoadMesh("Sphere.x");

	// Create the sprite at the bottom of the game
	ISprite* iBackdrop = myEngine->CreateSprite("ui_backdrop.jpg", 0, 660);
	ISprite* iBackdrop1 = myEngine->CreateSprite("ui_backdrop.jpg", 600, 660);

	// Load "Comic Sans MS" font at 70 points
	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 70);
	// Load "Comic Sans MS" font at 25 points
	IFont* carFont = myEngine->LoadFont("Comic Sans MS", 25);

	const int kTank = 22;       // Tank Array size
	const int kCheckpoint = 5;  // CheckPoint array size
	const int kIsle = 25;       // Isle array size
	const int kWall = 70;       // Wall array size
	const int Kwalkway = 41;    // Walkway size
	const int Ktribune = 3;		// Tribute Size
	const int kDummy = 10;      // Dummy size
	const int kBotDummy = 41;   // BotDummy size
	const int kCross = 5;		// Cross size

	// Models:
	IModel* skybox = mSkybox->CreateModel(0,-960,0);
	IModel* ground = mGround->CreateModel();
	IModel* checkpoint[kCheckpoint];
	IModel* isle[kIsle]; 
	IModel* wall[kWall];
	IModel* tank[kTank];
	IModel* tribune[Ktribune];
	IModel* cross[kCross];
	IModel* tribune2 = mTribune2->CreateModel(160, 0, 150);
	IModel* tribune3 = mTribune3->CreateModel(150, 0, -100);
	IModel* interstellar = mInterstellar->CreateModel(50 ,50 ,80);
	IModel* garage = mGarage->CreateModel(-100, 0, 250);
	garage->Scale(5);
	garage->RotateY(180);
	IModel* dummy[kDummy];
	IModel* botDummy[kBotDummy];
	IModel* walkway[Kwalkway];

	// Create and set position of checkpoints
	float dCheckPointZ[kCheckpoint] = { 0, 150 , 95 , 60 ,  -50 }; // Checkpoint Z
	float dCheckPointX[kCheckpoint] = { 0, 50 , 140 , 200 , 90};    // Checkpoint X
	float dCheckPointR[kCheckpoint] = { 0 , 90, 90 , 0, 90 };       // Checkpoint RotateY
	for (int i = 0; i < kCheckpoint; i++) {
		checkpoint[i] = mCheckpoint->CreateModel(dCheckPointX[i], 0, dCheckPointZ[i]);
		checkpoint[i]->RotateY(dCheckPointR[i]);
	}

	// Create and set position of cross
	for (int i = 0; i < kCross; i++) {
		cross[i] = mCross->CreateModel();
		cross[i]->AttachToParent(checkpoint[i]);
		cross[i]->Scale(0.2);
		cross[i]->MoveY(5);
	}

	// Create and set position of isle
	float dIsleX[kIsle] = { -30, -8,-40, -18, -25, -25, -38, -38, 10, 10 , 55,68, 50, 50, 116,116, 137 ,213, 215, 80, 80 ,14 , -21,14 , -21}; // Isle X
	float dIsleZ[kIsle] = { 40,50,53,63,150,130,150,130,130,150, 140,140, 165, 205, 205,113, 113 , 113,58, -35, -65 , -35 , -65, -5 ,1};          // Isle Z
	float dIsleR[kIsle] = { -35,-35,-35,-35, 90,90,90,90,90,90, 90,90 ,0,0, 0,0, 0 , 90, 0, 90 , 90 ,90 ,90 ,0 ,0};           // Isle RotateY
	for (int i = 0; i < kIsle; i++) {
		isle[i] = mIsle->CreateModel(dIsleX[i], 0, dIsleZ[i]);
		isle[i]->RotateY(dIsleR[i]);
	}

	// Create and set position of wall
	float dWallX[kWall] = { -35.5, -12.5, -31,-31,50,50,50,50,61 , 56, 65, 74, 83, 92, 101, 110, 116, 116, 116,116, 116, 116, 116, 116, 116,116, 122, 131,144 , 153 , 162,171,180 , 189, 198 , 207, 215, 215,215, 215,215, 215, 74 , 65,56,47 ,38,29,20 , 74,65,56,47,38,29 , 20 , 11 , 3 ,-6 , -15 , -21 , -21 , -21,-21,-21 , -21,-21 , 14 , 14, 14}; // Wall X
	float dWallZ[kWall] = { 46,56,130,150 ,171,180, 189, 198, 140, 205, 205, 205, 205,205, 205, 205, 198, 191, 182, 173, 164, 155, 146, 137, 128, 119, 113, 113, 113, 113, 113 , 113, 113,113,113, 113, 108 , 99 ,91 , 82,73,64 , -35, -35 , -35, -35, -35,-35,-35, -65, -65, -65, -65 ,-65 ,-65, -65 , -65 ,-65 ,-65 , -65 , -59, -50,-41,-32,-23, -14,-5 ,-29 , -20,-11};         // Wall Z
	float dWallR[kWall] = { -40,-40,90,90,0,0,0, 0, 90, 90, 90,90, 90, 90, 90, 90,0, 0, 0, 0, 0 , 0, 0 ,0, 0 , 0, 90,90 , 90 , 90 , 90, 90, 90, 90, 90 , 90, 0, 0 ,0 ,0,0,0, 90 , 90 , 90 , 90 , 90,90,90 ,90, 90 , 90, 90, 90 ,90 , 90,90,90,90, 90 , 0 , 0 , 0,0,0,0,0 };         // Wall RotateY
	for (int i = 0; i < kWall; i++) {
		wall[i] = mWall->CreateModel(dWallX[i], 0, dWallZ[i]);
		wall[i]->RotateY(dWallR[i]);
	}

	// Create and set position of tank
	float dTankX[kTank] = { -40,-80,-80, 75, 75, 75, 75, 75, 75,75, 90, 105, 120, 135, 150,170 , 180 , 160 , 200 , 90 , 130 , 180}; // Tank X
	float dTankY[kTank] = { 0,0,-5 , 0 , 0, 0 ,0, 0 ,0, 0 , 0, 0 , 0 , 0 , 0,0 , 0 , 0 , 0 , 0, 0 , -5};      // Tank Y
	float dTankZ[kTank] = { 100,100,160, 150, 165, 180, 135, 120, 105, 90, 80, 80,80, 80 , 80,80 , 70 , 30 , -10, -30, -60 , 10}; // Tank Z
	float dTankR[kTank] = { 0,0,10, 10, 0 , 0 , 0 , 0 , 0 ,0 , 0 ,0 ,0, 0 ,0,0  , 0 , 20 , 0 , 0 , 0 , 5};      // Tank RotateX
	for (int i = 0; i < kTank; i++) {
		tank[i] = mTank->CreateModel(dTankX[i], dTankY[i], dTankZ[i]);
		tank[i]->RotateX(dTankR[i]);
	}

	// Create and set position of walkway
	float dWalkWayX[Kwalkway] = { 0 , -10 , -25 , -40 , -55, -70, -70, -50 , - 30, -10  , 10 , 35 , 62 , 62 , 62 , 82 , 102 , 100 , 100 , 100 , 100 , 102 , 122, 145 , 165 , 185 , 200, 200 ,200 , 190, 175 ,155 , 135, 110, 85 , 65, 45, 25 , 0 , 0 , 0};
	float dWalkWayZ[Kwalkway] = { 15 , 35 , 55 , 75 , 95 , 115 , 140 , 140 , 140 , 140 , 140 , 145 , 155 , 175, 195 , 195 , 195 , 175 , 155 , 135 , 115 ,95 , 95 , 95 , 95 ,95 ,92, 72, 52 , 30, 8 , -14 , -35,-45, -50, -50, -50, -50, -50, -30, -10};
	float dWalkWayR[Kwalkway] = { -30 , -35 , -35 , -35 , -35, -0 , 90, 90 , 90 , 90, 90, 70 , 0 , 0 , 0 , 90 , 90 , 0 , 0 , 0 , 0 , 90 , 90 , 90 , 90 , 90 , 0, 0 , 0 , 45, 45 ,45 , 65 ,75, 90, 90, 90 , 90 , 0, 0 , 0};
	for (int i = 0; i < Kwalkway; i++) {
		walkway[i] = mWalkway->CreateModel(dWalkWayX[i], 0 , dWalkWayZ[i]);
		walkway[i]->RotateY(dWalkWayR[i]);
		walkway[i]->Scale(5);
		walkway[i]->SetSkin("floorMetal1.tga");
	}

	// Create and set position of botDummy
	float dBotDummyZ[kBotDummy] = { 15 , 35 , 55 , 75 , 95 , 115 , 140 , 140 , 140 , 140 , 140 , 145 , 155 , 175, 195 , 195 , 195 , 175 , 155 , 135 , 115 ,95 , 95 , 95 , 95 ,95 ,92, 72, 52 , 30, 8 , -14 , -35,-45, -50, -50, -50, -50, -50, -30, -10 };
	float dBotDummyX[Kwalkway] = { 0 , -10 , -25 , -40 , -55, -70, -70, -50 , -30, -10  , 10 , 35 , 62 , 62 , 62 , 82 , 102 , 100 , 100 , 100 , 100 , 102 , 122, 145 , 165 , 185 , 200, 200 ,200 , 190, 175 ,155 , 135, 110, 85 , 65, 45, 25 , 0 , 0 , 0 };
	for (int i = 0; i < kBotDummy; i++) {
		botDummy[i] = mDummy->CreateModel(dBotDummyX[i], 0, dBotDummyZ[i]);
	}

	// Create and set position of dummy (Collision on checkpoints)
	float dDummyX[kDummy] = { -10, 10, 50, 50 , 140, 140, 190, 210 , 90 , 90};
	float dDummyZ[kDummy] = { 0 , 0 , 140 , 160, 85,105, 60 ,60 , -40 , -60};
	for (int i = 0; i < kDummy; i++) {
		dummy[i] = mDummy->CreateModel(dDummyX[i], 0, dDummyZ[i]);
	}
	
	// Create and set position of tribune
	float dTribuneX[Ktribune] = { 50, -50, 100 };
	float dTribuneZ[Ktribune] = { 10, 10 , 250 };
	for (int i = 0; i < Ktribune; i++) {
		tribune[i] = mTribune->CreateModel(dTribuneX[i], 0, dTribuneZ[i]);
	}

	myEngine->Timer(); // Timer
	CHover hover(mCar, mBot);
	CCamera cam(myEngine, hover.GetModel());
	CVector3 vector;

	float dt = 0.0f; // Get timer
	int botPossition = 0; // Bot possition on dummys
	int state = STATE_STEDY; // Game State
	float countdown = 3.0f; // Countdown to start game
	int isleRaduis = 4;
	int wallRadius = 4;
	int tankRadius = 3;
	int tribuneRadius = 5;
	int dummyRadius = 1;
	int tribune2Radius = 20;
	int tribune3Radius = 25;
	int garageRadius = 40;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		
		/**** Update your scene each frame here ****/
		updateScene(myEngine, hover, vector, isle, wall, dt, state , countdown, myFont, carFont, checkpoint, cam , kIsle , kWall , tank, kTank, tribune , Ktribune , tribune2, tribune3, garage, dummy , kDummy , botDummy , kBotDummy, botPossition, mCar, cross, mCross, mBot, isleRaduis, wallRadius, tankRadius, tribuneRadius, dummyRadius, tribune2Radius, tribune3Radius,  garageRadius);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
