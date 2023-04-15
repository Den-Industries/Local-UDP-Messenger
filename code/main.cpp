#include "Global.h"
#include "UpdateFunctions.h"
#include "DrawFunctions.h"

int main()
{
	Init();
	thread monitoringthread1(&MonitoringFunc1);
	thread monitoringthread2(&MonitoringFunc2);
	thread monitoringthread3(&MonitoringFunc3);
	thread monitoringthread4(&MonitoringFunc4);
	monitoringthread1.detach();
	monitoringthread2.detach();
	monitoringthread3.detach();
	monitoringthread4.detach();
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			if (event.type == sf::Event::TextEntered)
			{
				if (window.hasFocus()) inputbuffer = event.text.unicode;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape) && window.hasFocus()) window.close();
		if (updateclock.getElapsedTime().asMilliseconds() > 30)
		{
			updateclock.restart();
			switch (progmode)
			{
			case ProgMode::EnterName:
				EnterNameUpdate();
				break;
			case ProgMode::Messaging:
				MessagingUpdate();
				break;
			case ProgMode::SendFile:
				SendFileUpdate();
				break;
			case ProgMode::WantToTakeFile:
				WantToTakeFileUpdate();
				break;
			}
			switch (progmode)
			{
			case ProgMode::EnterName:
				EnterNameDraw();
				break;
			case ProgMode::Messaging:
				MessagingDraw();
				break;
			case ProgMode::SendFile:
				SendFileDraw();
				break;
			case ProgMode::WantToTakeFile:
				WantToTakeDraw();
				break;
			}
		}
	}
	working = false;
	DeleteObject(hbitmap);
	ReleaseDC(NULL, hdc);
	delete[] lpPixels;
	Sleep(200);
	return 0;
}