#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <Windows.h>
#include "AllSound.h"

using namespace sf;
using namespace std;

Uint32 inputbuffer = 0;

RenderWindow window;

RectangleShape RectAnimThing(float value, RectangleShape& origrect)
{
	RectangleShape rect;
	rect.setFillColor(Color(Uint8(100 * (0.5 + value * 0.5)), Uint8(115 * (0.5 + value * 0.5)), Uint8(127 * (0.5 + value * 0.5))));
	rect.setOutlineThickness(-5); rect.setOutlineColor(Color(50, 57, 63));
	rect.setSize(Vector2f(
		origrect.getGlobalBounds().width * value, origrect.getGlobalBounds().height));
	rect.setPosition(
		float(origrect.getGlobalBounds().left + ((1.0 - value) / 2) * origrect.getGlobalBounds().width),
		float(origrect.getGlobalBounds().top));
	return rect;
}

class Button
{
public:
	Clock cooldown;
	Font font;
	Text text;
	Color textclr, bgclr;
	RectangleShape rectshape;
	Vector2i magic = Vector2i(0, 0);
	int cooldowntime = 200;
	bool mousehover = false;
	String thetext;
	Button() {}
	void Create(string fontfile, String _text, Color textcolor, Color backgroundcolor, FloatRect _rect, int size, int _cooldowntime = 200)
	{
		font.loadFromFile(fontfile);
		text.setFont(font);
		text.setFillColor(textcolor); 
		text.setStyle(sf::Text::Bold);
		text.setOutlineThickness(3);
		text.setOutlineColor(Color(textcolor.r / 2, textcolor.g / 2, textcolor.b / 2));
		text.setCharacterSize(size);
		thetext = _text;
		text.setString(thetext);
		textclr = textcolor;
		bgclr = backgroundcolor;
		cooldowntime = _cooldowntime;
		rectshape.setPosition(_rect.left, _rect.top);
		rectshape.setSize(Vector2f(_rect.width, _rect.height));
		rectshape.setFillColor(bgclr);
		rectshape.setOutlineThickness(5);
		rectshape.setOutlineColor(Color(bgclr.r / 2, bgclr.g / 2, bgclr.b / 2));
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2),
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2));
		magic.x = text.getGlobalBounds().left + text.getGlobalBounds().width / 2 - (rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2);
		magic.y = text.getGlobalBounds().top + text.getGlobalBounds().height / 2 - (rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2) - magic.x,
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2) - magic.y);
	}
	bool Update()
	{
		if (rectshape.getGlobalBounds().contains(
			window.mapPixelToCoords(Mouse::getPosition(window)).x,
			window.mapPixelToCoords(Mouse::getPosition(window)).y))
		{
			mousehover = true;
			rectshape.setFillColor(Color(min(bgclr.r + 40, 255), min(bgclr.g + 40, 255), min(bgclr.b + 40, 255)));
		}
		else
		{
			mousehover = false;
			rectshape.setFillColor(bgclr);
		}
		if (cooldown.getElapsedTime().asMilliseconds() > cooldowntime && window.hasFocus())
		{
			if (rectshape.getGlobalBounds().contains(
				window.mapPixelToCoords(Mouse::getPosition(window)).x,
				window.mapPixelToCoords(Mouse::getPosition(window)).y))
				if (Mouse::isButtonPressed(Mouse::Button::Left))
				{
					rectshape.setFillColor(Color(max(bgclr.r - 40, 0), max(bgclr.r - 40, 0), max(bgclr.r - 40, 0)));
					cooldown.restart(); 
					return true;
					
				}
		}
		return false;
	}
	void Draw(bool showtext = true)
	{
		window.draw(rectshape);
		while (text.getGlobalBounds().width >= rectshape.getGlobalBounds().width * 0.97)
			text.setScale(text.getScale().x - 0.01, text.getScale().y - 0.01);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2) - magic.x,
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2) - magic.y);
		if (showtext) window.draw(text);
	}
	void SetText(String str)
	{
		thetext = str;
		text.setString(thetext); text.setScale(1, 1);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2),
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2));
		magic.x = text.getGlobalBounds().left + text.getGlobalBounds().width / 2 - (rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2);
		magic.y = text.getGlobalBounds().top + text.getGlobalBounds().height / 2 - (rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2) - magic.x,
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2) - magic.y);
	}
	void LightUp()
	{
		int need = abs((cooldown.getElapsedTime().asMilliseconds() / 25) % 40 - 20) + 50;
		rectshape.setFillColor(Color(
			min(bgclr.r + need, 255),
			min(bgclr.g + need, 255),
			min(bgclr.b + need, 255)));
	}
	void LightDown()
	{
		int need = abs((cooldown.getElapsedTime().asMilliseconds() / 25) % 20 - 10) - 40;
		rectshape.setFillColor(Color(
			min(bgclr.r + need, 255),
			min(bgclr.g + need, 255),
			min(bgclr.b + need, 255)));
	}
};

