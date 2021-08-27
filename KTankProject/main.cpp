#include <SFML\Graphics.hpp>

#include "Game.h"
#include "Sounds.h"
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "K-Tank!"); // , sf::Style::Fullscreen);
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

	if (!Sounds::bigexplosionbuffer.loadFromFile("sounds\\BigExplosion.wav")) { std::cout << "Cannot load BigExplosion.wav" << std::endl; };
	Sounds::bigexplosion.setBuffer(Sounds::bigexplosionbuffer);
	if (!Sounds::shotbuffer.loadFromFile("sounds\\SHOT.wav")) { std::cout << "Cannot load Shot.wav" << std::endl; };
	Sounds::shot.setBuffer(Sounds::shotbuffer);
	if (!Sounds::boombuffer.loadFromFile("sounds\\boom2.wav")) { std::cout << "Cannot load boom2.wav" << std::endl; };
	Sounds::boom.setBuffer(Sounds::boombuffer);

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