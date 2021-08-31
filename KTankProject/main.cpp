#include <SFML\Graphics.hpp>

#include "Game.h"
#include "Sounds.h"
#include <iostream>

int main()
{
	std::cout << " *** K-TANK GAME LOADING ***" << std::endl;
	std::cout << "DEV Version 20210826 by Konstantin Kalinchenko" << std::endl;
	sf::RenderWindow window(sf::VideoMode(800, 600), "K-Tank!", sf::Style::Fullscreen);
	Game game(window);
	std::cout << "Loading graphics..." << std::endl;
	if (!TanksContainer::basictankbitmap.loadFromFile("graphics\\BasicTank.png")) { std::cout << "ERROR Cannot load BasicTank.png" << std::endl; };
	if (!TanksContainer::bluetankbitmap.loadFromFile("graphics\\BlueTank.png")) { std::cout << "ERROR Cannot load BlueTank.png" << std::endl; };
	if (!TanksContainer::redtankbitmap.loadFromFile("graphics\\RedTank.png")) { std::cout << "ERROR Cannot load RedTank.png" << std::endl; };
	if (!TanksContainer::greentankbitmap.loadFromFile("graphics\\GreenTank.png")) { std::cout << "ERROR Cannot load GreenTank.png" << std::endl; };
	if (!Rocket::rocketsbitmap.loadFromFile("graphics\\rockets.png")) { std::cout << "ERROR Cannot load rockets.png" << std::endl; };
	if (!MapObjectsContainer::mapobjectsbitmap.loadFromFile("graphics\\mapobjects.png")) { std::cout << "ERROR Cannot load mapobjects.png" << std::endl; };
	if (!Water::waterbitmap.loadFromFile("graphics\\DynamicWater.png")) { std::cout << "ERROR Cannot load DynamicWater.png" << std::endl; };
	if (!BonusesContainer::bonusesbitmap.loadFromFile("graphics\\Bonuses.png")) { std::cout << "ERROR Cannot load Bonuses.png" << std::endl; };
	if (!VisualEffect::bigexplosionbitmap.loadFromFile("graphics\\BigExplosion.png")) { std::cout << "ERROR Cannot load BigExplosion.png" << std::endl; };
	if (!VisualEffect::smallexplosionbitmap.loadFromFile("graphics\\SmallExplosion.png")) { std::cout << "ERROR Cannot load SmallExplosion.png" << std::endl; };

	std::cout << "Loading sounds..." << std::endl;
	if (!Sounds::bigexplosionbuffer.loadFromFile("sounds\\BigExplosion.wav")) { std::cout << "Cannot load BigExplosion.wav" << std::endl; };
	Sounds::bigexplosion.setBuffer(Sounds::bigexplosionbuffer);
	Sounds::bigexplosion.setVolume(15.f);
	if (!Sounds::shotbuffer.loadFromFile("sounds\\SHOT.wav")) { std::cout << "Cannot load Shot.wav" << std::endl; };
	Sounds::shot.setBuffer(Sounds::shotbuffer);
	Sounds::shot.setVolume(15.f);
	if (!Sounds::boombuffer.loadFromFile("sounds\\boom2.wav")) { std::cout << "Cannot load boom2.wav" << std::endl; };
	Sounds::boom.setBuffer(Sounds::boombuffer);
	Sounds::boom.setVolume(15.f);

	while (window.isOpen())
	{
		game.Go();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::Closed) | (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
			{
				window.close();
			}

		}

	}

	return 0;
}