class Trackbar
{
public:
	Font font;
	Text text;
	Color textclr, bgclr;
	RectangleShape rectshape, trackrectshape;
	Vector2i magic = Vector2i(0, 0);
	String thetext;
	int value = 10;
	Trackbar() {}
	void Create(string fontfile, String _text, Color textcolor, Color backgroundcolor, FloatRect _rect, int size)
	{
		font.loadFromFile(fontfile);
		text.setFont(font);
		text.setFillColor(textcolor);
		text.setStyle(sf::Text::Bold);
		text.setOutlineThickness(3);
		text.setOutlineColor(Color(textcolor.r / 2, textcolor.g / 2, textcolor.b / 2));
		text.setCharacterSize(size);
		thetext = _text;
		text.setString(thetext);
		textclr = textcolor;
		bgclr = backgroundcolor;
		rectshape.setPosition(_rect.left, _rect.top);
		rectshape.setSize(Vector2f(_rect.width, _rect.height));
		rectshape.setFillColor(bgclr);
		rectshape.setOutlineThickness(5);
		rectshape.setOutlineColor(Color(bgclr.r / 2, bgclr.g / 2, bgclr.b / 2));
		text.setPosition(
			rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2,
			rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2);
		trackrectshape.setPosition(_rect.left + (_rect.width / 20) * value, _rect.top);
		trackrectshape.setSize(Vector2f(_rect.width / 20, _rect.height));
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2),
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2));
		magic.x = text.getGlobalBounds().left + text.getGlobalBounds().width / 2 - (rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2);
		magic.y = text.getGlobalBounds().top + text.getGlobalBounds().height / 2 - (rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2) - magic.x,
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2) - magic.y);
	}
	int Update()
	{
		if (rectshape.getGlobalBounds().contains(
			window.mapPixelToCoords(Mouse::getPosition(window)).x,
			window.mapPixelToCoords(Mouse::getPosition(window)).y))
		{
			rectshape.setFillColor(Color(min(bgclr.r + 40, 255), min(bgclr.g + 40, 255), min(bgclr.b + 40, 255)));
			if (Mouse::isButtonPressed(Mouse::Button::Left))
			{
				rectshape.setFillColor(Color(max(bgclr.r - 40, 0), max(bgclr.r - 40, 0), max(bgclr.r - 40, 0)));
				int valuebuf = int((window.mapPixelToCoords(Mouse::getPosition(window)).x - rectshape.getPosition().x) / trackrectshape.getGlobalBounds().width);
				if (valuebuf >= 0 && valuebuf < 20)
				{
					value = valuebuf;
					trackrectshape.setPosition(rectshape.getGlobalBounds().left + 5
						+ ((rectshape.getGlobalBounds().width - 10) / 20) * value,
						rectshape.getGlobalBounds().top + 5);
					return value;
				}
			}
		}
		else
			rectshape.setFillColor(bgclr);
		return value;
	}
	void Draw()
	{
		window.draw(rectshape);
		window.draw(trackrectshape);
		window.draw(text);
	}
	void SetText(String str)
	{
		thetext = str;
		text.setString(thetext);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2),
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2));
		magic.x = text.getGlobalBounds().left + text.getGlobalBounds().width / 2 - (rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2);
		magic.y = text.getGlobalBounds().top + text.getGlobalBounds().height / 2 - (rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2) - magic.x,
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2) - magic.y);
	}
};

