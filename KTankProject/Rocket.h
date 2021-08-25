#pragma once
#include <cmath>
#include <list>
#include "VisualEffect.h"
#include "Tank.h"
#include "MapObject.h"
#include "Drawing.h"

class RocketContainer;
class TanksContainer;

//typedef sf::Vector2i Vei2;

class Rocket
{
private:
	Vec2 pos;
	Vec2 velocity;
	bool exists;
	int originteam;
	int kind;
public:
	Rocket(const Vec2& inpos, const Vec2& invelocity, int inteam, int inkind = 1) : pos(inpos), velocity(invelocity), exists(true), originteam(inteam), kind(inkind), animation(0) {};
	void Fly();
	void Draw(sf::RenderWindow& wnd);
	bool Exists() const { return exists; }
	float GetX() const { return pos.x; }
	float GetY() const { return pos.y; }
	int GetOriginTeam() const { return originteam; }
	int GetKind() { return kind; }
	Vec2 GetPos() const { return pos; }
	Vec2 GetVelocity() const { return velocity; }
	void Liquidate() { exists = false; }
	static sf::Texture rocketsbitmap;
	int animation;
};


class RocketsContainer
{
public:
	~RocketsContainer();
	void AddRocket(Rocket* procket);
	void ProcessRockets(MapObjectsContainer& mapobjects, TanksContainer& tanks, VisualEffectsContainer& effects);
	void Draw(sf::RenderWindow& wnd) const;
	void MakeEmpty();
private:
	std::list<Rocket*> rockets;
};
