#pragma once
#include "Global.h"

void EnterNameDraw()
{
	window.clear(bgcolor);
	if (true)
	{
		float magicv = float(abs(max(0, startanimclk.getElapsedTime().asMilliseconds()
			+ startanimval * int(startanimcostil == 0)) - startanimval)) / float(startanimval);
		if (startanimcostil == 1 && startanimclk.getElapsedTime().asMilliseconds() > startanimval)
			progmode = ProgMode::Messaging;
		if (magicv < 0.99)
		{
			DrawAnimThing(magicv, nameinput.rectshape);
			DrawAnimThing(magicv, textsizetb.rectshape);
			DrawAnimThing(magicv, bright.rectshape);
		}
		else
		{
			nameinput.Draw();
			entername.Draw();
			textsizetb.Draw();
			bright.Draw();
		}
	}
	window.display();
}

void MessagingDraw()
{
	window.clear(bgcolor);
	for (int i = 0; i < min(5, int(contacts.size() - contactspage * 5)); i++)
	{
		float magicv = float(abs(max(0, switchpageclk.getElapsedTime().asMilliseconds() -
			switchpageanimval1 * (i % 5)) - switchpageanimval)) / float(switchpageanimval);
		if (magicv < 0.99)
		{
			DrawAnimThing(magicv, contactsbuttons[i].rectshape);
		}
		else
		{
			float magicv1 = float(abs(max(0, startanimclk.getElapsedTime().asMilliseconds()
				+ startanimval * int(startanimcostil == 0)) - startanimval)) / float(startanimval);
			if (magicv1 < 0.99)
			{
				DrawAnimThing(magicv1, contactsbuttons[i].rectshape);
			}
			else
			{
				bool somebodytexted = false;
				for (int u = dialogs[i + contactspage * 5].size() - 1; u >= 0; u--)
				{
					if (dialogs[i + contactspage * 5][u].sender != name)
					{
						if (!dialogs[i + contactspage * 5][u].checked)
						{
							somebodytexted = true; break;
						}
						else
							break;
					}
				}
				if (somebodytexted) contactsbuttons[i].LightUp();
				if (choosedcontacttotalk == i + contactspage * 5) contactsbuttons[i].LightDown();
				contactsbuttons[i].Draw((switchpageclk.getElapsedTime().asMilliseconds() -
					switchpageanimval1 * min(int(i), 5) > switchpageanimval * 2));
			}
		}
	}
	if (contactspage == 0)
	{
		leftar.rectshape.setFillColor(Color(Uint8(leftar.bgclr.r * 0.75),
			Uint8(leftar.bgclr.g * 0.75), Uint8(leftar.bgclr.b * 0.75)));
		leftar.text.setFillColor(Color(Uint8(leftar.textclr.r * 0.75),
			Uint8(leftar.textclr.g * 0.75), Uint8(leftar.textclr.b * 0.75)));
	}
	else
		leftar.text.setFillColor(leftar.textclr);
	if ((contactspage + 1) * 5 >= qofcontacts)
	{
		rightar.rectshape.setFillColor(Color(Uint8(rightar.bgclr.r * 0.75),
			Uint8(rightar.bgclr.g * 0.75), Uint8(rightar.bgclr.b * 0.75)));
		rightar.text.setFillColor(Color(Uint8(rightar.textclr.r * 0.75),
			Uint8(rightar.textclr.g * 0.75), Uint8(rightar.textclr.b * 0.75)));
	}
	else
		rightar.text.setFillColor(rightar.textclr);
	if (true)
	{
		float magicv = float(abs(max(0, startanimclk.getElapsedTime().asMilliseconds()
			+ startanimval * int(startanimcostil == 0)) - startanimval)) / float(startanimval);
		if (startanimcostil == 2 && startanimclk.getElapsedTime().asMilliseconds() > startanimval)
			progmode = ProgMode::SendFile;
		if (startanimcostil == 4 && startanimclk.getElapsedTime().asMilliseconds() > startanimval)
			progmode = ProgMode::EnterName;
		if (magicv < 0.99)
		{
			DrawAnimThing(magicv, leftar.rectshape);
			DrawAnimThing(magicv, rightar.rectshape);
			DrawAnimThing(magicv, globfunctions.listbutton.rectshape);
			//DrawAnimThing(magicv, exitbutton.rectshape);
			DrawAnimThing(magicv, namelabel.rectshape);
		}
		else
		{
			leftar.Draw();
			rightar.Draw();
			globfunctions.Draw();
			//exitbutton.Draw();
			namelabel.Draw();
		}
	}
	 
	if (choosedcontacttotalk != -1 || (choosedcontacttotalk == -1 && dialoganimclk.getElapsedTime().asMilliseconds() <= dialoganimval1))
	{
		float magicv = float(abs(max(0, dialoganimclk.getElapsedTime().asMilliseconds() +
			dialoganimval1 * int(lastchoosedcontacttotalk == -1)) - dialoganimval1)) / float(dialoganimval1);
		if (magicv < 0.99)
		{
			DrawAnimThing(magicv, dialogplate.rectshape);
			DrawAnimThing(magicv, dialoginput.rectshape);
			DrawAnimThing(magicv, dialogname.rectshape);
			DrawAnimThing(magicv, dialogfunctions.listbutton.rectshape);
			//DrawAnimThing(magicv, sendbutton.rectshape);
			DrawAnimThing(magicv, closebutton.rectshape);
		}
		else
		{
			float magicv1 = float(abs(max(0, startanimclk.getElapsedTime().asMilliseconds()
				+ startanimval * int(startanimcostil == 0)) - startanimval)) / float(startanimval);
			if (magicv1 < 0.99)
			{
				DrawAnimThing(magicv1, dialogplate.rectshape);
				DrawAnimThing(magicv1, dialoginput.rectshape);
				DrawAnimThing(magicv1, dialogname.rectshape);
				DrawAnimThing(magicv1, dialogfunctions.listbutton.rectshape);
				//DrawAnimThing(magicv1, sendbutton.rectshape);
				DrawAnimThing(magicv1, closebutton.rectshape);
			}
			else
			{
				bool showit = false;
				for (int i = 0; i < streamdesktowho.size(); i++)
					if (streamdesktowho[i] == choosedcontacttotalk) showit = true;
				if (showit || receivestream)
				{
					streamanimvals[0] += streamanimclock.getElapsedTime().
						asMilliseconds() * (2 * int(streamanim[0]) - 1);
					streamanimvals[1] += streamanimclock.getElapsedTime().
						asMilliseconds() * (2 * int(streamanim[1]) - 1);
					streamanimvals[0] = min(max(0, streamanimvals[0]), 500);
					streamanimvals[1] = min(max(0, streamanimvals[1]), 500);
					streaminspr.spr.setPosition(365.0f, 65.0f);
					streaminspr.spr.setScale(
						0.5f + streamanimvals[0] * 0.001f,
						0.5f + streamanimvals[0] * 0.001f);
					streamoutspr.spr.setPosition(365.0f + 580.0f, 65.0f);
					streamoutspr.spr.setOrigin(580, 0);
					streamoutspr.spr.setScale(
						0.5f + streamanimvals[1] * 0.001f,
						0.5f + streamanimvals[1] * 0.001f);
					if (streaminspr.spr.getGlobalBounds().contains(
						(window).mapPixelToCoords(Mouse::getPosition((window))).x,
						(window).mapPixelToCoords(Mouse::getPosition((window))).y) && !streamanim[1])
						streamanim[0] = true; else streamanim[0] = false;
					if (streamoutspr.spr.getGlobalBounds().contains(
						(window).mapPixelToCoords(Mouse::getPosition((window))).x,
						(window).mapPixelToCoords(Mouse::getPosition((window))).y) && !streamanim[0])
						streamanim[1] = true; else streamanim[1] = false;
				}
				streamanimclock.restart();
				dialogplate.Draw();
				dialoginput.Draw();
				dialogname.Draw();
				closebutton.Draw();
				if (receivestream && !showit) window.draw(streaminspr.spr);
				if (showit && !receivestream) window.draw(streamoutspr.spr);
				if (showit && receivestream)
				{
					if (streamanimvals[1] > streamanimvals[0])
					{
						window.draw(streaminspr.spr);
						window.draw(streamoutspr.spr);
					}
					else
					{
						window.draw(streamoutspr.spr);
						window.draw(streaminspr.spr);
					}
				}
				dialogfunctions.Draw();
				Text dialogelement;
				dialogelement.setFont(mainfont);
				dialogelement.setCharacterSize(14 + TEXTSIZE);
				dialogelement.setFillColor(Color(245, 245, 255));
				dialogelement.setOutlineThickness(3);
				dialogelement.setOutlineColor(Color(122, 122, 127));
				if (choosedcontacttotalk != -1)
				{
					int bufposition = 0, i = dialogs[choosedcontacttotalk].size() - 1;
					//for (int i = int(dialogs[choosedcontacttotalk].size()) - 1; i >= max(0, int(dialogs[choosedcontacttotalk].size()) - 1 - 40); i--)
					while (bufposition < 470.0f && i >= 0)
					{
						dialogs[choosedcontacttotalk][i].checked = true;
						dialogelement.setScale(1.0f, 1.0f);
						dialogelement.setString(dialogs[choosedcontacttotalk][i].date + L" " + dialogs[choosedcontacttotalk][i].str);
						while (dialogelement.getGlobalBounds().width >= 570.0f)
							dialogelement.setScale(dialogelement.getScale().x - 0.025, dialogelement.getScale().y - 0.025);
						dialogelement.setPosition(370.0f, 565.0f - bufposition - dialogelement.getGlobalBounds().height - 10.0f);
						bufposition += dialogelement.getGlobalBounds().height + 7.0f; window.draw(dialogelement); i--;
					}
				}
			}
		}
	}
	window.display();
	
}

