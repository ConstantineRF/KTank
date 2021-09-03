#include "Game.h"
#include "dirent.h"
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>

Game::Game(sf::RenderWindow& wnd)
	:
	wnd(wnd),
	LAG(0),
	parameters(),
	variables(parameters),
	gamestatus(MainMenu),
	performancestats(),
	eagle(nullptr), tank1(nullptr), tank2(nullptr),
	cursor(0), listcursor(0), timertocursormove(0)
{
	// Load font
	consolasfont.loadFromFile("CONSOLA.ttf");

	// Get List of Maps
	numberofmaps = 0;
	struct dirent *entry;

	DIR *dir = opendir("maps");

	if (dir != NULL)
	{
		std::string currentfilename;
		while ((entry = readdir(dir)) != NULL)
		{
			currentfilename = entry->d_name;
			if (entry->d_name[0] != '.')
			{
				mapfilenames.push_back(std::string("maps\\") + entry->d_name);
				numberofmaps++;
			}
		}
		closedir(dir);
	}

	PREVIOUS_TIME = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Game::InitializeNewGame(const std::string & filename)
{
	variables.Reset(parameters);

	mapobjects.MakeEmpty();
	tanks.MakeEmpty();
	bonuses.MakeEmpty();

	std::ifstream mapfile(filename);
	int charnum = 0;
	for (char byte = mapfile.get(); mapfile.good(); byte = mapfile.get())
	{
		if (byte == '1') mapobjects.AddMapObject(new Blocks(Vec2(float((charnum % 15) * 40 + 20), float(((charnum - charnum % 15) / 15) * 40 + 20))));
		if (byte == '2') mapobjects.AddMapObject(new Bricks(Vec2(float((charnum % 15) * 40 + 20), float(((charnum - charnum % 15) / 15) * 40 + 20))));
		if (byte == '3') mapobjects.AddMapObject(new Water(Vec2(float((charnum % 15) * 40 + 20), float(((charnum - charnum % 15) / 15) * 40 + 20))));
		if (byte == '4') { eagle = new Eagle(Vec2(float((charnum % 15) * 40 + 20), float(((charnum - charnum % 15) / 15) * 40 + 20))); mapobjects.AddMapObject(eagle); }
		if (byte == '5') mapobjects.AddMapObject(new Ice(Vec2(float((charnum % 15) * 40 + 20), float(((charnum - charnum % 15) / 15) * 40 + 20))));
		if (byte == '6') mapobjects.AddMapObject(new Forest(Vec2(float((charnum % 15) * 40 + 20), float(((charnum - charnum % 15) / 15) * 40 + 20))));
		if ((byte >= '0') && (byte <= '6'))	charnum++;
	}
	mapinterpretation = MapInterpretation(60, 60, Vei2(28, 56));

	mapobjects.InterpretMap(mapinterpretation);

	tanks.CreateTank(Vec2(220.f, 580.f), 1, 2, 1);
	tank1 = tanks.GetTankAtLocation(Vec2(220.f, 580.f));
	if (parameters.gameplayers == 1)
	{
		tank2 = nullptr;
	}
	else
	{
		tanks.CreateTank(Vec2(380.f, 580.f), 1, 2, 2);
		tank2 = tanks.GetTankAtLocation(Vec2(380.f, 580.f));
	}
}

/*
Game::~Game()
{
	for (std::list<MapObject*>::iterator it = mapobjects.begin(); it != mapobjects.end(); it++)
	{
		delete (*it);
	}
}
*/

void Game::Go()
{
	long long int CURRENT_TIME = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	long long int ELAPSED = CURRENT_TIME - PREVIOUS_TIME;
	PREVIOUS_TIME = CURRENT_TIME;
	LAG += ELAPSED;

	wnd.clear();
	performancestats.MODELUPDATESPERFRAMEUPDATE = 0;
	while (LAG >= MainConstants::MS_PER_UPDATE)
	{
		UpdateModel();
		LAG -= MainConstants::MS_PER_UPDATE;
		performancestats.MODELUPDATESPERFRAMEUPDATE++;
	}

	long long int NEW_CURRENT_TIME = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (performancestats.MODELUPDATESPERFRAMEUPDATE > 0) performancestats.MSPERMODELUPDATE = (NEW_CURRENT_TIME - CURRENT_TIME) / performancestats.MODELUPDATESPERFRAMEUPDATE;
	ComposeFrame();
	performancestats.MSTODRAW = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - NEW_CURRENT_TIME;
	wnd.display();
}

void Game::UpdateModel()
{
	if (gamestatus == GameInProgress)
	{
		WatersAnimation::loop = (WatersAnimation::loop + 1) % (3 * 16);
		for (int player = 1; player <= parameters.gameplayers; player++)
		{
			if ((tanks.GetTeamTanksNumber(player) == 0) && (variables.lifesleft > 0))
			{
				// player's tank dead, but there are more lives
				const Vec2 possible_spawn_locations[2] = { Vec2(220.0f + 160.0f*(player-1), 580.0f), Vec2(380.0f - 160.0f*(player-1), 580.0f) };
				Vec2 try_location = possible_spawn_locations[0];
				if (tanks.NoTankHere(try_location + Vec2(-15.0f, -15.0f)) && tanks.NoTankHere(try_location + Vec2(15.0f, -15.0f)) &&
					tanks.NoTankHere(try_location + Vec2(-15.0f, 15.0f)) && tanks.NoTankHere(try_location + Vec2(15.0f, 15.0f)))
				{
					// location to the left of eagle is available
					tanks.CreateTank(try_location, 1, 2, player);
					if (player == 1) { tank1 = tanks.GetTankAtLocation(try_location); }
					else { tank2 = tanks.GetTankAtLocation(try_location); }
					variables.lifesleft--;
					effects.AddVisualEffect(try_location, TANK_CREATION);
				}
				else
				{
					// try placing to the right of eagle
					try_location = possible_spawn_locations[1];
					if (tanks.NoTankHere(try_location + Vec2(-15.0f, -15.0f)) && tanks.NoTankHere(try_location + Vec2(15.0f, -15.0f)) &&
						tanks.NoTankHere(try_location + Vec2(-15.0f, 15.0f)) && tanks.NoTankHere(try_location + Vec2(15.0f, 15.0f)))
					{
						tanks.CreateTank(try_location, 1, 2, player);
						if (player == 1) { tank1 = tanks.GetTankAtLocation(try_location); }
						else { tank2 = tanks.GetTankAtLocation(try_location); }
						variables.lifesleft--;
						effects.AddVisualEffect(try_location, TANK_CREATION);
					}
				}
			}
		}
		// Process enemy tanks generation
		if (variables.tankgenerationcountdown == 0)
		{
			// create a new enemy tank
			const Vec2 possible_spawn_locations[3] = { Vec2(20.0f, 20.0f), Vec2(300.0f, 20.0f), Vec2(580.0f, 20.0f) };
			const int z = rand() % 3;
			Vec2 try_location = possible_spawn_locations[z];
			if (tanks.NoTankHere(try_location + Vec2(-15.0f, -15.0f)) && tanks.NoTankHere(try_location + Vec2(15.0f, -15.0f)) &&
				tanks.NoTankHere(try_location + Vec2(-15.0f, 15.0f)) && tanks.NoTankHere(try_location + Vec2(15.0f, 15.0f)) &&
				(variables.tankgenerationnumber < parameters.TOTAL_ENEMY_TANKS))
			{
				tanks.CreateTank(try_location, parameters.generationsequence[variables.tankgenerationnumber] % parameters.TOTAL_ENEMY_TANKS, 4, -1);
				variables.tankgenerationnumber++;
				variables.tankgenerationcountdown = parameters.TANKGENERATIONINTERVAL;
				effects.AddVisualEffect(try_location, TANK_CREATION);
			}
		}
		else if (((tanks.GetTeamTanksNumber(-1) < parameters.MAX_ENEMY_TANKS) || (variables.tankgenerationcountdown > 200)) && (variables.tankgenerationcountdown > 0))
		{
			variables.tankgenerationcountdown--;
		}

		if (variables.bonusgenerationcountdown <= 0)
		{
			bool tryagain = true;
			Vec2 newbonuspos;
			while (tryagain)
			{
				newbonuspos = Vec2(Vei2((rand() % 14) * 40 + 40, (rand() % 14) * 40 + 40));
				tryagain = false;
				if ((eagle->GetPos() - newbonuspos).LInfNorm() <= 100) tryagain = true;
				if (tank1->IsLive()) { if ((tank1->GetPos() - newbonuspos).LInfNorm() <= 60) tryagain = true; }
				if (parameters.gameplayers == 2)
				{
					if (tank2->IsLive()) { if ((tank2->GetPos() - newbonuspos).LInfNorm() <= 60) tryagain = true; }
				}
			}
			bonuses.CreateBonus(new Bonus(newbonuspos, parameters.bonusgenerationsequence[variables.bonusgenerationnumber % parameters.bonusgenerationsequence.size()], parameters.BONUSLIFEDURATION));
			variables.bonusgenerationnumber++;
			variables.bonusgenerationcountdown = parameters.BONUSGENERATIONINTERVAL;
		}
		else
		{
			variables.bonusgenerationcountdown--;
		}


		if (variables.fortress)
		{
			variables.fortresstimer--;
			if (variables.fortresstimer <= 0)
			{
				variables.fortress = false;
				// Remove fortress
				mapobjects.DestroyObjectsInArea(RectF(eagle->GetPos() + Vec2(-40.1f, -40.1f), eagle->GetPos() + Vec2(40.1f, 40.1f)));

				// Put bricks
				mapobjects.AddMapObject(new Bricks(eagle->GetPos() + Vec2(-40.f, 0.f)));
				mapobjects.AddMapObject(new Bricks(eagle->GetPos() + Vec2(-40.f, -40.f)));
				mapobjects.AddMapObject(new Bricks(eagle->GetPos() + Vec2(0.f, -40.f)));
				mapobjects.AddMapObject(new Bricks(eagle->GetPos() + Vec2(40.f, -40.f)));
				mapobjects.AddMapObject(new Bricks(eagle->GetPos() + Vec2(40.f, 0.f)));

				mapinterpretation = MapInterpretation(60, 60, Vei2(28, 56));

				mapobjects.InterpretMap(mapinterpretation);
			}
		}

		tanks.Refill();

		tanks.Progress();

		tanks.ProcessUserControls(wnd, rockets, mapinterpretation, parameters.gameplayers);

		rockets.ProcessRockets(mapobjects, tanks, effects);


		// Bonuses require special treatment, hence they are in this top layer
		Bonus* consumedbonus = nullptr;
		if (tank1->IsLive())
		{
			consumedbonus = bonuses.GetTouchedBonus(tank1->GetPos());
		}
		if (parameters.gameplayers == 2)
		{
			if ((consumedbonus == nullptr) && (tank2->IsLive()))
			{
				consumedbonus = bonuses.GetTouchedBonus(tank2->GetPos());
			}
		}
		if (consumedbonus != nullptr)
		{
			int bonuskind = consumedbonus->GetKind();
			consumedbonus->Eliminate();  // Cleaned the bonus from map and memory
			if (bonuskind == 1) { variables.lifesleft++; }
			if (bonuskind == 2) 
			{ 
				if (tank1->IsLive()) { tank1->ConsumeRevolver(); } 
				if (parameters.gameplayers == 2) { if (tank2->IsLive()) { tank2->ConsumeRevolver(); } }
			};
			if (bonuskind == 3) 
			{ 
				if (tank1->IsLive()) { tank1->ConsumeShield(); }
				if (parameters.gameplayers == 2) { if (tank2->IsLive()) { tank2->ConsumeShield(); } }
			};
			if (bonuskind == 4) { tanks.BlowUpTeam(-1, effects); }
			if (bonuskind == 5)
			{
				// Process Fortress consumption
				if (variables.fortress) { variables.fortresstimer += parameters.FORTDURATION; }
				else
				{
					// First, kill all tanks near eagle
					tanks.BlowUpTeam(-1, effects, RectF(eagle->GetPos() + Vec2(-80.1f, -80.1f), eagle->GetPos() + Vec2(80.1f, 80.1f)));

					// Second, remove remaining bricks around eagle
					mapobjects.DestroyObjectsInArea(RectF(eagle->GetPos() + Vec2(-40.1f, -40.1f), eagle->GetPos() + Vec2(40.1f, 40.1f)));

					// Third, put fortress
					mapobjects.AddMapObject(new Fortress(eagle->GetPos() + Vec2(-40.f, 0.f)));
					mapobjects.AddMapObject(new Fortress(eagle->GetPos() + Vec2(-40.f, -40.f)));
					mapobjects.AddMapObject(new Fortress(eagle->GetPos() + Vec2(0.f, -40.f)));
					mapobjects.AddMapObject(new Fortress(eagle->GetPos() + Vec2(40.f, -40.f)));
					mapobjects.AddMapObject(new Fortress(eagle->GetPos() + Vec2(40.f, 0.f)));

					variables.fortress = true;
					variables.fortresstimer = parameters.FORTDURATION;

					// Also, fortress will need to expire and return to old status
				}
			}
		}

		bonuses.Process();

		if (variables.checkmaploop == 0)
		{
			mapobjects.InterpretMap(mapinterpretation);
			variables.checkmaploop = 100;
		}
		else { variables.checkmaploop--; }
		if (variables.rerouteloop == 0)
		{
			mapinterpretation.SeekRoutesWithQueue();
			variables.rerouteloop = 2;
		}
		else
		{
			variables.rerouteloop--;
		}

		tanks.Move(mapobjects);

		effects.EvolveVisualEffects();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))  // Q key
		{
			variables._soilon = !variables._soilon;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))  // Pause
		{
			gamestatus = Pause;
		}

		if (((tanks.GetTeamTanksNumber(1) == 0) && (tanks.GetTeamTanksNumber(2) == 0) && (variables.lifesleft == 0)) || (eagle->exists == false))
		{
			variables.endgametimer--;
			if (variables.endgametimer <= 0) gamestatus = GameOver;
		}
		else if ((tanks.GetTeamTanksNumber(-1) == 0) && (variables.tankgenerationnumber == parameters.TOTAL_ENEMY_TANKS))
		{
			variables.endgametimer--;
			if (variables.endgametimer <= 0)	gamestatus = GameWon;
		}
	}
	else if (gamestatus == Pause)
	{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		gamestatus = GameInProgress;
	}
	else if (gamestatus == MainMenu)
	{
		if (timertocursormove > 0) { timertocursormove--; }
		if (cursor > 1) { cursor = 1; }
		if (cursor < 0) { cursor = 0; }
		if (timertocursormove <= 0)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (cursor > 0) { cursor--; }
				timertocursormove = 5;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (cursor < 1) { cursor++; }
				timertocursormove = 5;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				if (cursor == 0)
				{
					gamestatus = MapSelection;
					timertocursormove = 5;
					cursor = 0;
				}
				if (cursor == 1)
				{
					gamestatus = OptionsConfig;
					timertocursormove = 5;
					cursor = 0;
				}
			}
		}
	}
	else if (gamestatus == OptionsConfig)
	{
		if (timertocursormove > 0) { timertocursormove--; }
		if (cursor > 3) { cursor = 3; }
		if (cursor < 0) { cursor = 0; }
		if (timertocursormove <= 0)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (cursor > 0) { cursor--; }
				timertocursormove = 5;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (cursor < 3) { cursor++; }
				timertocursormove = 5;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				if (cursor == 0)
				{
					options::currentdiff++;
					if (options::currentdiff == options::diffseqs.size()) { options::currentdiff = 0; }
					parameters.generationsequence = options::diffseqs[options::currentdiff];
					parameters.TOTAL_ENEMY_TANKS = parameters.generationsequence.size();
					timertocursormove = 5;
				}
				if (cursor == 1)
				{
					if (parameters.MAX_ENEMY_TANKS < 20) { parameters.MAX_ENEMY_TANKS++; }
					timertocursormove = 5;
				}
				if (cursor == 2)
				{
					if (parameters.TANKGENERATIONINTERVAL < 20 * (1000 / MainConstants::MS_PER_UPDATE)) { parameters.TANKGENERATIONINTERVAL += (1000 / MainConstants::MS_PER_UPDATE); }
					timertocursormove = 5;
				}
				if (cursor == 3)
				{
					parameters.gameplayers=2;
					timertocursormove = 5;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				if (cursor == 0)
				{
					options::currentdiff--;
					if (options::currentdiff < 0) { options::currentdiff = options::diffseqs.size() - 1; }
					parameters.generationsequence = options::diffseqs[options::currentdiff];
					parameters.TOTAL_ENEMY_TANKS = parameters.generationsequence.size();
					timertocursormove = 5;
				}
				if (cursor == 1)
				{
					if (parameters.MAX_ENEMY_TANKS > 1) { parameters.MAX_ENEMY_TANKS--; }
					timertocursormove = 5;
				}
				if (cursor == 2)
				{
					if (parameters.TANKGENERATIONINTERVAL > 3 * (1000 / MainConstants::MS_PER_UPDATE)) { parameters.TANKGENERATIONINTERVAL -= (1000 / MainConstants::MS_PER_UPDATE); }
					timertocursormove = 5;
				}
				if (cursor == 3)
				{
					parameters.gameplayers = 1;
					timertocursormove = 5;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				if (cursor == 0)
				{
					options::currentdiff++;
					if (options::currentdiff == options::diffseqs.size()) { options::currentdiff = 0; }
					parameters.generationsequence = options::diffseqs[options::currentdiff];
					parameters.TOTAL_ENEMY_TANKS = parameters.generationsequence.size();
					timertocursormove = 5;
				}
				if (cursor == 1)
				{
					timertocursormove = 5;
				}
				if (cursor == 2)
				{
					timertocursormove = 5;
				}
				if (cursor == 3)
				{
					parameters.gameplayers = 3 - parameters.gameplayers;
					timertocursormove = 5;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
			{
				gamestatus = MainMenu;
				timertocursormove = 5;
				cursor = 1;
			}
		}
	}
	else if (gamestatus == MapSelection)
	{
		if (timertocursormove > 0) { timertocursormove--; }
		if (timertocursormove <= 0)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (cursor > 0)	cursor--;
				if (listcursor > cursor) listcursor = cursor;
				timertocursormove = 5;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (cursor < numberofmaps - 1)	cursor++;
				if (listcursor < cursor - 14) listcursor = cursor - 14;
				timertocursormove = 5;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				InitializeNewGame(mapfilenames[cursor]);
				gamestatus = GameInProgress;
				timertocursormove = 5;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
			{
				gamestatus = MainMenu;
				timertocursormove = 5;
				cursor = 0;
			}
		}
	}
	else if (gamestatus == GameOver)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			gamestatus = MainMenu;
			variables.lifesleft = 2;
		}
	}
	else if (gamestatus == GameWon)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			gamestatus = MainMenu;
		}
	}
}

