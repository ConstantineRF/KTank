#include "MapObject.h"

sf::Texture MapObjectsContainer::mapobjectsbitmap;
sf::Texture Water::waterbitmap;
sf::Texture BonusesContainer::bonusesbitmap;

//using namespace sf::Glsl;

bool Block::IsBlocking(const Vec2 & inpos) const
{
	return ((inpos - pos).LInfNorm() < 10.001f) && (exists);
}

void Block::Draw(sf::RenderWindow& wnd) const
{
	if (exists)
	{
		DrawSpriteChromaCentered(wnd, Vei2(pos), MapObjectsContainer::mapobjectsbitmap, RectI(Vei2(0, 0), Vei2(19, 19)));
	}
}

inline void Block::Destroy(VisualEffectsContainer& effects) { exists = false; effects.AddVisualEffect(GetPos(), BRICKS_BREAKING); }

inline Brick::Brick(const Vec2 & inpos) : pos(inpos), exists(true) {}

inline bool Brick::IsBlocking(const Vec2 & inpos) const
{
	return ((inpos - pos).LInfNorm() < 5.001f) && (exists);
}

inline void Brick::Destroy(VisualEffectsContainer & effects) { exists = false; effects.AddVisualEffect(GetPos(), BRICKS_BREAKING); }

void Brick::Draw(sf::RenderWindow& wnd, int i) const
{
	if (exists)
	{
		int c = i % 4;
		int r = (i - c) / 4;
		DrawSpriteChromaCentered(wnd, Vei2(pos), MapObjectsContainer::mapobjectsbitmap, RectI(Vei2(40 + c * 10, r * 10), Vei2(49 + c * 10, 9 + r * 10)));
	}
}

// Constructor first invokes constructor of the base (virtual) class



Blocks::Blocks(const Vec2 & inpos) : MapObject(inpos)
{
	block[0] = Block(inpos + Vec2(-10.0f, -10.0f));
	block[1] = Block(inpos + Vec2(10.0f, -10.0f));
	block[2] = Block(inpos + Vec2(-10.0f, 10.0f));
	block[3] = Block(inpos + Vec2(10.0f, 10.0f));
}


bool Fortress::IsBlocking(const Vec2 & inpos) const
{
	return (GetPos() - inpos).LInfNorm() < 19.999f;
}

bool Fortress::IsBlockingShot(const Vec2 & inpos) const
{
	return (inpos - GetPos()).LInfNorm() < 20.001f;
}

void Fortress::Draw(sf::RenderWindow& wnd) const
{
	DrawSpriteChromaCentered(wnd, Vei2(GetPos()), MapObjectsContainer::mapobjectsbitmap, RectI(Vei2(0, 40), Vei2(39, 79)));
}


bool Blocks::IsBlocking(const Vec2 & inpos) const
{
	if ((GetPos() - inpos).LInfNorm() > 20.001f)
	{
		return false;
	}
	else
	{
		int i = 0;
		while (i < 4)
		{
			if (block[i].IsBlocking(inpos))
			{
				return true;
			}
			i++;
		}
		return false;
	}
}

bool Blocks::IsBlockingShot(const Vec2 & inpos) const
{
	if ((GetPos() - inpos).LInfNorm() > 20.001f)
	{
		return false;
	}
	else
	{
		int i = 0;
		while (i < 4)
		{
			if (block[i].IsBlocking(inpos))
			{
				return true;
			}
			i++;
		}
		return false;
	}
}

void Blocks::ProcessExplosion(const Vec2 & inpos, int kind, VisualEffectsContainer & effects)
{
	// LATER RETURN THE BELOW TO ADDRESS VARIOUS KINDS OF EXPLOSIONS
	if (kind == 2)
	{
		if ((GetPos() - inpos).LInfNorm() < 20.001f)
		{
			int i = 0;
			while (i < 4)
			{
				if (block[i].IsBlocking(inpos))
				{
					block[i].Destroy(effects);
				}
				i++;
			}
		}
	}
}

