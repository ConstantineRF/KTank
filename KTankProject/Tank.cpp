#include "Tank.h"
//#include <cassert>

sf::Texture TanksContainer::basictankbitmap;
sf::Texture TanksContainer::bluetankbitmap;
sf::Texture TanksContainer::redtankbitmap;
sf::Texture TanksContainer::greentankbitmap;

Tank::Tank(const Vec2& inpos, int inkind, int inor, float inV, int inteam) : pos(inpos), kind(inkind), orientation(inor), V(inV), team(inteam), velocity(0,0), dpush(0), gas(inV), 
rechargetimer(0), freezetimer(100), shieldtimer(100), revolvertimer(0), islive(true)
{
	if (kind==0) { life = 1; rechargeduration = 60; }
	else if (kind == 1) { life = 1; rechargeduration = 30; }
	else if (kind == 2) { life = 2; rechargeduration = 40; }
	else if (kind == 3) { life = 3; rechargeduration = 60; }
	else { life = 1; rechargeduration = 30; }
	if (inteam == 1) { ai = nullptr; }
	else { ai = new MachineBrain(this); }
}

Tank::Tank(const Vec2& inpos, int inkind, int inor, int inteam) : pos(inpos), kind(inkind), orientation(inor), team(inteam), velocity(0, 0), dpush(0),
rechargetimer(0), freezetimer(100), shieldtimer(100), revolvertimer(0), islive(true)
{
	if (kind==0) { V = 1.5f; life = 1; rechargeduration = 60; }
	else if (kind == 1) { V = 2.5f; life = 1; rechargeduration = 30; }
	else if (kind == 2) { V = 2.0f; life = 2; rechargeduration = 40; }
	else if (kind == 3) { V = 1.5f; life = 3; rechargeduration = 60; }
	else { V = 2.5f; life = 1; rechargeduration = 30; }
	gas = V;
	if (inteam == 1) { ai = nullptr; }
	else { ai = new MachineBrain(this); }

}

void Tank::SwitchOrientation(int inor) { orientation = inor; }
void Tank::SwitchOrientation() { orientation = dpush; }

int Tank::GetOrientation() const
{
	return orientation;
}

void Tank::Stop() { dpush = 0; }

void Tank::Push(int ind)
{
	dpush = ind;
}


void Tank::Progress()
{
	// This funciton cannot initiate a movement: speed vx, vy must have been established by Move(), prior to which blocking is checked
	if (!IsStill())
	{
		// Progress happens regardless of the instruction to push in any direction: it progresses to the nearest grid level , but no more than by remainder of the gas
		// It is assumed that this interval is free to move to
		float xcur = pos.x;
		float ycur = pos.y;
		float xnew = pos.x;
		float ynew = pos.y;
		if (GetOrientation() == 1)
		{
			xnew = std::min(xcur + gas, std::max(floor((xcur + 9.999f) / 10.0f)*10.0f, xcur));
		}
		if (GetOrientation() == 3)
		{
			xnew = std::max(xcur - gas, std::min(ceil((xcur - 9.999f) / 10.0f) * 10.0f, xcur));
		}
		if (GetOrientation() == 4)
		{
			ynew = std::min(ycur + gas, std::max(floor((ycur + 9.999f) / 10.0f) * 10.0f, ycur));
		}
		if (GetOrientation() == 2)
		{
			ynew = std::max(ycur - gas, std::min(ceil((ycur - 9.999f) / 10.0f) * 10.0f, ycur));
		}
		gas = gas - abs(xnew - xcur) - abs(ynew - ycur);
		pos = Vec2(xnew, ynew);
	}
}