class Label
{
public:
	Font font;
	Text text;
	Color textclr;
	RectangleShape rectshape;
	Clock clock;
	Vector2i magic = Vector2i(0, 0);
	String thetext;
	bool userect, downorient = false;
	Label() {}
	void Create(string fontfile, String _text, Color textcolor, Color backgroundcolor, 
		FloatRect _rect, int size, bool _userect = true)
	{
		font.loadFromFile(fontfile);
		text.setFont(font);
		text.setFillColor(textcolor);
		text.setStyle(sf::Text::Bold);
		text.setOutlineThickness(3);
		text.setOutlineColor(Color(textcolor.r / 2, textcolor.g / 2, textcolor.b / 2));
		text.setCharacterSize(size);
		thetext = _text;
		text.setString(thetext);
		textclr = textcolor;
		text.setPosition(_rect.left + _rect.width / 2, _rect.top + _rect.height / 2);
		rectshape.setPosition(_rect.left, _rect.top);
		rectshape.setSize(Vector2f(_rect.width, _rect.height));
		rectshape.setFillColor(backgroundcolor);
		rectshape.setOutlineThickness(5);
		rectshape.setOutlineColor(Color(
			backgroundcolor.r / 2, 
			backgroundcolor.g / 2,
			backgroundcolor.b / 2));
		userect = _userect;
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2),
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2));
		magic.x = text.getGlobalBounds().left + text.getGlobalBounds().width / 2 - (rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2);
		magic.y = text.getGlobalBounds().top + text.getGlobalBounds().height / 2 - (rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2) - magic.x,
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2) - magic.y);
	}
	void Draw()
	{
		if (userect) window.draw(rectshape);
		text.setFillColor(Color(
			min(textclr.r + abs(int(clock.getElapsedTime().asMilliseconds() * 0.01f) % 20 - 10), 255),
			min(textclr.g + abs(int(clock.getElapsedTime().asMilliseconds() * 0.01f) % 20 - 10), 255),
			min(textclr.b + abs(int(clock.getElapsedTime().asMilliseconds() * 0.01f) % 20 - 10), 255)
		));
		window.draw(text);
	}
	void SetText(String str)
	{
		thetext = str;
		text.setString(thetext);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2),
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2));
		magic.x = text.getGlobalBounds().left + text.getGlobalBounds().width / 2 - (rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2);
		magic.y = text.getGlobalBounds().top + text.getGlobalBounds().height / 2 - (rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2) - magic.x,
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2) - magic.y);
	}
};

class Plate
{
public:
	RectangleShape rectshape;
	Color bgclr;
	Plate() {}
	void Create(Color backgroundcolor, FloatRect _rect)
	{
		bgclr = backgroundcolor;
		rectshape.setPosition(_rect.left, _rect.top);
		rectshape.setSize(Vector2f(_rect.width, _rect.height));
		rectshape.setFillColor(backgroundcolor);
		rectshape.setOutlineThickness(5);
		rectshape.setOutlineColor(Color(
			backgroundcolor.r / 2,
			backgroundcolor.g / 2,
			backgroundcolor.b / 2));
	}
	void Draw()
	{
		rectshape.setFillColor(bgclr);
		rectshape.setOutlineColor(Color(
			bgclr.r / 2,
			bgclr.g / 2,
			bgclr.b / 2));
		window.draw(rectshape);
	}
};