void Blocks::Draw(sf::RenderWindow& wnd) const
{
	for (int i = 0; i < 4; i++)
	{
		block[i].Draw(wnd);
	}
}

// Constructor first invokes constructor of the base (virtual) class

Bricks::Bricks(const Vec2 & inpos) : MapObject(inpos)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			brick[i + j * 4] = Brick(inpos + Vec2::UnitX()*(-15.0f + i * 10.0f) + Vec2::UnitY()*(-15.0f + j * 10.0f));
		}
	}
}

bool Bricks::IsBlocking(const Vec2 & inpos) const
{
	if ((GetPos() - inpos).LInfNorm() > 20.001f)
	{
		return false;
	}
	else
	{
		int i = 0;
		while (i < 16)
		{
			if (brick[i].IsBlocking(inpos))
			{
				return true;
			}
			i++;
		}
		return false;
	};
}

bool Bricks::IsBlockingShot(const Vec2 & inpos) const
{
	if ((GetPos() - inpos).LInfNorm() > 20.001f)
	{
		return false;
	}
	else
	{
		int i = 0;
		while (i < 16)
		{
			if (brick[i].IsBlocking(inpos))
			{
				return true;
			}
			i++;
		}
		return false;
	};
}

void Bricks::ProcessExplosion(const Vec2 & inpos, int kind, VisualEffectsContainer& effects)
{
	if ((GetPos() - inpos).LInfNorm() < 20.001f)
	{
		int i = 0;
		while (i < 16)
		{
			if (brick[i].IsBlocking(inpos))
			{
				brick[i].Destroy(effects);
			}
			i++;
		}
	}
}

void Bricks::Draw(sf::RenderWindow& wnd) const
{
	for (int i = 0; i < 16; i++)
	{
		brick[i].Draw(wnd, i);
	}
}


int WatersAnimation::loop = 0;

Water::Water(const Vec2 & inpos) : MapObject(inpos)
{
}

bool Water::IsBlocking(const Vec2 & inpos) const
{
	return ((GetPos() - inpos).LInfNorm() < 19.999f);
}

bool Water::IsBlockingShot(const Vec2 & inpos) const
{
	return false;
}

void Water::ProcessExplosion(const Vec2 & inpos, int kind, VisualEffectsContainer & effects)
{
}

void Water::Draw(sf::RenderWindow& wnd) const
{
	int i = (WatersAnimation::loop/3) % 4;
	int j = (WatersAnimation::loop/3 - i) / 4;
	DrawSpriteChromaCentered(wnd, Vei2(GetPos()), waterbitmap, RectI(Vei2(i*40, j*40), Vei2(i*40+39, j*40+39)));
}


Eagle::Eagle(const Vec2 & inpos) : MapObject(inpos), exists(true)
{
}

bool Eagle::IsBlocking(const Vec2 & inpos) const
{
	return ((GetPos() - inpos).LInfNorm() < 19.999f);
}

bool Eagle::IsBlockingShot(const Vec2 & inpos) const
{
	return ((GetPos() - inpos).LInfNorm() < 19.999f);
}

void Eagle::ProcessExplosion(const Vec2 & inpos, int kind, VisualEffectsContainer & effects)
{
	if ((GetPos() - inpos).LInfNorm() < 20.001f)
	{
		exists = false;
		effects.AddVisualEffect(GetPos(), TANK_EXPLOSION);
	}
}

void Eagle::Draw(sf::RenderWindow& wnd) const
{
	if (exists)
	DrawSpriteChromaCentered(wnd, Vei2(GetPos()), MapObjectsContainer::mapobjectsbitmap, RectI(Vei2(120, 0), Vei2(159, 39)));
}


MapObjectsContainer::~MapObjectsContainer()
{
	for (std::list<MapObject*>::iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		delete (*it);
	}
	mapobjects.clear();
}

