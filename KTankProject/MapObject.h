#pragma once

#include <SFML\Graphics.hpp>
#include <list>
#include "VisualEffect.h"
#include "MapInterpretation.h"
#include "Drawing.h"

//using namespace sf::Glsl;

class MapInterpretation;

class Block
{
public:
	Block() : pos(0, 0), exists(false) {};
	Block(const Vec2& inpos) : pos(inpos), exists(true) {};
	bool IsBlocking(const Vec2& inpos) const;;
	void Draw(sf::RenderWindow& wnd) const;
	void Destroy(VisualEffectsContainer& effects);
	Vec2 GetPos() const { return pos; };
private:
	bool exists;
	Vec2 pos;
};

class Brick
{
public:
	Brick() : pos(0,0), exists(false) {};
	Brick(const Vec2& inpos);;
	bool IsBlocking(const Vec2& inpos) const;
	void Destroy(VisualEffectsContainer& effects);
	void Draw(sf::RenderWindow& wnd, int i) const;
	Vec2 GetPos() const { return pos; };
private:
	bool exists;
	Vec2 pos;
};

class MapObject
{
public:
	MapObject(const Vec2& inpos): pos(inpos) {};
	virtual ~MapObject() {};
	virtual bool IsBlocking(const Vec2& inpos) const = 0;
	virtual bool IsBlockingShot(const Vec2& inpos) const = 0;
	virtual void ProcessExplosion(const Vec2& inpos, int kind, VisualEffectsContainer & effects) = 0;
	virtual void Draw(sf::RenderWindow& wnd) const = 0;
	virtual char ObjectType() const = 0;
	float GetX() const { return pos.x; };
	float GetY() const { return pos.y; };
	Vec2 GetPos() const { return pos; };
private:
	Vec2 pos;
};



class Fortress : public MapObject
{
public:
	// Constructor first invokes constructor of the base (virtual) class
	Fortress(const Vec2& inpos) : MapObject(inpos) {};
	bool IsBlocking(const Vec2& inpos) const;
	bool IsBlockingShot(const Vec2& inpos) const;
	void ProcessExplosion(const Vec2& inpos, int kind, VisualEffectsContainer& effects) {};
	void Draw(sf::RenderWindow& wnd) const;
	char ObjectType() const { return 0; };
};




class Blocks : public MapObject
{
public:
	// Constructor first invokes constructor of the base (virtual) class
	Blocks(const Vec2& inpos);
	bool IsBlocking(const Vec2& inpos) const;
	bool IsBlockingShot(const Vec2& inpos) const;
	void ProcessExplosion(const Vec2& inpos, int kind, VisualEffectsContainer& effects);
	void Draw(sf::RenderWindow& wnd) const;
	char ObjectType() const { return 1; };
private:
	Block block[4];
};


class Bricks : public MapObject
{
public:
	// Constructor first invokes constructor of the base (virtual) class
	Bricks(const Vec2& inpos);;
	bool IsBlocking(const Vec2& inpos) const;
	bool IsBlockingShot(const Vec2& inpos) const;
	void ProcessExplosion(const Vec2& inpos, int kind, VisualEffectsContainer& effects);
	void Draw(sf::RenderWindow& wnd) const;
	char ObjectType() const { return 2; };
private:
	Brick brick[16];
};

class WatersAnimation
{
public:
	static int loop;
};

class Water : public MapObject
{
public:
	// Constructor first invokes constructor of the base (virtual) class
	Water(const Vec2& inpos);
	bool IsBlocking(const Vec2& inpos) const;
	bool IsBlockingShot(const Vec2& inpos) const;
	void ProcessExplosion(const Vec2& inpos, int kind, VisualEffectsContainer& effects);
	void Draw(sf::RenderWindow& wnd) const;
	char ObjectType() const { return 3; };
	static sf::Texture waterbitmap;
};


class Eagle : public MapObject
{
public:
	// Constructor first invokes constructor of the base (virtual) class
	Eagle(const Vec2& inpos);
	bool IsBlocking(const Vec2& inpos) const;
	bool IsBlockingShot(const Vec2& inpos) const;
	void ProcessExplosion(const Vec2& inpos, int kind, VisualEffectsContainer& effects);
	void Draw(sf::RenderWindow& wnd) const;
	char ObjectType() const { return 4; };
	bool exists;
};


class Ice : public MapObject
{
public:
	// Constructor first invokes constructor of the base (virtual) class
	Ice(const Vec2& inpos);
	bool IsBlocking(const Vec2& inpos) const { return false; };
	bool IsBlockingShot(const Vec2& inpos) const { return false; };
	void ProcessExplosion(const Vec2& inpos, int kind, VisualEffectsContainer& effects) {};
	void Draw(sf::RenderWindow& wnd) const;
	char ObjectType() const { return 5; };
};


class Forest : public MapObject
{
public:
	// Constructor first invokes constructor of the base (virtual) class
	Forest(const Vec2& inpos);
	bool IsBlocking(const Vec2& inpos) const { return false; };
	bool IsBlockingShot(const Vec2& inpos) const { return false; };
	void ProcessExplosion(const Vec2& inpos, int kind, VisualEffectsContainer& effects) {};
	void Draw(sf::RenderWindow& wnd) const;
	char ObjectType() const { return 6; };
};



class MapObjectsContainer
{
public:
	~MapObjectsContainer();
	int WhatIsThere(const Vec2& inpos) const;
	MapObject* WhatIsBlocking(const Vec2& inpos);
	MapObject* WhatIsBlockingShot(const Vec2& inpos);
	bool IsPassageFree(const Vec2& inpos) const;
	void ProcessExplosion(const Vec2& inpos, const Vec2& invelocity, int kind, VisualEffectsContainer & effects);
	void AddMapObject(MapObject* mapobject) { mapobjects.emplace_back(mapobject); };
	void Draw(sf::RenderWindow& wnd) const;
	void DrawForests(sf::RenderWindow& wnd) const;
	void InterpretMap(MapInterpretation & mapinterpretation) const;
	void MakeEmpty();
	void DestroyObjectsInArea(const RectF & area);
	static sf::Texture mapobjectsbitmap;
private:
	std::list<MapObject*> mapobjects;
};


class Bonus
{
public:
	Bonus(const Vec2 & inpos, int inkind, int inremainingtime) : pos(inpos), kind(inkind), remainingtime(inremainingtime), exists(true) {}
	int GetKind() const { return kind; }
	void Draw(sf::RenderWindow& wnd) const;
	void Eliminate() { exists = false; };
	bool Exists() { return exists; }
	void Process() { if (remainingtime > 0) { remainingtime--; }; if (remainingtime <= 0) { Eliminate(); } };
	Vec2 GetPos() const { return pos; }
private:
	int kind;
	Vec2 pos;
	int remainingtime;
	bool exists;
};

class BonusesContainer
{
public:
	void MakeEmpty();
	~BonusesContainer() {MakeEmpty(); };
	void Draw(sf::RenderWindow& wnd) const;
	void Process();
	Bonus* GetTouchedBonus(const Vec2 & pos);
	static sf::Texture bonusesbitmap;
	void CreateBonus(Bonus * bonus) { bonuses.emplace_back(bonus); };
private:
	std::list<Bonus*> bonuses;
};