class TextBox
{
public:
	Clock cooldown, cursorblink;
	Font font;
	Text text;
	Color textclr, bgclr;
	RectangleShape rectshape, cursorrect;
	String textinside;
	Vector2i magic = Vector2i(0, 0);
	int cooldowntime = 200, cursor = 0, limit = 0;
	bool mousehover = false, active = true;
	TextBox() {}
	void Create(string fontfile, Color textcolor, Color backgroundcolor, FloatRect _rect, int size, int _limit, int _cooldowntime = 200)
	{
		limit = _limit;
		font.loadFromFile(fontfile);
		text.setFont(font);
		text.setFillColor(textcolor);
		text.setStyle(sf::Text::Bold);
		text.setOutlineThickness(3);
		text.setOutlineColor(Color(textcolor.r / 2, textcolor.g / 2, textcolor.b / 2));
		text.setCharacterSize(size);
		text.setString("123");
		textclr = textcolor;
		bgclr = backgroundcolor;
		cooldowntime = _cooldowntime;
		rectshape.setPosition(_rect.left, _rect.top);
		rectshape.setSize(Vector2f(_rect.width, _rect.height));
		rectshape.setFillColor(bgclr);
		rectshape.setOutlineThickness(5);
		rectshape.setOutlineColor(Color(bgclr.r / 2, bgclr.g / 2, bgclr.b / 2));
		cursorrect.setSize(Vector2f(4, _rect.height * 0.6));
		cursorrect.setFillColor(Color(bgclr.r / 2, bgclr.g / 2, bgclr.b / 2));
		cursorrect.setOrigin(2, _rect.height * 0.3);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2),
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2));
		magic.x = text.getGlobalBounds().left + text.getGlobalBounds().width / 2 - (rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2);
		magic.y = text.getGlobalBounds().top + text.getGlobalBounds().height / 2 - (rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2);
	}
	bool Update(String* targetstr)
	{
		if (active) rectshape.setFillColor(Color(min(bgclr.r + 40, 255), min(bgclr.g + 40, 255), min(bgclr.b + 40, 255)));
		else rectshape.setFillColor(bgclr);
		if (rectshape.getGlobalBounds().contains(
			window.mapPixelToCoords(Mouse::getPosition(window)).x,
			window.mapPixelToCoords(Mouse::getPosition(window)).y))
		{
			//if (!mousehover && window.hasFocus()) sounds[0].play(); 
			rectshape.setFillColor(Color(
				min(rectshape.getFillColor().r + 20, 255),
				min(rectshape.getFillColor().g + 20, 255),
				min(rectshape.getFillColor().b + 20, 255)));
			mousehover = true;
		}
		else
		{
			mousehover = false;
			//rectshape.setFillColor(bgclr);
		}
		if (cooldown.getElapsedTime().asMilliseconds() > cooldowntime && window.hasFocus())
		{
			if (rectshape.getGlobalBounds().contains(
				window.mapPixelToCoords(Mouse::getPosition(window)).x,
				window.mapPixelToCoords(Mouse::getPosition(window)).y))
				if (Mouse::isButtonPressed(Mouse::Button::Left))
				{
					rectshape.setFillColor(Color(max(bgclr.r - 40, 0), max(bgclr.r - 40, 0), max(bgclr.r - 40, 0)));
					cooldown.restart();
					active = true;
					inputbuffer = 0;
					cursorblink.restart();
				}
		}
		if (!rectshape.getGlobalBounds().contains(
			window.mapPixelToCoords(Mouse::getPosition(window)).x,
			window.mapPixelToCoords(Mouse::getPosition(window)).y))
			if (Mouse::isButtonPressed(Mouse::Button::Left)) active = false;
		if (active)
		{
			if (cooldown.getElapsedTime().asMilliseconds() > cooldowntime)
			{
				if (Keyboard::isKeyPressed(Keyboard::Left) && cursor > 0)
				{
					cursor--; cooldown.restart();
				}
				if (Keyboard::isKeyPressed(Keyboard::Right) && cursor < textinside.getSize())
				{
					cursor++; cooldown.restart();
				}
			}
			if (inputbuffer != 0)
			{
				if (inputbuffer == 8)
				{
					if (textinside.getSize() > cursor - 1)
					{
						textinside.erase(cursor - 1);
						cursor--;
					}
				}
				else
					if (inputbuffer == 13)
					{
						if (textinside.getSize() > 0 && cooldown.getElapsedTime().asMilliseconds() > cooldowntime)
						{
							(*targetstr) = textinside; 
							inputbuffer = 0;
							textinside = "";
							cursor = 0;
							return true;
						}
					}
					else
					{
						if (textinside.getSize() < limit)
						{
							textinside.insert(cursor, inputbuffer);
							cursor++;
						}
					}
				inputbuffer = 0;
			}
		}
		return false;
	}
	void Draw()
	{
		window.draw(rectshape);
		text.setScale(1, 1);
		text.setString(textinside);
		while (text.getGlobalBounds().width >= 570.0f)
			text.setScale(text.getScale().x - 0.01, text.getScale().y - 0.01);
		text.setPosition(
			(rectshape.getGlobalBounds().left + rectshape.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2) - magic.x,
			(rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2) - magic.y);
		window.draw(text);
		if (cursorblink.getElapsedTime().asMilliseconds() > 750 && active)
		{
			if (cursorblink.getElapsedTime().asMilliseconds() > 1500) cursorblink.restart();
			int bufx = text.getGlobalBounds().left;
			text.setString(textinside.substring(0, cursor));
			cursorrect.setPosition(
				bufx + text.getGlobalBounds().width - magic.x,
				rectshape.getGlobalBounds().top + rectshape.getGlobalBounds().height / 2);
			window.draw(cursorrect);
		}
	}
};