void MapObjectsContainer::MakeEmpty()
{
	for (std::list<MapObject*>::iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		delete (*it);
	}
	mapobjects.clear();
}

void MapObjectsContainer::DestroyObjectsInArea(const RectF & area)
{
	std::list<MapObject*>::iterator it = mapobjects.begin();
	while ( it != mapobjects.end() )
	{
		if ((area.Contains((*it)->GetPos())) && ((*it)->ObjectType() != 4))
		{
			delete *it;
			it = mapobjects.erase(it);
		}
		else { it++; }
	}
}

int MapObjectsContainer::WhatIsThere(const Vec2 & inpos) const
{
	for (std::list<MapObject*>::const_iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		if (((*it)->GetPos() - inpos).LInfNorm() < 20.001)
		{
			return (*it)->ObjectType();
		}
	}
	return 0;
}

MapObject * MapObjectsContainer::WhatIsBlocking(const Vec2 & inpos)
{
	for (std::list<MapObject*>::iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		if ((*it)->IsBlocking(inpos))
		{
			return (*it);
		}
	}
	return NULL;
}

MapObject * MapObjectsContainer::WhatIsBlockingShot(const Vec2 & inpos)
{
	for (std::list<MapObject*>::iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		if ((*it)->IsBlockingShot(inpos))
		{
			return (*it);
		}
	}
	return NULL;
}

bool MapObjectsContainer::IsPassageFree(const Vec2 & inpos) const
{
	if (!inpos.IsWithinRectangle(Vec2(-0.001f, -0.001f), Vec2(600.001f, 600.001f)))
	{
		return false;
	}
	for (std::list<MapObject*>::const_iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		if ((*it)->IsBlocking(inpos))
		{
			return false;
		}
	}
	return true;
}

