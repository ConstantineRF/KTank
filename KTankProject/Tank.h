#pragma once

#include <SFML\Graphics.hpp>
#include <cmath>
#include "MapObject.h"
#include "Rocket.h"
#include <cstdlib>
#include <string>
#include "MapInterpretation.h"
#include "VisualEffect.h"

class TanksContainer;
class MachineBrain;
class RocketsContainer;

class Tank
{
public:
	Tank(const Vec2& inpos, int inkind, int inor, float inV, int inteam);
	Tank(const Vec2& inpos, int inkind, int inor, int inteam);
	void Draw(sf::RenderWindow& wnd) const;
	void SwitchOrientation(int ind);
	void SwitchOrientation();
	int GetOrientation() const;
	bool IsStill() const { return (abs(velocity.x) < 0.0001f) && (abs(velocity.y) < 0.0001f); };
	void Progress();
	void Stop();
	void Push(int ind);
	void Move(const MapObjectsContainer& mapobjects, const TanksContainer* tanksptr);
	void Refill() { gas = V; /* velocity.GetLength(); */};
	void TryShooting(RocketsContainer& rockets);
	void AbsorbHit(int kind=1);
	void Annihilate();
	void ConsumeRevolver() { revolvertimer = 500; };
	void ConsumeShield() { shieldtimer = 500; };
	int RevolverTimer() { return revolvertimer; }
	int ShieldTimer() { return shieldtimer; }
	float GetX() const { return pos.x; };
	float GetY() const { return pos.y; };
	int GetTeam() const { return team; };
	int GetKind() const { return kind; };
	float GetGasLeft() const { return gas; };
	bool IsLive() const { return islive; };
	bool IsShieldOn() const { return shieldtimer > 0; };
	Vec2 GetPos() const { return pos; };
	static const Vec2 dir2vec(int d);
private:
	Vec2 pos;
	int orientation; // direction of current position: 1 right, 2 up, 3 left, 4 down
	float V; // speed potential
	int dpush; // direction of current instruction to move
	Vec2 velocity;  // vector coordinates of direction of current instruction to move
	float gas; // portion of speed left for this turn
	int team; // team=1 is player, team=-1 is enemy
	int kind; // 1=blue, 2=red, 3=green
	int life; 
	bool islive;
	int rechargetimer, rechargeduration;
	int freezetimer;
	int shieldtimer;
	int revolvertimer;
public:
	MachineBrain* ai;
};



class TanksContainer
{
public:
	~TanksContainer();
	void CreateTank(const Vec2& inpos, int inor, float inV, int inteam);
	int GetTanksNumber() const;
	int GetTeamTanksNumber(int inteam) const;
	void BlowUpTeam(int team, VisualEffectsContainer & effects, RectF area = RectF(Vec2(0, 0), Vec2(600, 600)));
	Tank* GetTankAtLocation(const Vec2& inpos);
	bool NoTankHere(const Vec2& inpos) const;
	void Refill();
	void Progress();
	void ProcessUserControls(sf::RenderWindow& wnd, RocketsContainer& rockets, const MapInterpretation& mapinterpretation, int players);
	void Move(const MapObjectsContainer& mapobjects);
	void Draw(sf::RenderWindow& wnd) const;
	void MakeEmpty();
public:  // tanks are revealed intentionally
	std::list<Tank*> tanks;
	static sf::Texture basictankbitmap;
	static sf::Texture bluetankbitmap;
	static sf::Texture redtankbitmap;
	static sf::Texture greentankbitmap;
};



class MachineBrain
{
public:
	MachineBrain(Tank* intank);
	int GetPushDirection(const MapInterpretation & mapinterpretation);
	void HitWall(int inpush);
private:
	int patience;
	int preferreddirection;
	int followingalternativedirectioncounter;
	Tank* tank;
	// We are okay with default destructor, because the pointer to tank isn't where the tank is created.
};







