#pragma once

#include <list>
#include <SFML\Graphics.hpp>
#include "Vec2.h"
#include "Rect.h"
#include "Drawing.h"
#include "Sounds.h"

enum VisualEffectType {ROCKET_EXPLOSION, TANK_EXPLOSION, BRICKS_BREAKING, TANK_CREATION};

class VisualEffect
{
public:
	VisualEffect(const Vec2& inpos, VisualEffectType visualeffecttype) : pos(inpos), type(visualeffecttype)
	{
		switch (visualeffecttype)
		{
		case ROCKET_EXPLOSION: countdown = 25; break;
		case TANK_EXPLOSION: countdown = 50; break;
		case BRICKS_BREAKING: countdown = 20; break;
		case TANK_CREATION: countdown = 100; break;
		}
			
	};
	void Evolve() {
		countdown--;
	}
	int GetCountdown() const { return countdown; };
	float GetX() const { return pos.x; };
	float GetY() const { return pos.y; };
	Vec2 GetPos() const { return pos; };
	void Draw(sf::RenderWindow& wnd) const
	{
		switch (type)
		{
		case ROCKET_EXPLOSION: {
			int n = 25 - countdown;
			if (n > 24) { n = 24; }
			if (n < 0) { n = 0; }
			int i = n % 5; int j = (n - i) / 5;
			DrawSpriteChromaCentered(wnd, Vei2(pos), smallexplosionbitmap, RectI(Vei2(15 * i, 15 * j), Vei2(14 + 15 * i, 14 + 15 * j)));
			if (countdown==24)	Sounds::boom.play();
		} break;
		case TANK_EXPLOSION: {
			int n = (50 - countdown) / 2;
			if (n > 24) { n = 24; } 
			if (n < 0) { n = 0; }
			int i = n % 5; int j = (n - i) / 5;
			DrawSpriteChromaCentered(wnd, Vei2(pos), bigexplosionbitmap, RectI(Vei2(40*i, 40*j), Vei2(39+40*i, 39+40*j)));
			if (countdown==49) Sounds::bigexplosion.play();
		} break;
		case BRICKS_BREAKING: {
			sf::VertexArray va(sf::Points, 5 + countdown);
			for (int i=0; i<5+countdown; i++) { 
				va[i].position = sf::Vector2f(pos.x - 5 + (rand() % 10), pos.y - 5 + (rand() % 10));
				/*
				gfx.PutPixel(int(pos.x) + 15 + (rand() % 10), int(pos.y) + 15 + (rand() % 10), 100, 20, 20);
				*/
			}
			wnd.draw(va);
		} break;
		case TANK_CREATION: {
			sf::RectangleShape rct(sf::Vector2f(38.f, 38.f));
			rct.setPosition(pos.x - 19.f, pos.y - 19.f);
			rct.setOutlineColor(sf::Color::Color(255, 255, 255, 255 - 2 * countdown));
			rct.setOutlineThickness(2.f);
			/*
			for (int i = 0; i < 10*countdown; i++) { gfx.PutPixel(int(pos.x) + (rand() % 40), int(pos.y) + (rand() % 40), 200, 200, 200); }
			gfx.DrawLine(int(pos.x), int(pos.y), int(pos.x) + 40, int(pos.y), 255 - 2 * countdown, 255 - 2 * countdown, 255 - 2 * countdown);
			gfx.DrawLine(int(pos.x) + 40, int(pos.y), int(pos.x) + 40, int(pos.y) + 40, 255 - 2 * countdown, 255 - 2 * countdown, 255 - 2 * countdown);
			gfx.DrawLine(int(pos.x), int(pos.y) + 40, int(pos.x) + 40, int(pos.y) + 40, 255 - 2 * countdown, 255 - 2 * countdown, 255 - 2 * countdown);
			gfx.DrawLine(int(pos.x), int(pos.y), int(pos.x), int(pos.y) + 40, 255 - 2 * countdown, 255 - 2 * countdown, 255 - 2 * countdown);
			*/
			rct.setFillColor(sf::Color::Transparent);
			wnd.draw(rct);
		} break;
		}
		
	};
private:
	Vec2 pos;
	int countdown;
	VisualEffectType type;
public:
	static sf::Texture bigexplosionbitmap;
	static sf::Texture smallexplosionbitmap;
};

class VisualEffectsContainer
{
public:
	~VisualEffectsContainer() 
	{
		for (std::list<VisualEffect*>::iterator it = effects.begin(); it != effects.end(); it++)
		{
			delete (*it);
		}
		effects.clear();
	};
	void AddVisualEffect(const Vec2& inpos, VisualEffectType type)
	{
		effects.emplace_back(new VisualEffect(inpos, type));
	};
	void EvolveVisualEffects()
	{
		std::list<VisualEffect*>::iterator it = effects.begin();
		while (it != effects.end())
		{
			if ((*it)->GetCountdown() <= 0) { it = effects.erase(it); }
			else { (*it)->Evolve();  it++; }
		}
	};
	void Draw(sf::RenderWindow& wnd) const
	{
		for (std::list<VisualEffect*>::const_iterator it = effects.begin(); it != effects.end(); it++)
		{
			(*it)->Draw(wnd);
		}
	};

private:
	std::list<VisualEffect*> effects;

};