void Tank::Move(const MapObjectsContainer& mapobjects, const TanksContainer* tanksptr)
{
	if (shieldtimer > 0) { shieldtimer--; }
	if (revolvertimer > 0) { revolvertimer--; }
	if (freezetimer > 0) { freezetimer--; }
	else
	{
		int objecttypeunder = mapobjects.WhatIsThere(GetPos());
		if ((dpush > 0) && (dpush != GetOrientation()) && (gas > 0.001f))
		{
			// When turning on the move, don't start moving in new direction right away
			SwitchOrientation(dpush);
		}
		else if ((((dpush > 0) && (dpush == GetOrientation())) || ((dpush==0) && (objecttypeunder==5)) ) && (gas > 0.001f))
		{
			// This basically means to continue moving in the same direction
			bool passagefree = false;
			if (GetOrientation() == 1)
			{
				passagefree = (
					mapobjects.IsPassageFree(GetPos() + Vec2(25.f, -15.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(25.f, -5.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(25.f, 5.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(25.f, 15.f)) &&
					tanksptr->NoTankHere(GetPos() + Vec2(25.f, -15.f)) && tanksptr->NoTankHere(GetPos() + Vec2(25.f, -5.f)) && tanksptr->NoTankHere(GetPos() + Vec2(25.f, 5.f)) && tanksptr->NoTankHere(GetPos() + Vec2(25.f, 15.f))
					);
			}
			if (GetOrientation() == 3)
			{
				passagefree = (
					mapobjects.IsPassageFree(GetPos() + Vec2(-25.f, -15.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(-25.f, -5.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(-25.f, 5.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(-25.f, 15.f)) &&
					tanksptr->NoTankHere(GetPos() + Vec2(-25.f, -15.f)) && tanksptr->NoTankHere(GetPos() + Vec2(-25.f, -5.f)) && tanksptr->NoTankHere(GetPos() + Vec2(-25.f, 5.f)) && tanksptr->NoTankHere(GetPos() + Vec2(-25.f, 15.f))
					);
			}
			if (GetOrientation() == 2)
			{
				passagefree = (
					mapobjects.IsPassageFree(GetPos() + Vec2(-15.f, -25.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(-5.f, -25.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(5.f, -25.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(15.f, -25.f)) &&
					tanksptr->NoTankHere(GetPos() + Vec2(-15.f, -25.f)) && tanksptr->NoTankHere(GetPos() + Vec2(-5.f, -25.f)) && tanksptr->NoTankHere(GetPos() + Vec2(5.f, -25.f)) && tanksptr->NoTankHere(GetPos() + Vec2(15.f, -25.f))
					);
			}
			if (GetOrientation() == 4)
			{
				passagefree = (
					mapobjects.IsPassageFree(GetPos() + Vec2(-15.f, 25.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(-5.f, 25.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(5.f, 25.f)) && mapobjects.IsPassageFree(GetPos() + Vec2(15.f, 25.f)) &&
					tanksptr->NoTankHere(GetPos() + Vec2(-15.f, 25.f)) && tanksptr->NoTankHere(GetPos() + Vec2(-5.f, 25.f)) && tanksptr->NoTankHere(GetPos() + Vec2(5.f, 25.f)) && tanksptr->NoTankHere(GetPos() + Vec2(15.f, 25.f))
					);
			}
			if (passagefree)
			{
				velocity = (dir2vec(GetOrientation()) * V);
				pos += (dir2vec(GetOrientation()) * gas);
				//velocity = (dpush==0) ? (dir2vec(GetOrientation()) * V * 0.5) : (dir2vec(GetOrientation()) * V);
				//pos += (dpush == 0) ? (dir2vec(GetOrientation()) * gas * 0.5) : (dir2vec(GetOrientation()) * gas);
				gas = 0.f;
			}
			else if (ai != nullptr) { ai->HitWall(GetOrientation()); }
		}
		if (rechargetimer > 0) { rechargetimer--; }
	}
}

void Tank::TryShooting(RocketsContainer & rockets)
{
	if ((rechargetimer == 0) && (freezetimer == 0))
	{
		rechargetimer = rechargeduration;
		int kind = 1;
		if ((team > 0) & (RevolverTimer() > 0)) { kind = 2; }
		rockets.AddRocket(new Rocket(GetPos() + Tank::dir2vec(GetOrientation())*(21.f), Tank::dir2vec(GetOrientation())*(5.f), team, kind));
	}
}

void Tank::AbsorbHit(int kind)
{
	if (kind == 2) { life-=3; }	else { life--; };
	if (life <= 0) { islive = false; }
}

void Tank::Annihilate()
{
	islive = false;
}

const Vec2 Tank::dir2vec(int d)
{
	if (d == 1) { return Vec2::UnitX(); }
	else if (d == 2) { return Vec2::UnitY()*(-1.0f); }
	else if (d == 3) { return Vec2::UnitX()*(-1.0f); }
	else if (d == 4) { return Vec2::UnitY(); }
	else return Vec2(0, 0);
}





void Tank::Draw(sf::RenderWindow& wnd) const
{
	RectI frame(Vei2(0,0), Vei2(38, 38));
	if (kind == 0) DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), TanksContainer::basictankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), GetOrientation());
	if (kind == 1) DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), GetOrientation());
	if (kind == 2) DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), TanksContainer::redtankbitmap, RectI(Vei2(0, 0), Vei2(38, 38)), GetOrientation());
	if (kind == 3) DrawSpriteChromaCenteredRotated(wnd, Vei2(pos), TanksContainer::greentankbitmap, RectI(Vei2(0, 0), Vei2(49, 39)), GetOrientation());
	/*
	if (kind == 1) 
	{
		sf::Sprite sprite;
		sprite.setTexture(TanksContainer::bluetankbitmap);
		sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
		if (GetOrientation() == 2) {
			sprite.setRotation(270);
		}
		else if (GetOrientation() == 3) {
			sprite.setRotation(180);
		}
		else if (GetOrientation() == 4) {
			sprite.setRotation(90);
		}
		sprite.setPosition((float)pos.x, (float)pos.y);
		sprite.setScale(sf::Vector2f(0.25, 0.25));
		wnd.draw(sprite);
	}
	*/
}

TanksContainer::~TanksContainer()
{
	for (std::list<Tank*>::iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		delete (*it);
	}
	tanks.clear();
}

void TanksContainer::MakeEmpty()
{
	for (std::list<Tank*>::iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		delete (*it);
	}
	tanks.clear();
}

void TanksContainer::BlowUpTeam(int team, VisualEffectsContainer & effects, RectF area)
{
	for (std::list<Tank*>::iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if (((*it)->GetTeam() == team) && ((*it)->IsLive()) && (!((*it)->IsShieldOn())) && (area.Contains((*it)->GetPos())))
		{
			(*it)->Annihilate();
			effects.AddVisualEffect((*it)->GetPos(), TANK_EXPLOSION);
		}
	}
}

void TanksContainer::CreateTank(const Vec2 & inpos, int inor, float inV, int inteam)
{
	tanks.emplace_back(new Tank(inpos, inor, inV, inteam));
}

int TanksContainer::GetTanksNumber() const
{
	int cnt = 0;
	for (std::list<Tank*>::const_iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if ((*it)->IsLive()) { cnt++; };
	}

	return cnt;
}

int TanksContainer::GetTeamTanksNumber(int inteam) const
{
	int cnt = 0;
	for (std::list<Tank*>::const_iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if ((*it)->IsLive() && (*it)->GetTeam()==inteam) { cnt++; };
	}

	return cnt;
}

Tank * TanksContainer::GetTankAtLocation(const Vec2 & inpos)
{
	for (std::list<Tank*>::const_iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if ((*it)->IsLive() && ( ((*it)->GetPos() - inpos).LInfNorm()<=20 )) { return *it; };
	}
	return nullptr;
}

bool TanksContainer::NoTankHere(const Vec2 & inpos) const
{
	for (std::list<Tank*>::const_iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if ((*it)->IsLive() && (((*it)->GetPos() - inpos).LInfNorm() <= 20)) { return false; };
	}
	return true;
}

void TanksContainer::Refill()
{
	for (std::list<Tank*>::const_iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if ((*it)->IsLive()) { (*it)->Refill(); };
	}
}

void TanksContainer::Progress()
{
	for (std::list<Tank*>::const_iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if ((*it)->IsLive()) { (*it)->Progress(); };
	}
}

void TanksContainer::ProcessUserControls(sf::RenderWindow& wnd, RocketsContainer& rockets, const MapInterpretation & mapinterpretation)
{
	for (std::list<Tank*>::const_iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if (((*it)->IsLive()) && ((*it)->GetTeam()==1)) 
		{ 
			//(*it)->Progress(); 
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				(*it)->Push(1);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				(*it)->Push(3);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				(*it)->Push(2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				(*it)->Push(4);
			}
			else
			{
				(*it)->Stop();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			{
				(*it)->TryShooting(rockets);
			}
		};
		if (((*it)->IsLive()) && ((*it)->GetTeam() == -1))
		{
			(*it)->Push((*it)->ai->GetPushDirection(mapinterpretation));
			(*it)->TryShooting(rockets);
		}
	}

}

void TanksContainer::Move(const MapObjectsContainer& mapobjects)
{
	for (std::list<Tank*>::const_iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if ((*it)->IsLive()) { (*it)->Move(mapobjects, this); };
	}
}

void TanksContainer::Draw(sf::RenderWindow& wnd) const
{
	for (std::list<Tank*>::const_iterator it = tanks.begin(); it != tanks.end(); it++)
	{
		if ((*it)->IsLive()) { (*it)->Draw(wnd); };
	}
}

MachineBrain::MachineBrain(Tank * intank) : tank(intank), patience(60), preferreddirection(4), followingalternativedirectioncounter(0) {}

Vei2 dir2vei(int d)
{
	if (d == 1) { return Vei2(1, 0); }
	if (d == 2) { return Vei2(0, -1); }
	if (d == 3) { return Vei2(-1, 0); }
	if (d == 4) { return Vei2(0, 1); }
	return Vei2(0, 0);
}

int MachineBrain::GetPushDirection(const MapInterpretation & mapinterpretation)
{
	if (tank->GetKind() <= 2)
	{
		// Fast tanks are assumed to consistently explore, preferring going straight, and changing direction
		// when either can't get through an obstacle (patience==0), or when there is a relatively smooth alternative
		if (patience == 0)
		{
			// When hit an obstacle and stuck
			patience = 60;
			preferreddirection = ((rand() % 3 + 1) + (preferreddirection - 1)) % 4 + 1;
		}
		else if (tank->GetGasLeft()>0)
		{
			// infer probability and direction of more optimal route
			Vei2 iloc((tank->GetPos() - Vec2(20.0f, 20.0f)) * 0.1);
			bool wholex = (iloc.x % 4) == 0;
			bool wholey = (iloc.y % 4) == 0;
			int bestdirection = 0; // (iloc.y < 56) ? 4 : 2;
			int secondbestdirection = 0; // (iloc.y < 56) ? 4 : 2;
			int currentcost = mapinterpretation.route[iloc.x][iloc.y];
			int bestcost = 999999999;  //(iloc.y<56) ? mapinterpretation.route[iloc.x][iloc.y + 1] : mapinterpretation.route[iloc.x][iloc.y - 1];
			int secondbestcost = 999999999; // (iloc.y < 56) ? mapinterpretation.route[iloc.x][iloc.y + 1] : mapinterpretation.route[iloc.x][iloc.y - 1];
			if (iloc.x<56) if (mapinterpretation.route[iloc.x + 1][iloc.y] < bestcost) { bestdirection = 1; bestcost = mapinterpretation.route[iloc.x + 1][iloc.y]; }
			if (iloc.y>0) if (mapinterpretation.route[iloc.x][iloc.y-1] < bestcost) { bestdirection = 2; bestcost = mapinterpretation.route[iloc.x][iloc.y - 1]; }
			if (iloc.x > 0) if (mapinterpretation.route[iloc.x - 1][iloc.y] < bestcost) { bestdirection = 3; bestcost = mapinterpretation.route[iloc.x - 1][iloc.y]; }
			if (iloc.y < 56) if (mapinterpretation.route[iloc.x][iloc.y + 1] < bestcost) { bestdirection = 4; bestcost = mapinterpretation.route[iloc.x][iloc.y + 1]; }
			if (iloc.x < 56) if ((mapinterpretation.route[iloc.x + 1][iloc.y] < secondbestcost) && ((mapinterpretation.route[iloc.x + 1][iloc.y] > bestcost) || ((mapinterpretation.route[iloc.x + 1][iloc.y] == bestcost)&&(bestdirection!=1)))) { secondbestdirection = 1; secondbestcost = mapinterpretation.route[iloc.x + 1][iloc.y]; }
			if (iloc.y > 0) if ((mapinterpretation.route[iloc.x][iloc.y - 1] < secondbestcost) && ((mapinterpretation.route[iloc.x][iloc.y - 1] > bestcost) || ((mapinterpretation.route[iloc.x][iloc.y - 1] == bestcost) && (bestdirection != 2)))) { secondbestdirection = 2; secondbestcost = mapinterpretation.route[iloc.x][iloc.y - 1]; }
			if (iloc.x > 0) if ((mapinterpretation.route[iloc.x - 1][iloc.y] < secondbestcost) && ((mapinterpretation.route[iloc.x - 1][iloc.y] > bestcost) || ((mapinterpretation.route[iloc.x - 1][iloc.y] == bestcost) && (bestdirection != 3)))) { secondbestdirection = 3; secondbestcost = mapinterpretation.route[iloc.x - 1][iloc.y]; }
			if (iloc.y < 56) if ((mapinterpretation.route[iloc.x][iloc.y + 1] < secondbestcost) && ((mapinterpretation.route[iloc.x][iloc.y + 1] > bestcost) || ((mapinterpretation.route[iloc.x][iloc.y + 1] == bestcost) && (bestdirection != 4)))) { secondbestdirection = 4; secondbestcost = mapinterpretation.route[iloc.x][iloc.y + 1]; }
			int orthogonal = ((preferreddirection - 1) + (rand()%2 + 1) * 3) % 4 + 1;
			//assert(bestdirection > 0);
			//assert(bestdirection < 5);
			//assert(secondbestdirection > 0);
			//assert(secondbestdirection < 5);
			//assert(orthogonal > 0);
			//assert(orthogonal < 5);
			// Must decide whether to change direction, and to where. Its probability depends on position of tank, the difference of costs, and whether direction should be changed.
			int probchangetobest = 0;
			int probchangetosecondbest = 0;
			int probchangetoorthogonal = 0;
			//if ((abs(preferreddirection - bestdirection) % 2 == 1)) { probchangetobest = (iloc.x % 4 + iloc.y % 4 == 0) ? 25 : 5; probchangetosecondbest = 0; }
			//if ((abs(preferreddirection - bestdirection) % 2 == 0)) { probchangetobest = 1; probchangetosecondbest = (iloc.x % 4 + iloc.y % 4 == 0) ? 4 : 1; }
			if ((wholex) && (wholey))
			{
				if ((preferreddirection == bestdirection) && (abs(preferreddirection - secondbestdirection) % 2 == 1)) { probchangetobest = 0; probchangetosecondbest = 20; }
				else if ((preferreddirection == bestdirection) && (abs(preferreddirection - secondbestdirection) % 2 == 0)) { probchangetobest = 0; probchangetosecondbest = 2; probchangetoorthogonal = 2;}
				else if ((abs(preferreddirection - bestdirection) % 2 == 1) && (preferreddirection == secondbestdirection)) { probchangetobest = 40; probchangetosecondbest = 0; probchangetoorthogonal = 2; }
				else if ((abs(preferreddirection - bestdirection) % 2 == 1) && (abs(preferreddirection - secondbestdirection) % 2 == 0)) { probchangetobest = 40; probchangetosecondbest = 2; }
				else if ((abs(preferreddirection - bestdirection) % 2 == 1) && (abs(preferreddirection - secondbestdirection) % 2 == 1)) { probchangetobest = 40; probchangetosecondbest = 5; }
				else if ((abs(preferreddirection - bestdirection) % 2 == 0) && (preferreddirection == secondbestdirection)) { probchangetobest = 5; probchangetosecondbest = 0; probchangetoorthogonal = 2; }
				else if ((abs(preferreddirection - bestdirection) % 2 == 0) && (abs(preferreddirection - secondbestdirection) % 2 == 1)) { probchangetobest = 10; probchangetosecondbest = 15; }
			}
			else
			{
				if ((preferreddirection == bestdirection) && (abs(preferreddirection - secondbestdirection) % 2 == 1)) { probchangetobest = 0; probchangetosecondbest = 1; }
				else if ((preferreddirection == bestdirection) && (abs(preferreddirection - secondbestdirection) % 2 == 0)) { probchangetobest = 0; probchangetosecondbest = 1; }
				else if ((abs(preferreddirection - bestdirection) % 2 == 1) && (preferreddirection == secondbestdirection)) { probchangetobest = 2; probchangetosecondbest = 0; }
				else if ((abs(preferreddirection - bestdirection) % 2 == 1) && (abs(preferreddirection - secondbestdirection) % 2 == 0)) { probchangetobest = 2; probchangetosecondbest = 1; }
				else if ((abs(preferreddirection - bestdirection) % 2 == 1) && (abs(preferreddirection - secondbestdirection) % 2 == 1)) { probchangetobest = 7; probchangetosecondbest = 3; }
				else if (((preferreddirection - bestdirection) % 2 == 0) && (preferreddirection == secondbestdirection)) { probchangetobest = 2; probchangetosecondbest = 0; }
				else if (((preferreddirection - bestdirection) % 2 == 0) && (abs(preferreddirection - secondbestdirection) % 2 == 1)) { probchangetobest = 3; probchangetosecondbest = 1; }
			}
			/*
			if ((abs(preferreddirection - bestdirection) % 2 == 1)) { probchangetobest = ((iloc.x % 4)*(bestdirection%2) + (iloc.y % 4)*((bestdirection-1) % 2) == 0) ? 20 : 5; probchangetosecondbest = 1; }
			if ((abs(preferreddirection - bestdirection) % 2 == 0)) { probchangetobest = 1; probchangetosecondbest = ((iloc.x % 4)*(secondbestdirection % 2) + (iloc.y % 4)*((secondbestdirection - 1) % 2) == 0) ? 4 : 1; }
			*/
			int p = rand() % 100;
			if (p < probchangetobest) { preferreddirection = bestdirection; }
			if (p < probchangetobest + probchangetosecondbest) { preferreddirection = secondbestdirection; }
			if (p < probchangetobest + probchangetosecondbest + probchangetoorthogonal) { preferreddirection = orthogonal; }
		}
		return preferreddirection;
	}
	if (tank->GetKind() >= 3)
	{
		// Slow tanks are assumed to go straight to final destination
		// At each step, tells Game where to push AI tank
		if (patience == 0)
		{
			patience = 60;
			preferreddirection = ((rand() % 3 + 1) + (preferreddirection - 1)) % 4 + 1;
			followingalternativedirectioncounter = 10;
			//followingalternativedirectioncounter--; 
			//return preferreddirection; 
		}
		if (followingalternativedirectioncounter > 0)
		{
			followingalternativedirectioncounter--;
			return preferreddirection;
		}
		Vei2 iloc((tank->GetPos() - Vec2(20.0f, 20.0f)) * 0.1);
		int bestdirection = 4;
		int bestvalue = 1000000;
		int choosebestprobability = 0;
		if (iloc.y < mapinterpretation.r - 4) { bestvalue = mapinterpretation.route[iloc.x][iloc.y + 1]; }
		if (iloc.y > 0)
		{
			if (mapinterpretation.route[iloc.x][iloc.y - 1] < bestvalue)
			{
				bestvalue = mapinterpretation.route[iloc.x][iloc.y - 1];
				bestdirection = 2;
			}
		}
		if (iloc.x > 0)
		{
			if (mapinterpretation.route[iloc.x - 1][iloc.y] < bestvalue)
			{
				bestvalue = mapinterpretation.route[iloc.x - 1][iloc.y];
				bestdirection = 3;
			}
		}
		if (iloc.x < mapinterpretation.c - 4)
		{
			if (mapinterpretation.route[iloc.x + 1][iloc.y] < bestvalue)
			{
				bestvalue = mapinterpretation.route[iloc.x + 1][iloc.y];
				bestdirection = 1;
			}
		}
		if (bestvalue == 1000000) { return preferreddirection; }
		if ((iloc.x % 4 == 0) && (iloc.y % 4 == 0)) { choosebestprobability = 90; }
		else if ((iloc.x % 4 == 0) && (iloc.y % 2 == 0)) { choosebestprobability = 95; }
		else if ((iloc.x % 2 == 0) && (iloc.y % 4 == 0)) { choosebestprobability = 95; }
		else if ((iloc.x % 2 == 0) && (iloc.y % 2 == 0)) { choosebestprobability = 99; }
		else { choosebestprobability = 100; }

		int outdirection = (rand() % 100 < choosebestprobability) ? bestdirection : preferreddirection;
		return outdirection;
	}

	//return bestdirection;
}

void MachineBrain::HitWall(int inpush)
{
	if (patience > 0) { patience--; }
	/*
	else
	{
		preferreddirection = ((rand() % 3 + 1) + (preferreddirection - 1)) % 4 + 1;
		followingalternativedirectioncounter = 150;
		patience = 60;
	}
	*/
}