void SendFileDraw()
{
	window.clear(bgcolor);
	for (int i = filespage * 12; i < min(filespage * 12 + 12, int(findedfiles.size())); i++)
	{
		float magicv = float(abs(max(0, filesanimclk.getElapsedTime().asMilliseconds() -
			filepageanimval1 * (i % 12)) - filepageanimval)) / float(filepageanimval);
		if (magicv < 0.99)
		{
			DrawAnimThing(magicv, filebuttons[i % 12].rectshape);
		}
		else
		{
			float magicv1 = float(abs(max(0, startanimclk.getElapsedTime().asMilliseconds()
				+ startanimval * int(startanimcostil == 0)) - startanimval)) / float(startanimval);
			if (startanimcostil == 3 && startanimclk.getElapsedTime().asMilliseconds() > startanimval)
				progmode = ProgMode::Messaging;
			if (magicv1 < 0.99)
			{
				DrawAnimThing(magicv1, filebuttons[i % 12].rectshape);
			}
			else
				filebuttons[i % 12].Draw(filesanimclk.getElapsedTime().asMilliseconds() -
					filepageanimval1 * (min(int(findedfiles.size() - (filespage * 12 + 12)), 12)) > filepageanimval * 2);
		}
	}
	if (filespage == 0)
	{
		leftarfiles.rectshape.setFillColor(Color(Uint8(leftarfiles.bgclr.r * 0.75),
			Uint8(leftarfiles.bgclr.g * 0.75), Uint8(leftarfiles.bgclr.b * 0.75)));
		leftarfiles.text.setFillColor(Color(Uint8(leftarfiles.textclr.r * 0.75),
			Uint8(leftarfiles.textclr.g * 0.75), Uint8(leftarfiles.textclr.b * 0.75)));
	}
	else
		leftar.text.setFillColor(leftar.textclr);
	if ((filespage + 1) * 12 >= findedfiles.size())
	{
		rightarfiles.rectshape.setFillColor(Color(Uint8(rightarfiles.bgclr.r * 0.75),
			Uint8(rightarfiles.bgclr.g * 0.75), Uint8(rightarfiles.bgclr.b * 0.75)));
		rightarfiles.text.setFillColor(Color(Uint8(rightarfiles.textclr.r * 0.75),
			Uint8(rightarfiles.textclr.g * 0.75), Uint8(rightarfiles.textclr.b * 0.75)));
	}
	else
		rightar.text.setFillColor(rightar.textclr);
	if (true)
	{
		float magicv1 = float(abs(max(0, startanimclk.getElapsedTime().asMilliseconds()
			+ startanimval * int(startanimcostil == 0)) - startanimval)) / float(startanimval);
		if (magicv1 < 0.99)
		{
			DrawAnimThing(magicv1, leftarfiles.rectshape);
			DrawAnimThing(magicv1, rightarfiles.rectshape);
			DrawAnimThing(magicv1, sendfilesback.rectshape);
		}
		else
		{
			leftarfiles.Draw();
			rightarfiles.Draw();
			sendfilesback.Draw();
		}
	}
	window.display();
}

void WantToTakeDraw()
{
	window.clear(bgcolor);
	takefile.Draw();
	takefilebut[0].Draw();
	takefilebut[1].Draw();
	window.display();
}