void MapObjectsContainer::ProcessExplosion(const Vec2 & inpos, const Vec2 & invelocity, int kind, VisualEffectsContainer & effects)
{
	Vec2 spreaddirection;
	spreaddirection = (invelocity.GetRotated90()).GetNormalized();
	Vec2 pos[4] = { inpos + (spreaddirection*5.0f), inpos + (spreaddirection * 15.0f), inpos - (spreaddirection * 5.0f), inpos - (spreaddirection * 15.0f) };
	for (int i = 0; i < 4; i++)
	{
		for (std::list<MapObject*>::iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
		{
			if ((*it)->IsBlocking(pos[i]))
			{
				(*it)->ProcessExplosion(pos[i], kind, effects);
			}
		}

	}
}



void MapObjectsContainer::Draw(sf::RenderWindow& wnd) const
{
	for (std::list<MapObject*>::const_iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		if ((*it)->ObjectType()!=6) (*it)->Draw(wnd);
	}
}

void MapObjectsContainer::DrawForests(sf::RenderWindow& wnd) const
{
	for (std::list<MapObject*>::const_iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		if ((*it)->ObjectType() == 6) (*it)->Draw(wnd);
	}
}

void MapObjectsContainer::InterpretMap(MapInterpretation & mapinterpretation) const
{
	for (int i = 0; i < mapinterpretation.c; i++)
		for (int j = 0; j < mapinterpretation.r; j++)
			mapinterpretation.map[i][j] = 1;
	for (std::list<MapObject*>::const_iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		int cfirst = int(((*it)->GetPos().x - 20) / 10);
		int rfirst = int(((*it)->GetPos().y - 20) / 10);
		if ((*it)->ObjectType() == 1)  // blocks
		{
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-10, -10)))
			{
				mapinterpretation.map[cfirst][rfirst] = 1000;
				mapinterpretation.map[cfirst + 1][rfirst] = 1000;
				mapinterpretation.map[cfirst][rfirst + 1] = 1000;
				mapinterpretation.map[cfirst + 1][rfirst + 1] = 1000;
			}
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(10, -10)))
			{
				mapinterpretation.map[cfirst + 2][rfirst] = 1000;
				mapinterpretation.map[cfirst + 3][rfirst] = 1000;
				mapinterpretation.map[cfirst + 2][rfirst + 1] = 1000;
				mapinterpretation.map[cfirst + 3][rfirst + 1] = 1000;
			}
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-10, 10)))
			{
				mapinterpretation.map[cfirst][rfirst + 2] = 1000;
				mapinterpretation.map[cfirst + 1][rfirst + 2] = 1000;
				mapinterpretation.map[cfirst][rfirst + 3] = 1000;
				mapinterpretation.map[cfirst + 1][rfirst + 3] = 1000;
			}
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(10, 10)))
			{
				mapinterpretation.map[cfirst + 2][rfirst + 2] = 1000;
				mapinterpretation.map[cfirst + 3][rfirst + 2] = 1000;
				mapinterpretation.map[cfirst + 2][rfirst + 3] = 1000;
				mapinterpretation.map[cfirst + 3][rfirst + 3] = 1000;
			}
		}
		if ((*it)->ObjectType() == 2)  // bricks
		{
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-15, -15))) mapinterpretation.map[cfirst][rfirst] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-5, -15))) mapinterpretation.map[cfirst + 1][rfirst] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(5, -15))) mapinterpretation.map[cfirst + 2][rfirst] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(15, -15))) mapinterpretation.map[cfirst + 3][rfirst] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-15, -5))) mapinterpretation.map[cfirst][rfirst + 1] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-5, -5))) mapinterpretation.map[cfirst + 1][rfirst + 1] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(5, -5))) mapinterpretation.map[cfirst + 2][rfirst + 1] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(15, -5))) mapinterpretation.map[cfirst + 3][rfirst + 1] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-15, 5))) mapinterpretation.map[cfirst][rfirst + 2] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-5, 5))) mapinterpretation.map[cfirst + 1][rfirst + 2] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(5, 5))) mapinterpretation.map[cfirst + 2][rfirst + 2] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(15, 5))) mapinterpretation.map[cfirst + 3][rfirst + 2] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-15, 15))) mapinterpretation.map[cfirst][rfirst + 3] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(-5, 15))) mapinterpretation.map[cfirst + 1][rfirst + 3] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(5, 15))) mapinterpretation.map[cfirst + 2][rfirst + 3] = 10;
			if ((*it)->IsBlocking((*it)->GetPos() + Vec2(15, 15))) mapinterpretation.map[cfirst + 3][rfirst + 3] = 10;
		}
		if ((*it)->ObjectType() == 3)  // water
		{
			mapinterpretation.map[cfirst][rfirst] = 1000;
			mapinterpretation.map[cfirst + 1][rfirst] = 1000;
			mapinterpretation.map[cfirst + 2][rfirst] = 1000;
			mapinterpretation.map[cfirst + 3][rfirst] = 1000;
			mapinterpretation.map[cfirst][rfirst + 1] = 1000;
			mapinterpretation.map[cfirst + 1][rfirst + 1] = 1000;
			mapinterpretation.map[cfirst + 2][rfirst + 1] = 1000;
			mapinterpretation.map[cfirst + 3][rfirst + 1] = 1000;
			mapinterpretation.map[cfirst][rfirst + 2] = 1000;
			mapinterpretation.map[cfirst + 1][rfirst + 2] = 1000;
			mapinterpretation.map[cfirst + 2][rfirst + 2] = 1000;
			mapinterpretation.map[cfirst + 3][rfirst + 2] = 1000;
			mapinterpretation.map[cfirst][rfirst + 3] = 1000;
			mapinterpretation.map[cfirst + 1][rfirst + 3] = 1000;
			mapinterpretation.map[cfirst + 2][rfirst + 3] = 1000;
			mapinterpretation.map[cfirst + 3][rfirst + 3] = 1000;
		}
		if ((*it)->ObjectType() == 4)  // eagle
		{
			mapinterpretation.goal = Vei2(cfirst, rfirst);
			mapinterpretation.map[cfirst][rfirst] = 0;
			mapinterpretation.map[cfirst + 1][rfirst] = 0;
			mapinterpretation.map[cfirst + 2][rfirst] = 0;
			mapinterpretation.map[cfirst + 3][rfirst] = 0;
			mapinterpretation.map[cfirst][rfirst + 1] = 0;
			mapinterpretation.map[cfirst + 1][rfirst + 1] = 0;
			mapinterpretation.map[cfirst + 2][rfirst + 1] = 0;
			mapinterpretation.map[cfirst + 3][rfirst + 1] = 0;
			mapinterpretation.map[cfirst][rfirst + 2] = 0;
			mapinterpretation.map[cfirst + 1][rfirst + 2] = 0;
			mapinterpretation.map[cfirst + 2][rfirst + 2] = 0;
			mapinterpretation.map[cfirst + 3][rfirst + 2] = 0;
			mapinterpretation.map[cfirst][rfirst + 3] = 0;
			mapinterpretation.map[cfirst + 1][rfirst + 3] = 0;
			mapinterpretation.map[cfirst + 2][rfirst + 3] = 0;
			mapinterpretation.map[cfirst + 3][rfirst + 3] = 0;
		}
	}
}

