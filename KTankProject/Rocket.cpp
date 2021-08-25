#include "Rocket.h"
sf::Texture Rocket::rocketsbitmap;

void Rocket::Fly()
{
	if ((exists) && ((pos + velocity).IsWithinRectangle(Vec2(0.f, 0.f), Vec2(599.999f, 599.999f))))
	{
		pos = pos + velocity;
	}
	else exists = false;
}

void Rocket::Draw(sf::RenderWindow& wnd)
{
	int dir;
	if (velocity.x > 0.1f) { dir = 1; }
	if (velocity.y < -0.1f) { dir = 2; }
	if (velocity.x < -0.1f) { dir = 3; }
	if (velocity.y > 0.1f) { dir = 4; }
	if (Exists())
	{
		if (kind == 1)
		{
			if (animation == 0) { DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), Rocket::rocketsbitmap, RectI(Vei2(0, 0), Vei2(19, 19)), dir); }
			if (animation == 1) { DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), Rocket::rocketsbitmap, RectI(Vei2(20, 0), Vei2(39, 19)), dir); }
			if (animation == 2) { DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), Rocket::rocketsbitmap, RectI(Vei2(0, 20), Vei2(19, 39)), dir); }
		}
		else if (kind == 2)
		{
			if (animation == 0) { DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), Rocket::rocketsbitmap, RectI(Vei2(20, 20), Vei2(39, 39)), dir); }
			if (animation == 1) { DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), Rocket::rocketsbitmap, RectI(Vei2(0, 40), Vei2(19, 59)), dir); }
			if (animation == 2) { DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), Rocket::rocketsbitmap, RectI(Vei2(20, 40), Vei2(39, 59)), dir); }
		}
	}
	animation = (animation + 1) % 3;
}

RocketsContainer::~RocketsContainer()
{
	MakeEmpty();
}

void RocketsContainer::MakeEmpty()
{
	for (std::list<Rocket*>::iterator it = rockets.begin(); it != rockets.end(); it++)
	{
		delete (*it);
	}
	rockets.clear();
}

void RocketsContainer::AddRocket(Rocket * procket) { rockets.emplace_back(procket); }

void RocketsContainer::ProcessRockets(MapObjectsContainer & mapobjects, TanksContainer & tanks, VisualEffectsContainer& effects)
{
	std::list<Rocket*>::iterator it = rockets.begin();
	while (it != rockets.end())
	{
		(*it)->Fly();
		bool itstillflying = (*it)->Exists();
		// First, check if it hit another rocket
		if (itstillflying)
		{
			for (std::list<Rocket*>::iterator another = rockets.begin(); another != rockets.end(); another++)
			{
				if (((*it) != (*another)) && (((*it)->GetPos() - (*another)->GetPos()).LengthSq() < 36))
				{
					effects.AddVisualEffect((*it)->GetPos(), ROCKET_EXPLOSION);
					effects.AddVisualEffect((*another)->GetPos(), ROCKET_EXPLOSION);
					(*it)->Liquidate();
					(*another)->Liquidate();
					itstillflying = false;
				}
			}
		}
		// Second, check if hit a wall			
		if (itstillflying)
		{
			MapObject* blockingobject = NULL;
			blockingobject = mapobjects.WhatIsBlockingShot((*it)->GetPos());
			if (blockingobject != NULL) {
				mapobjects.ProcessExplosion((*it)->GetPos(), (*it)->GetVelocity(), (*it)->GetKind(), effects);
				effects.AddVisualEffect((*it)->GetPos(), ROCKET_EXPLOSION);
				(*it)->Liquidate();
				itstillflying = false;
			}
		}
		// Finally, check if hit another tank
		if (itstillflying)
		{
			Tank* hittank = nullptr;
			hittank = tanks.GetTankAtLocation((*it)->GetPos());
			if (hittank != nullptr)
			{
				if ((!(hittank->IsShieldOn())) & (hittank->GetTeam()!=(*it)->GetOriginTeam()))
				{
					hittank->AbsorbHit((*it)->GetKind());
					if (!hittank->IsLive()) { effects.AddVisualEffect(hittank->GetPos(), TANK_EXPLOSION); }					
				}
				effects.AddVisualEffect((*it)->GetPos(), ROCKET_EXPLOSION);
				(*it)->Liquidate();
				itstillflying = false;
			}
		}
		// No worries if following rocket liquidates this rocket. The below step will be executed in next loop.
		if ((*it)->Exists() == false) {
			delete *it;
			it = rockets.erase(it);
		}
		else {
			it++;
		}

	}
}

void RocketsContainer::Draw(sf::RenderWindow& wnd) const
{
	for (std::list<Rocket*>::const_iterator it = rockets.begin(); it != rockets.end(); it++)
	{
		(*it)->Draw(wnd);
	}
}
