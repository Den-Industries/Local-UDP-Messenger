#pragma once
#include "Global.h"

void EnterNameUpdate()
{
	if (nameinput.Update(&name))
	{
		namelabel.SetText(L"Ваше имя: " + name); //cout << "yes" << endl;
		startanimcostil = 1; startanimclk.restart();
		if (startflag)
			for (int i = 0; i < contacts.size(); i++)
			{
				wstring yes = L"@@changename" + name;
				Code(yes, IpAddress::getLocalAddress());
				Packet pp; pp << yes;
				socket4.send(pp, contacts[i].ip, unsigned short(2002));
			}
		startflag = true;
	}
	if (window.hasFocus())
	{
		int buf = 19 - bright.Update();
		bgcolor = Color(200 - buf * 10, 230 - buf * 10, 255 - buf * 10);
		TEXTSIZE = textsizetb.Update();
		bright.text.setCharacterSize(TEXTSIZE + 10);
		bright.SetText(bright.text.getString());
		textsizetb.text.setCharacterSize(TEXTSIZE + 10);
		textsizetb.SetText(textsizetb.text.getString());
		nameinput.text.setCharacterSize(30 + TEXTSIZE);
		entername.text.setCharacterSize(30 + TEXTSIZE);
		entername.SetText(entername.text.getString());
	}
}

void MessagingUpdate()
{
	if (dialoginput.Update(&sendbuffer))
	{
		if (sendbuffer == L"save_to_excel")
		{
			SaveToExcel();
			dialoginput.textinside = "";
			dialoginput.cursor = 0;
		}
		else
		{
			if (sendbuffer == L"save_all_to_excel")
			{
				SaveAllToExcel();
				dialoginput.textinside = "";
				dialoginput.cursor = 0;
			}
			else
			{
				dialoginput.textinside = "";
				dialoginput.cursor = 0;
				dialogs[choosedcontacttotalk].push_back(Message(L"Вы: " + sendbuffer, name));
				wstring yes = L"@@" + sendbuffer;
				Code(yes, IpAddress::getLocalAddress());
				Packet pp; pp << yes;
				socket4.send(pp, contacts[choosedcontacttotalk].ip, unsigned short(2002));
			}
		}
	}
	if (leftar.Update() && switchpageclk.getElapsedTime().asMilliseconds() -
		switchpageanimval1 * min(int(qofcontacts - (contactspage * 5 + 5)), 5) > switchpageanimval * 2)
		if (contactspage > 0)
		{
			contactspage--; switchpageclk.restart(); UpdateContactButtons();
		}
	if (rightar.Update() && switchpageclk.getElapsedTime().asMilliseconds() -
		switchpageanimval1 * min(int(qofcontacts - (contactspage * 5 + 5)), 5) > switchpageanimval * 2)
		if ((contactspage + 1) * 5 < qofcontacts)
		{
			contactspage++; switchpageclk.restart(); UpdateContactButtons();
		}
	for (int i = 0; i < 5; i++)
		if (float(abs(max(0, switchpageclk.getElapsedTime().asMilliseconds() -
			switchpageanimval1 * i) - switchpageanimval)) / float(switchpageanimval) >= 0.99)
			if (contactsbuttons[i].Update() && choosedcontacttotalk != i + contactspage * 5)
			{
				lastchoosedcontacttotalk = choosedcontacttotalk;
				dialoganimclk.restart();
				dialogname.SetText(contacts[i].name);
				//+ " " + contacts[i].ip.toString()
				choosedcontacttotalk = i + contactspage * 5;
				receivestream = false;
			}
	if (namelabel.Update())
	{
		nameinput.active = true;
		startanimcostil = 4; startanimclk.restart();
	}
	if (closebutton.Update())
	{
		lastchoosedcontacttotalk = choosedcontacttotalk;
		dialoganimclk.restart();
		choosedcontacttotalk = -1;
		dialogfunctions.opened = 0;
	}
	if (true)
	{
		int func = globfunctions.Update(); 
		if (func == 0)
		{
			SaveAllToExcel();
		}
		if (func == 1)
		{
			libxl::Book* book = xlCreateXMLBookW();
			if (book)
			{
				//String hah; TakeScreenShot(hah, Vector2i(580, 325));
				libxl::Sheet* sheet = book->addSheet(L"Stats");
				if (sheet)
				{
					sheet->writeStr(1, 0, L"Time");
					sheet->writeStr(1, 1, L"Filename");
					sheet->writeStr(1, 2, L"Size KBytes");
					sheet->writeStr(1, 3, L"Time to send ~ seconds");
					for (int i = 0; i < statistics.size(); i++)
					{
						sheet->writeStr(2 + i, 0, statistics[i].date.toWideString().c_str());
						sheet->writeStr(2 + i, 1, statistics[i].filename.toWideString().c_str());
						int buf = statistics[i].size / 512;
						wstringstream wss; wss << buf;
						sheet->writeStr(2 + i, 2, wss.str().c_str());
						wss.clear(); wss << (statistics[i].time * 0.001f);
						sheet->writeStr(2 + i, 3, wss.str().c_str());
					}
				}
				book->save(L"Files\\SendFileStatistics.xlsx");
				book->release();
			}
		}
		if (func == 2)
		{
			window.close();
		}
	}
	if (true)
	{
		int func = dialogfunctions.Update();
		if (func == 0 && !sendingfilestate)
		{
			startanimclk.restart(); startanimcostil = 2;
			findedfiles.clear();
			WIN32_FIND_DATAW wfd; HANDLE const hFind = FindFirstFileW(L"Share\\*", &wfd);
			if (INVALID_HANDLE_VALUE != hFind)
			{
				do findedfiles.push_back(&wfd.cFileName[0]);
				while (FindNextFileW(hFind, &wfd) != NULL);
				FindClose(hFind);
			}
			findedfiles.erase(findedfiles.begin()); findedfiles.erase(findedfiles.begin());
			for (int i = filespage * 12; i < min(filespage * 12 + 12, int(findedfiles.size())); i++) filebuttons[i].SetText(findedfiles[i]);
			sendfilesback.cooldown.restart();
		}
		if (func == 1)
		{
			SaveToExcel();
		}
		if (func == 2)
		{
			if (find(streamdesktowho.begin(), streamdesktowho.end(),
				choosedcontacttotalk) == streamdesktowho.end())
			{
				streamdesktowho.push_back(choosedcontacttotalk);
				streaminfo.push_back({ -1, 0 });
				dialogfunctions.buttons[2].SetText(L"Закончить трансляцию");
				wstring bufbuf = L"%#%"; //cout << "NIGGA" << endl;
				Packet pp; pp << bufbuf;
				socket6.send(pp, contacts[choosedcontacttotalk].ip, unsigned short(2004));
			}
			else
			{
				int yes = find(streamdesktowho.begin(), streamdesktowho.end(),
					choosedcontacttotalk) - streamdesktowho.begin();
				streamdesktowho.erase(yes + streamdesktowho.begin());
				streaminfo.erase(yes + streaminfo.begin());
				dialogfunctions.buttons[2].SetText(L"Трансляция экрана");
				wstring bufbuf = L"***"; //cout << "NIGGA" << endl;
				Packet pp; pp << bufbuf;
				socket6.send(pp, contacts[choosedcontacttotalk].ip, unsigned short(2004));
			}
		}
	}
}