void Game::ComposeFrame()
{
	if ((gamestatus == GameInProgress) || (gamestatus == Pause))
	{
		for (int i = 0; i <= 15; i++)
		{
			for (int j = 0; j <= 15; j++)
			{
				// UPDATE HERE AFTER GRAPHICS FIGURED
				//gfx.PutPixel(gfx.leftindent + i * 40, gfx.topindent + j * 40, 255, 255, 255);
			}
		}
		if (variables._soilon)
		{
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					// UPDATE HERE AFTER GRAPHICS FIGURED
					//gfx.DrawSpriteCentered(Vei2(i * 40 + 40, j * 40 + 40), MapObjectsContainer::mapobjectsbitmap, RectI(Vei2(40, 40), Vei2(79, 79)));
					DrawSpriteChromaCenteredRotated(wnd, Vei2(i * 40 + 20, j * 40 + 20), MapObjectsContainer::mapobjectsbitmap, RectI(Vei2(40, 40), Vei2(79, 79)));
				}
			}
		}
		long long int FRAME_START_TIME = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		mapobjects.Draw(wnd);
		long long int MAP_DRAWN_TIME = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		tanks.Draw(wnd);
		long long int TANKS_DRAWN_TIME = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		mapobjects.DrawForests(wnd);
		rockets.Draw(wnd);
		bonuses.Draw(wnd);
		effects.Draw(wnd);

		// UPDATE HERE AFTER GRAPHICS FIGURED
		/*
		gfx.DrawRect(RectI(Vei2(19, 19), Vei2(621, 621)), Colors::Green);
		gfx.DrawRect(RectI(Vei2(17, 17), Vei2(623, 623)), Colors::Green);
		gfx.DrawRect(RectI(Vei2(14, 14), Vei2(626, 626)), Colors::Green);
		*/

		long long int REMAINDER_DRAWN_TIME = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		kprint("TANK Game", Vei2(650, 10), consolasfont, sf::Color::Green, wnd);

		DrawSpriteChromaCenteredRotated(wnd, Vei2(685, 65), TanksContainer::redtankbitmap, RectI(Vei2(0, 0), Vei2(38, 38)), 4);
		kprint(": " + std::to_string(parameters.TOTAL_ENEMY_TANKS - variables.tankgenerationnumber), Vei2(710, 53), consolasfont, sf::Color::Green, wnd);

		DrawSpriteChromaCenteredRotated(wnd, Vei2(685, 140), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 2);
		kprint(": " + std::to_string(variables.lifesleft), Vei2(710, 128), consolasfont, sf::Color::Green, wnd);

		if (tank1->RevolverTimer() > 0)
		{
			DrawSpriteChromaCentered(wnd, Vei2(650, 200), BonusesContainer::bonusesbitmap, RectI(Vei2(40, 0), Vei2(79, 39)));
			kprint(std::to_string(tank1->RevolverTimer() / 50), Vei2(640, 225), consolasfont, (tank1->RevolverTimer() < 150) ? sf::Color::Red : sf::Color::Green, wnd);
		}

		if (tank1->ShieldTimer() > 0)
		{
			DrawSpriteChromaCentered(wnd, Vei2(710, 200), BonusesContainer::bonusesbitmap, RectI(Vei2(0, 40), Vei2(39, 79)));
			kprint(std::to_string(tank1->ShieldTimer() / 50), Vei2(700, 225), consolasfont, (tank1->ShieldTimer() < 150) ? sf::Color::Red : sf::Color::Green, wnd);
		}

		if (variables.fortresstimer > 0)
		{
			DrawSpriteChromaCentered(wnd, Vei2(770, 200), BonusesContainer::bonusesbitmap, RectI(Vei2(0, 80), Vei2(39, 119)));
			kprint(std::to_string(variables.fortresstimer / 50), Vei2(760, 225), consolasfont, (variables.fortresstimer < 150) ? sf::Color::Red : sf::Color::Green, wnd);
		}

		if (parameters.gameplayers == 2)
		{
			if (tank2->RevolverTimer() > 0)
			{
				DrawSpriteChromaCentered(wnd, Vei2(650, 270), BonusesContainer::bonusesbitmap, RectI(Vei2(40, 0), Vei2(79, 39)));
				kprint(std::to_string(tank2->RevolverTimer() / 50), Vei2(640, 295), consolasfont, (tank2->RevolverTimer() < 150) ? sf::Color::Red : sf::Color::Green, wnd);
			}

			if (tank2->ShieldTimer() > 0)
			{
				DrawSpriteChromaCentered(wnd, Vei2(710, 270), BonusesContainer::bonusesbitmap, RectI(Vei2(0, 40), Vei2(39, 79)));
				kprint(std::to_string(tank2->ShieldTimer() / 50), Vei2(700, 295), consolasfont, (tank2->ShieldTimer() < 150) ? sf::Color::Red : sf::Color::Green, wnd);
			}
		}

		kprint("Tanks ttl " + std::to_string(tanks.tanks.size()), Vei2(640, 395), consolasfont, sf::Color::Blue, wnd);
		kprint("UPF: " + std::to_string(performancestats.MODELUPDATESPERFRAMEUPDATE), Vei2(640, 420), consolasfont, sf::Color::Blue, wnd);
		kprint("ModUpdMS: " + std::to_string(performancestats.MSPERMODELUPDATE), Vei2(640, 445), consolasfont, sf::Color::Blue, wnd);
		kprint("DrawMS: " + std::to_string(performancestats.MSTODRAW), Vei2(640, 470), consolasfont, sf::Color::Blue, wnd);
		kprint("TanksMS: " + std::to_string(TANKS_DRAWN_TIME - MAP_DRAWN_TIME), Vei2(640, 495), consolasfont, sf::Color::Blue, wnd);
		kprint("MapMS: " + std::to_string(MAP_DRAWN_TIME - FRAME_START_TIME), Vei2(640, 520), consolasfont, sf::Color::Blue, wnd);
		kprint("OtherMS: " + std::to_string(REMAINDER_DRAWN_TIME - MAP_DRAWN_TIME), Vei2(640, 545), consolasfont, sf::Color::Blue, wnd);
		kprint("(" + std::to_string(int(tank1->GetPos().x)) + "; " + std::to_string(int(tank1->GetPos().y)) + ")", Vei2(630, 570), consolasfont, sf::Color::Blue, wnd);


		if (gamestatus == Pause)
		{
			kprint("PAUSE", Vei2(340, 200), consolasfont, sf::Color::Green, wnd);
			kprint("Press SPACE to resume", Vei2(200, 300), consolasfont, sf::Color::Green, wnd);
		}
	}
	else if (gamestatus == MainMenu)
	{
		kprint("TANK GAME", Vei2(340, 50), consolasfont, sf::Color::Green, wnd);
		kprint("Version 0", Vei2(340, 75), consolasfont, sf::Color::Green, wnd);
		kprint("Play", Vei2(275, 150), consolasfont, sf::Color::Green, wnd);
		kprint("Options", Vei2(275, 175), consolasfont, sf::Color::Green, wnd);

		DrawSpriteChromaCentered(wnd, Vei2(265, 150 + 20 + cursor * 25), Rocket::rocketsbitmap, RectI(Vei2(0, 0), Vei2(19, 19)));
		kprint("Navigate menu by arrow buttons and ENTER", Vei2(130, 500), consolasfont, sf::Color::Green, wnd);
		kprint("Created by Konstantin Kalinchenko", Vei2(190, 550), consolasfont, sf::Color::Green, wnd);

	}
	else if (gamestatus == OptionsConfig)
	{
		kprint("TANK GAME", Vei2(340, 50), consolasfont, sf::Color::Green, wnd);
		kprint("Options", Vei2(350, 75), consolasfont, sf::Color::Green, wnd);
		kprint("Enemy Army:", Vei2(150, 150), consolasfont, sf::Color::Green, wnd);
		kprint(options::diffnames[options::currentdiff], Vei2(400, 150), consolasfont, sf::Color::Green, wnd);
		kprint("Enemy Capacity:", Vei2(150, 175), consolasfont, sf::Color::Green, wnd);
		kprint(std::to_string(parameters.MAX_ENEMY_TANKS), Vei2(400, 175), consolasfont, sf::Color::Green, wnd);
		kprint("Enemy Spawn time:", Vei2(150, 200), consolasfont, sf::Color::Green, wnd);
		kprint(std::to_string(parameters.TANKGENERATIONINTERVAL / (1000 / MainConstants::MS_PER_UPDATE)) + " seconds", Vei2(400, 200), consolasfont, sf::Color::Green, wnd);
		kprint("Number of Players:", Vei2(150, 225), consolasfont, sf::Color::Green, wnd);
		kprint(parameters.gameplayers==1 ? "Solo" : "Bros", Vei2(400, 225), consolasfont, sf::Color::Green, wnd);
		DrawSpriteChromaCentered(wnd, Vei2(140, 150 + 20 + cursor * 25), Rocket::rocketsbitmap, RectI(Vei2(0, 0), Vei2(19, 19)));

		// Draw enemy army
		for (int i = 0; i < parameters.TOTAL_ENEMY_TANKS; i++)
		{
			if (parameters.generationsequence[i] == 0) DrawSpriteChromaCenteredRotated(wnd, Vei2(50 + 700 * i / parameters.TOTAL_ENEMY_TANKS, 400), TanksContainer::basictankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 2);
			if (parameters.generationsequence[i] == 1) DrawSpriteChromaCenteredRotated(wnd, Vei2(50 + 700 * i / parameters.TOTAL_ENEMY_TANKS, 400), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 2);
			if (parameters.generationsequence[i] == 2) DrawSpriteChromaCenteredRotated(wnd, Vei2(50 + 700 * i / parameters.TOTAL_ENEMY_TANKS, 400), TanksContainer::redtankbitmap, RectI(Vei2(0, 0), Vei2(38, 38)), 2);
			if (parameters.generationsequence[i] == 3) DrawSpriteChromaCenteredRotated(wnd, Vei2(50 + 700 * i / parameters.TOTAL_ENEMY_TANKS, 400), TanksContainer::greentankbitmap, RectI(Vei2(0, 0), Vei2(49, 39)), 2);
		}

		kprint("Navigate menu by arrow buttons and ENTER", Vei2(130, 500), consolasfont, sf::Color::Green, wnd);
		kprint("Press BACKSPACE to return to main menu", Vei2(130, 525), consolasfont, sf::Color::Green, wnd);
		kprint("Created by Konstantin Kalinchenko", Vei2(190, 550), consolasfont, sf::Color::Green, wnd);

	}
	else if (gamestatus == MapSelection)
	{
		//gfx.DrawSpriteChromaCenteredRotated(Vei2(400, 50), TanksContainer::redtankbitmap, RectI(Vei2(0, 0), Vei2(38, 38)), 1, Colors::White, RectI(Vei2(0, 0), Vei2(gfx.ScreenWidth - 1, gfx.ScreenHeight - 1)));
		
		kprint("TANK GAME", Vei2(340, 50), consolasfont, sf::Color::Green, wnd);
		kprint("Version 0", Vei2(340, 75), consolasfont, sf::Color::Green, wnd);

		if (numberofmaps > 0)
		{
			for (int k = 0; (listcursor + k < numberofmaps) && (k < 15); k++)
			{
				//printer.PrintColor(mapfilenames[listcursor + k], Vei2(275, 175 + k * 25), Colors::Green, gfx);
				kprint(mapfilenames[listcursor + k], Vei2(275, 125 + k * 25), consolasfont, sf::Color::Green, wnd);
			}
		}
		DrawSpriteChromaCentered(wnd, Vei2(265, 125 + 20 + (cursor - listcursor) * 25), Rocket::rocketsbitmap, RectI(Vei2(0, 0), Vei2(19, 19)));
		kprint("Navigate menu by arrow buttons and ENTER", Vei2(130, 500), consolasfont, sf::Color::Green, wnd);
		kprint("Press BACKSPACE to return to main menu", Vei2(130, 525), consolasfont, sf::Color::Green, wnd);
		kprint("Created by Konstantin Kalinchenko", Vei2(190, 550), consolasfont, sf::Color::Green, wnd);
		//printer.PrintColor("Created by Konstantin Kalinchenko", Vei2(190, 600), Colors::Green, gfx);
	}
	else if (gamestatus == GameOver)
	{
		kprint("GAME OVER", Vei2(335, 100), consolasfont, sf::Color::Red, wnd);

		DrawSpriteChromaCenteredRotated(wnd, Vei2(320, 180), TanksContainer::greentankbitmap, RectI(Vei2(0, 0), Vei2(49, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(400, 180), TanksContainer::greentankbitmap, RectI(Vei2(0, 0), Vei2(49, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(480, 180), TanksContainer::greentankbitmap, RectI(Vei2(0, 0), Vei2(49, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(280, 220), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(360, 220), TanksContainer::redtankbitmap, RectI(Vei2(0, 0), Vei2(38, 38)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(440, 220), TanksContainer::redtankbitmap, RectI(Vei2(0, 0), Vei2(38, 38)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(520, 220), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(320, 260), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(400, 260), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(480, 260), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(280, 300), TanksContainer::basictankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(360, 300), TanksContainer::basictankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(440, 300), TanksContainer::basictankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 4);
		DrawSpriteChromaCenteredRotated(wnd, Vei2(520, 300), TanksContainer::basictankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 4);

		kprint("Press SPACE for main menu", Vei2(240, 500), consolasfont, sf::Color::Red, wnd);
		//printer.PrintColor("Press SPACE for main menu", Vei2(250, 550), Colors::Red, gfx);
	}
	else if (gamestatus == GameWon)
	{
		kprint("YOU WON!!!", Vei2(335, 100), consolasfont, sf::Color::Yellow, wnd);

		DrawSpriteChromaCenteredRotated(wnd, Vei2(360, 220), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 2);
		DrawSpriteChromaCentered(wnd, Vei2(400, 220), MapObjectsContainer::mapobjectsbitmap, RectI(Vei2(120, 0), Vei2(159, 39)));
		DrawSpriteChromaCenteredRotated(wnd, Vei2(440, 220), TanksContainer::bluetankbitmap, RectI(Vei2(0, 0), Vei2(39, 39)), 2);

		kprint("Try another map with accumulated lifes!", Vei2(160, 125), consolasfont, sf::Color::Yellow, wnd);
		kprint("Press SPACE for main menu", Vei2(230, 500), consolasfont, sf::Color::Yellow, wnd);
	}

}






void kprint(const std::string & str, const Vei2 & pos, const sf::Font & font, const sf::Color & color, sf::RenderWindow& wnd)
{
	sf::Text text;

	// select the font
	text.setFont(font); // font is a sf::Font

	// set the string to display
	text.setString(str);

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setFillColor(color);

	text.setPosition(pos.x, pos.y);

	// set the text style
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	// inside the main loop, between window.clear() and window.display()
	wnd.draw(text);

}


