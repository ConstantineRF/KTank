#include "Drawing.h"

void DrawSpriteChromaCenteredRotated(sf::RenderWindow& wnd, const Vei2 & pos, const sf::Texture & bitmap, const RectI & srcRect, int dir)
{
	sf::Sprite sprite;
	sprite.setTexture(bitmap);
	const sf::IntRect rectangle(srcRect.left, srcRect.top, srcRect.GetWidth(), srcRect.GetHeight());
	sprite.setTextureRect(rectangle);
	if (dir == 1) {
		sprite.setPosition((float)pos.x - srcRect.GetWidth() / 2, (float)pos.y - srcRect.GetHeight() / 2);
	}
	if (dir == 2) {
		sprite.setRotation(270);
		sprite.setPosition((float)pos.x - srcRect.GetHeight() / 2, (float)pos.y + srcRect.GetWidth()/2);
	}
	else if (dir == 3) {
		sprite.setRotation(180);
		sprite.setPosition((float)pos.x - srcRect.GetWidth() / 2 + srcRect.GetWidth(), (float)pos.y - srcRect.GetHeight() / 2 + srcRect.GetHeight());
	}
	else if (dir == 4) {
		sprite.setRotation(90);
		sprite.setPosition((float)pos.x - srcRect.GetHeight() / 2 + srcRect.GetHeight(), (float)pos.y - srcRect.GetWidth() / 2);
	}
	wnd.draw(sprite);
}


void DrawSpriteChromaCentered(sf::RenderWindow& wnd, const Vei2 & pos, const sf::Texture & bitmap, const RectI & srcRect, int dir)
{
	DrawSpriteChromaCenteredRotated(wnd, pos, bitmap, srcRect, 1);
}