void SendFileUpdate()
{
	if (sendfilesback.Update() && startanimcostil != 3)
	{
		startanimcostil = 3; filespage = 0; startanimclk.restart();
	}
	if (leftarfiles.Update() && filesanimclk.getElapsedTime().asMilliseconds() -
		filepageanimval1 * min(int(findedfiles.size() - (filespage * 12 + 12)), 12) > filepageanimval * 2)
		if (filespage > 0)
		{
			filespage--; filesanimclk.restart();
			for (int i = filespage * 12; i < min(filespage * 12 + 12, int(findedfiles.size())); i++) filebuttons[i % 12].SetText(findedfiles[i]);
		}
	if (rightarfiles.Update() && filesanimclk.getElapsedTime().asMilliseconds() -
		filepageanimval1 * min(int(findedfiles.size() - (filespage * 12 + 12)), 12) > filepageanimval * 2)
		if ((filespage + 1) * 12 < findedfiles.size())
		{
			filespage++; filesanimclk.restart();
			for (int i = filespage * 12; i < min(filespage * 12 + 12, int(findedfiles.size())); i++) filebuttons[i % 12].SetText(findedfiles[i]);
		}
	for (int i = 0; i < 12; i++)
		if (float(abs(max(0, filesanimclk.getElapsedTime().asMilliseconds() -
			filepageanimval1 * (i % 12)) - filepageanimval)) / float(filepageanimval) >= 0.99)
			if (filebuttons[i].Update())
			{
				sendingfile = filebuttons[i].text.getString();
				wstring yes = L"@@sendfile~" + sendingfile;
				Code(yes, IpAddress::getLocalAddress());
				Packet pp; pp << yes; sendingfilestate = true;
				socket4.send(pp, contacts[choosedcontacttotalk].ip, unsigned short(2002));
				startanimcostil = 3; filespage = 0; startanimclk.restart();
				sendtowho = contacts[choosedcontacttotalk].ip;
				sendingfilestate = true; Sleep(150);
			}
}

void WantToTakeFileUpdate()
{
	if (takefilebut[0].Update())
	{
		wstring yes = L"!!sendfileokay"; Code(yes, IpAddress::getLocalAddress());
		Packet pp; pp << yes;
		socket4.send(pp, sendfromwho, unsigned short(2002));
		progmode = ProgMode::Messaging;
		clkforstat.restart();
	}
	if (takefilebut[1].Update())
	{
		wstring yes = L"!!sendfileno"; Code(yes, IpAddress::getLocalAddress());
		Packet pp; pp << yes;
		socket4.send(pp, sendfromwho, unsigned short(2002));
		progmode = ProgMode::Messaging;
		takingfile = false;
	}
}