class ButtonList
{
public:
	vector<Button> buttons;
	Button listbutton;
	int opened = -1;
	int openanimvall = 0;
	Clock openclk;
	String lbstropen, lbstrback;
	ButtonList() {}
	void Create(int qofbuttons, int _openanimvall)
	{
		openanimvall = _openanimvall;
		for (int i = 0; i < qofbuttons; i++) buttons.push_back(Button());
	}
	int Update()
	{
		if (listbutton.Update() &&
			openclk.getElapsedTime().asMilliseconds() > openanimvall * buttons.size())
		{
			if (opened == -1) opened = 1;
			else opened = 1 - opened;
			openclk.restart();
			listbutton.SetText(lbstrback);
			if (opened == 1) listbutton.SetText(lbstrback);
			else listbutton.SetText(lbstropen);
		}
		if (opened == 1 && openclk.getElapsedTime().asMilliseconds() > openanimvall * buttons.size())
			for (int i = 0; i < buttons.size(); i++)
				if (buttons[i].Update()) 
				{
					opened = 0; listbutton.SetText(lbstropen); 
					openclk.restart(); buttons[i].Update(); return i;
				}
		return -1;
	}
	void Draw()
	{
		listbutton.Draw();
		if (opened != -1)
		{
			if (opened == 0 && openclk.getElapsedTime().asMilliseconds() < openanimvall * buttons.size() + openanimvall)
			{
				for (int i = 0; i < buttons.size() -
					int(openclk.getElapsedTime().asMilliseconds() / openanimvall); i++)
				{
					buttons[i].Draw();
				}
				window.draw(RectAnimThing(
					float(openanimvall - openclk.getElapsedTime().asMilliseconds() % openanimvall) / float(openanimvall),
					buttons[buttons.size() - int(openclk.getElapsedTime().asMilliseconds() / openanimvall)].rectshape));
			}
			if (opened == 1)
			{
				if (openclk.getElapsedTime().asMilliseconds() > openanimvall * buttons.size())
				{
					for (int i = 0; i < buttons.size(); i++)
					{
						buttons[i].Draw();
					}
				}
				else
				{
					for (int i = 0; i <
						min(int(openclk.getElapsedTime().asMilliseconds() / openanimvall), int(buttons.size())); i++)
					{
						buttons[i].Draw();
					}
					window.draw(RectAnimThing(float(openclk.getElapsedTime().asMilliseconds() % openanimvall) / float(openanimvall),
						buttons[int(openclk.getElapsedTime().asMilliseconds() / openanimvall)].rectshape));
				}
			}
		}
	}
	void PreCreateAll(string fontfile, Color textcolor, Color backgroundcolor, FloatRect _rect, int size, bool up, int modif, int _cooldowntime = 200)
	{
		//cout << int(int(up) * 2) << " " << (int(int(up) * 2) - 1) << endl;
		listbutton.Create(fontfile, "", textcolor, backgroundcolor,
			FloatRect(_rect.left, _rect.top,
				_rect.width, _rect.height), size, _cooldowntime);
		for (int i = 0; i < buttons.size(); i++)
			buttons[i].Create(fontfile, "", textcolor, backgroundcolor, 
				FloatRect(_rect.left, _rect.top + (_rect.height + modif) * (i + 1) * (int(int(up) * 2) - 1),
					_rect.width, _rect.height), size, _cooldowntime);
	}
};

enum class ProgMode { EnterName, Messaging, SendFile, WantToTakeFile };

struct Contact
{
	IpAddress ip;
	String name;
	Contact(IpAddress _ip, String _name) { ip = _ip; name = _name; }
};

class SPRITE
{
public:
	Image img;
	Texture txtr;
	Sprite spr;
	SPRITE() { }
	Vector2i GetSize()
	{
		return Vector2i(img.getSize().x, img.getSize().y);
	}
	void Reload()
	{
		txtr.loadFromImage(img);
		spr.setTexture(txtr);
	}
	void Create(Vector2i size, Color clr = Color(255, 255, 255, 255))
	{
		img.create(size.x, size.y, clr);
		this->Reload();
	}
	void Load(string file)
	{
		img.loadFromFile(file);
		this->Reload();
	}
};