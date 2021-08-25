#pragma once
#include <SFML\Graphics.hpp>
#include "MapObject.h"
#include "Tank.h"
#include <string>


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
	//Font printer;
	sf::Font consolasfont;
	/********************************/
	/*  User Variables              */
	//Tank mytank;
	TanksContainer tanks;
	MapObjectsContainer mapobjects;
	BonusesContainer bonuses;
	RocketsContainer rockets;
	VisualEffectsContainer effects;
	MapInterpretation mapinterpretation;
	Eagle* eagle;
	Tank* tank1;
	Tank* tank2;
	//Surface tankbitmap;
	long long int PREVIOUS_TIME, LAG;
	static constexpr long long int MS_PER_UPDATE = 20;
	static constexpr int MAX_ENEMY_TANKS = 5;
	static constexpr int MAX_ENEMY_TANKS_TOTAL = 30;
	static constexpr int TANKGENERATIONINTERVAL = 300;
	static constexpr int BONUSGENERATIONINTERVAL = 600;
	static constexpr int BONUSLIFEDURATION = 800;
	static constexpr int FORTDURATION = 500;
	static constexpr int generationsequence[30] = { 0,0,0,1,0, 0,1,0,0,2, 1,0,1,2,0, 0,1,2,1,2, 0,1,3,0,2, 3,0,1,3,3 };
	static constexpr int bonusgenerationsequence[5] = { 1,2,3,4,5 };
	int tankgenerationcountdown;
	int bonusgenerationcountdown;
	int bonusgenerationnumber;
	int lifesleft;
	int tankgenerationnumber;
	int fortresstimer;
	bool fortress;
	int checkmaploop;
	int rerouteloop;
	int testreadchars;
	int MODELUPDATESPERFRAMEUPDATE, MSTODRAW, MSPERMODELUPDATE;
	bool _soilon;
	enum GameStatus { MainMenu, GameInProgress, GameOver, GameWon } gamestatus;
	std::vector<std::string> mapfilenames;
	int numberofmaps;
	int cursor;
	int listcursor;
	int timertocursormove;
	int endgametimer;
	/********************************/
};



void kprint(const std::string & str, const Vei2 & pos, const sf::Font & font, const sf::Color & color, sf::RenderWindow& wnd);