Ice::Ice(const Vec2 & inpos) : MapObject(inpos)
{
}

void Ice::Draw(sf::RenderWindow& wnd) const
{
	DrawSpriteChromaCentered(wnd, Vei2(GetPos()), MapObjectsContainer::mapobjectsbitmap, RectI(Vei2(160, 0), Vei2(199, 39)));
}

Forest::Forest(const Vec2 & inpos) : MapObject(inpos)
{
}

void Forest::Draw(sf::RenderWindow& wnd) const
{
	DrawSpriteChromaCentered(wnd, Vei2(GetPos()), MapObjectsContainer::mapobjectsbitmap, RectI(Vei2(200, 0), Vei2(239, 39)));
}

void Bonus::Draw(sf::RenderWindow& wnd) const
{
	if (exists) 
	{
		RectI frame(Vei2(0, 0), Vei2(39, 39));
		if (GetKind() == 1) { frame = RectI(Vei2(0, 0), Vei2(39, 39)); }
		if (GetKind() == 2) { frame = RectI(Vei2(40, 0), Vei2(79, 39)); }
		if (GetKind() == 3) { frame = RectI(Vei2(0, 40), Vei2(39, 79)); }
		if (GetKind() == 4) { frame = RectI(Vei2(40, 40), Vei2(79, 79)); }
		if (GetKind() == 5) { frame = RectI(Vei2(0, 80), Vei2(39, 119)); }
		DrawSpriteChromaCentered(wnd, Vei2(GetPos()), BonusesContainer::bonusesbitmap, frame);
	}
}

void BonusesContainer::MakeEmpty()
{
	for (std::list<Bonus*>::iterator it = bonuses.begin(); it != bonuses.end(); it++)
	{
		delete (*it);
	}
	bonuses.clear();
}

void BonusesContainer::Draw(sf::RenderWindow& wnd) const
{
	for (std::list<Bonus*>::const_iterator it = bonuses.begin(); it != bonuses.end(); it++)
	{
		(*it)->Draw(wnd);
	}
}

void BonusesContainer::Process()
{
	std::list<Bonus*>::iterator it = bonuses.begin();
	while (it != bonuses.end())
	{
		(*it)->Process();
		if ((*it)->Exists() == false) {
			delete *it;
			it = bonuses.erase(it);
		}
		else {
			it++;
		}
	}
}

Bonus * BonusesContainer::GetTouchedBonus(const Vec2 & pos)
{
	for (std::list<Bonus*>::iterator it = bonuses.begin(); it != bonuses.end(); it++)
	{
		if ((((*it)->GetPos() - pos).LInfNorm() < 35) && ((*it)->Exists()))
		{
			return *it;
		};
	}
	return nullptr;
}
