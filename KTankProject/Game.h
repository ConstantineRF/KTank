#pragma once
#include <SFML\Graphics.hpp>
#include "MapObject.h"
#include "Tank.h"
#include <string>
#include <vector>
#include <cassert>

class MainConstants
{
	// These constants are not meant to be changed ever.
public:
	static constexpr long long int MS_PER_UPDATE = 20;
};

namespace options
{
	static std::vector<std::vector<int>> diffseqs = {
		{ 0,0,1,0,0, 1,0,0,2,0, 1,0,2,0,1, 3,0,1,2,3},
		{ 0,0,0,1,0, 0,1,0,0,2, 1,0,1,2,0, 0,1,2,1,2, 0,1,3,0,2, 3,0,1,3,3 }, 
		{ 0,0,0,1,0, 0,1,0,0,2, 1,0,1,2,0, 0,1,2,1,2, 0,1,3,0,2, 3,0,1,3,3, 1,2,3,2,3, 1,2,3,3,3 }
	};
	static std::vector<std::string> diffnames = { "TOUCH ME GENTLY", "DEAD MEAT", "HURT ME PLENTY" };
	static int currentdiff = 1;
	//static constexpr int numdiffs = 3;
}

class GameParameters
{
	// These reflect difficulty calibration and expected to be stable during a single game level
public:
	GameParameters()
	{
		MAX_ENEMY_TANKS = 5;
		TANKGENERATIONINTERVAL = 300;
		BONUSGENERATIONINTERVAL = 600;
		BONUSLIFEDURATION = 800;
		FORTDURATION = 500;
		generationsequence = { 0,0,0,1,0, 0,1,0,0,2, 1,0,1,2,0, 0,1,2,1,2, 0,1,3,0,2, 3,0,1,3,3 };
		TOTAL_ENEMY_TANKS = generationsequence.size();
		bonusgenerationsequence = { 1,2,3,4,5 };
	}
	int MAX_ENEMY_TANKS;
	int TOTAL_ENEMY_TANKS;
	int TANKGENERATIONINTERVAL;
	int BONUSGENERATIONINTERVAL;
	int BONUSLIFEDURATION;
	int FORTDURATION;
	std::vector<int> generationsequence;
	std::vector<int> bonusgenerationsequence;
};

class GameVariables
{
	// These reflect and navigate game dynamics
public:
	GameVariables(const GameParameters & parameters) 
	{
		Reset(parameters);
		lifesleft = 2;
		_soilon = false;
	}
	void Reset(const GameParameters & parameters)
	{
		tankgenerationcountdown = 100;
		bonusgenerationcountdown = 500;
		tankgenerationnumber = 0;
		bonusgenerationnumber = 0;
		checkmaploop = 0;
		rerouteloop = 0;
		fortresstimer = 0;
		fortress = false;
		endgametimer = 150;
	}
	int tankgenerationcountdown;
	int tankgenerationnumber;
	int bonusgenerationcountdown;
	int bonusgenerationnumber;
	int lifesleft;
	int fortresstimer;
	bool fortress;
	int checkmaploop;
	int rerouteloop;
	//int testreadchars;
	bool _soilon;
	int endgametimer;
};

class PerformanceStats
{
public:
	PerformanceStats() : MODELUPDATESPERFRAMEUPDATE(0), MSTODRAW(0), MSPERMODELUPDATE(0) {};
	int MODELUPDATESPERFRAMEUPDATE, MSTODRAW, MSPERMODELUPDATE;
};

class Game
{
public:
	Game(sf::RenderWindow& wnd);
	Game(const Game&) = delete;
	//~Game();
	Game& operator=(const Game&) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	void InitializeNewGame(const std::string & filename);
	/********************************/
private:
	sf::RenderWindow& wnd;
	sf::Font consolasfont;
	/********************************/
	TanksContainer tanks;
	MapObjectsContainer mapobjects;
	BonusesContainer bonuses;
	RocketsContainer rockets;
	VisualEffectsContainer effects;
	MapInterpretation mapinterpretation;

	Eagle* eagle;
	Tank* tank1;
	Tank* tank2;
	
	GameParameters parameters;
	GameVariables variables;

	enum GameStatus { MainMenu, MapSelection, OptionsConfig, GameInProgress, Pause, GameOver, GameWon } gamestatus;

	long long int PREVIOUS_TIME, LAG;  // Manage consistent game speed for all platforms and during all times

	PerformanceStats performancestats; // These are used only for debugging
	
	
	std::vector<std::string> mapfilenames;
	int numberofmaps;
	int cursor;
	int listcursor;
	int timertocursormove;
	
	/********************************/
};



void kprint(const std::string & str, const Vei2 & pos, const sf::Font & font, const sf::Color & color, sf::RenderWindow& wnd);

