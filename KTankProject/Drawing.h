#pragma once

#include <SFML\Graphics.hpp>
#include "Vec2.h"
#include "Rect.h"

void DrawSpriteChromaCenteredRotated(sf::RenderWindow& wnd, const Vei2 & pos, const sf::Texture & bitmap, const RectI & srcRect, int dir = 1);

void DrawSpriteChromaCentered(sf::RenderWindow& wnd, const Vei2 & pos, const sf::Texture & bitmap, const RectI & srcRect, int dir = 1);

