#include <SFML\Graphics.hpp>
#include "Game.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!", sf::Style::Fullscreen);
	Game game(window);
	TanksContainer::basictankbitmap.loadFromFile("graphics\\BasicTank.png");
	TanksContainer::bluetankbitmap.loadFromFile("graphics\\BlueTank.png");
	TanksContainer::redtankbitmap.loadFromFile("graphics\\RedTank.png");
	TanksContainer::greentankbitmap.loadFromFile("graphics\\GreenTank.png");
	Rocket::rocketsbitmap.loadFromFile("graphics\\rockets.png");
	MapObjectsContainer::mapobjectsbitmap.loadFromFile("graphics\\mapobjects.png");
	Water::waterbitmap.loadFromFile("graphics\\DynamicWater.png");
	BonusesContainer::bonusesbitmap.loadFromFile("graphics\\Bonuses.png");
	VisualEffect::bigexplosionbitmap.loadFromFile("graphics\\BigExplosion.png");
	VisualEffect::smallexplosionbitmap.loadFromFile("graphics\\SmallExplosion.png");


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