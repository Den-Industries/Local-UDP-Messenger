#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <thread>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <filesystem>
#include "libxl.h"
#include "GlobalClassesAndStructs.h"

using namespace sf;
using namespace std;

struct StatInfo
{
	unsigned int time;
	unsigned int size;
	String filename;
	String date;
};

struct Message
{
	String str;
	String sender;
	String date;
	bool checked = false;
	Message(String _str, String _sender)
	{
		str = _str; sender = _sender;
		wstringstream wss;
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		wss << now->tm_hour << L':';
		if (now->tm_min < 10) wss << L'0';
		wss << now->tm_min << L':';
		if (now->tm_sec < 10) wss << L'0';
		wss << now->tm_sec;
		date = wss.str();
	}
};

int ScreenSize[2] = { 960, 640 }, choosedcontacttotalk = -1, lastchoosedcontacttotalk = -1, filespage = 0, 
qofcontacts = 0, startanimval = 500, contactspage = 0, switchpageanimval = 400, switchpageanimval1 = 100, 
dialoganimval1 = 400, filepageanimval = 400, filepageanimval1 = 50, startanimcostil = 0, startanim1val = 500,
whichpartoffilesended = 0, qofpartsofsendingfile = -1, streamanimvals[2] = {0, 0};
bool working = true, takingfile = false, sendingfilestate = false, receivestream = false, streamanim[2] = {false, false}, startflag = false;
vector<IpAddress> ipadresses;
vector<Contact> contacts;
vector<int> streamdesktowho;
vector<pair<int, int>> streaminfo;
ButtonList dialogfunctions, globfunctions;
Button contactsbuttons[5], namelabel, takefilebut[2];
Button leftar, rightar, exitbutton, sendbutton, closebutton, sendfilesback, filebuttons[12], leftarfiles, rightarfiles;
Font mainfont;
Clock updateclock, monitoringclock, switchpageclk, dialoganimclk, filesanimclk, 
	startanimclk, clkforstat, clkforstat1, streamanimclock, clockforstream;
UdpSocket socket1, socket2, socket3, socket4, socket5, socket6; //1, 2 для обнаружения друг друга, 3 4 для общения, 5 6 для картинок
String name, sendbuffer, sendingfile, gettingfile;
vector<vector<Message>> dialogs;
vector<String> findedfiles;
ProgMode progmode = ProgMode::EnterName;
TextBox nameinput, dialoginput;
Label entername, dialogname, takefile;
Plate dialogplate;
IpAddress sendtowho, sendfromwho;
ifstream input, inputstream;
vector<unsigned char> buffer, bufferstream;
vector<StatInfo> statistics;
wstring thefile, thefilestream;
SPRITE streaminspr, streamoutspr;
StatInfo statinfobuf;
Trackbar bright, textsizetb;
Color bgcolor(200, 230, 255);

Texture txtr;
Image imgbuf;
Sprite sprbuf;
RenderTexture rndrtxtr;
Image img;
HDC hdc;
HDC memdc;
HBITMAP hbitmap;
HGDIOBJ oldbmp;
BITMAPINFO bi = { 0 };
unsigned char* lpPixels;
int width, height, TEXTSIZE = 10;

bool FileIsExist(std::string filePath)
{
	bool isExist = false;
	std::ifstream fin(filePath.c_str());
	if (fin.is_open()) isExist = true;
	fin.close();
	return isExist;
}

void Code(wstring &str, IpAddress ip)
{
	int buf = pow(2, 16);
	for (int i = 0; i < str.size(); i++)
		str[i] = (str[i] + 5 + i % 20 + int(ip.toString()[i % ip.toString().size()]) % 100) % buf;
}

void Decode(wstring& str, IpAddress ip)
{
	int buf = pow(2, 16);
	for (int i = 0; i < str.size(); i++)
		str[i] = (str[i] - 5 - i % 20 - int(ip.toString()[i % ip.toString().size()]) % 100) % buf;
}

void GetLanIP() {
	/*
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen("arp -a > nigger.txt", "r"), _pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (fgets(buffer.data(), int(buffer.size()), pipe.get()) != nullptr) result += buffer.data();*/
	ShellExecute(0, L"open", L"cmd.exe", L"/C arp -a > TMP/ipstmp.txt", 0, SW_HIDE);
	ifstream ipsfile("TMP/ipstmp.txt");
	std::string result = "", buff;
	while (getline(ipsfile, buff)) result += buff + "\n";
	int buf = 0;
	while (buf < result.size())
	{
		buf = result.find("\n", size_t(buf) + 1);
		if (buf < result.size())
			if (result[buf + 1] == ' ' && isdigit(result[buf + 3]))
			{
				if (std::find(ipadresses.begin(), ipadresses.end(),
					IpAddress(result.substr(buf + 3, 15))) >= ipadresses.end())
				{
					ipadresses.push_back(result.substr(buf + 3, 15));
				}
			}
	}
	//for (int i = 0; i < ipadresses.size(); i++) std::cout << ipadresses[i].toString() << endl;
}

void UpdateContactButtons()
{
	for (int i = 5 * contactspage; i < min(5 * (1 + contactspage), int(contacts.size())); i++)
	{
		contactsbuttons[i % 5].SetText(contacts[i].name);
			//Create("timesnewroman.ttf", contacts[i].name, Color(245, 245, 255),
			//Color(100, 115, 127), FloatRect(15.0f, 95.0f + (i % 5) * 80.0f, 330.0f, 65.0f), 18);
	}
	Sleep(10);
}

void Init()
{
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	lpPixels = new unsigned char[width * height * 4];
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = width;
	bi.bmiHeader.biHeight = -height;
	bi.bmiHeader.biBitCount = 32; //32-bit bitmap
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biCompression = BI_RGB;
	//lpPixels = new unsigned char[height * width * 4];
	std::cout << "init" << endl;
	mainfont.loadFromFile("timesnewroman.ttf");
	bool success = 
		socket1.bind(2000) == 0 && socket2.bind(2001) == 0 && 
		socket3.bind(2002) == 0 && socket4.bind(2003) == 0 &&
		socket5.bind(2004) == 0 && socket6.bind(2005) == 0;
	std::cout << "Socket1 binded " << socket1.getLocalPort() << endl;
	std::cout << "Socket2 binded " << socket2.getLocalPort() << endl;
	std::cout << "Socket3 binded " << socket3.getLocalPort() << endl;
	std::cout << "Socket4 binded " << socket4.getLocalPort() << endl;
	std::cout << "Socket5 binded " << socket5.getLocalPort() << endl;
	std::cout << "Socket6 binded " << socket6.getLocalPort() << endl;
	if (!FileIsExist("timesnewroman.ttf")) success = false;
	entername.Create("timesnewroman.ttf", L"Введите имя", Color(245, 245, 255), Color(100, 115, 127), FloatRect(480.0f - 240.0f, 200.0f, 480.0f, 60.0f), 40, false);
	nameinput.Create("timesnewroman.ttf", Color(245, 245, 255), Color(100, 115, 127), FloatRect(480.0f - 240.0f, 320.0f - 40.0f, 480.0f, 80.0f), 40, 18);
	namelabel.Create("timesnewroman.ttf", L"", Color(245, 245, 255), Color(100, 115, 127), FloatRect(15.0f, 15.0f, 330.0f, 50.0f), 20);
	bright.Create("timesnewroman.ttf", L"Яркость", Color(245, 245, 255), Color(100, 115, 127), FloatRect(240.0f, 380.0f, 480.0f, 40.0f), 20);
	bright.value = 19;
	bright.trackrectshape.setPosition(bright.rectshape.getGlobalBounds().left + 5
		+ ((bright.rectshape.getGlobalBounds().width - 10) / 20) * bright.value,
		bright.rectshape.getGlobalBounds().top + 5);
	textsizetb.Create("timesnewroman.ttf", L"Размер шрифта", Color(245, 245, 255), Color(100, 115, 127), FloatRect(240.0f, 430.0f, 480.0f, 40.0f), 20);
	dialogname.Create("timesnewroman.ttf", L"", Color(245, 245, 255), Color(100, 115, 127), FloatRect(365.0f, 15.0f, 330.0f, 45.0f), 20);
	
	globfunctions.Create(3, 300);
	globfunctions.lbstropen = L"Глобальные функции";
	globfunctions.lbstrback = L"Закрыть";
	globfunctions.PreCreateAll("timesnewroman.ttf", Color(245, 245, 255),
		Color(100, 115, 127), FloatRect(15.0f, 575.0f, 330.0f, 50.0f), 30, false, 10);
	globfunctions.listbutton.SetText(L"Глобальные функции");
	globfunctions.buttons[0].SetText(L"Сохранить все диалоги");
	globfunctions.buttons[1].SetText(L"Сохранить статистику отправки файлов");
	globfunctions.buttons[2].SetText(L"Выйти");

	dialogfunctions.Create(3, 300);
	dialogfunctions.lbstropen = L"Функции диалога";
	dialogfunctions.lbstrback = L"Закрыть";
	dialogfunctions.PreCreateAll("timesnewroman.ttf", Color(245, 245, 255), 
		Color(100, 115, 127), FloatRect(700.0f, 15.0f, 195.0f, 45.0f), 20, true, 5);
	dialogfunctions.listbutton.SetText(L"Функции");
	dialogfunctions.buttons[0].SetText(L"Отправить файл");
	dialogfunctions.buttons[1].SetText(L"Сохранить диалог");
	dialogfunctions.buttons[2].SetText(L"Трансляция экрана");
	//sendbutton.Create("timesnewroman.ttf", L"Отправить файл", Color(245, 245, 255), Color(100, 115, 127), FloatRect(700.0f, 15.0f, 195.0f, 45.0f), 20);

	
	closebutton.Create("timesnewroman.ttf", L"X", Color(245, 245, 255), Color(100, 115, 127), FloatRect(365.0f + 530.0f, 15.0f, 50.0f, 45.0f), 40);
	dialogplate.Create(Color(100, 115, 127), FloatRect(365.0f, 65.0f, 580.0f, 500.0f));
	dialoginput.Create("timesnewroman.ttf", Color(245, 245, 255), Color(100, 115, 127), FloatRect(365.0f, 20.0f + 555.0f, 580.0f, 50.0f), 30, 55);
	leftar.Create("timesnewroman.ttf", L"<", Color(245, 245, 255), Color(100, 115, 127), FloatRect(15.0f, 515.0f, 160.0f, 50.0f), 40);
	rightar.Create("timesnewroman.ttf", L">", Color(245, 245, 255), Color(100, 115, 127), FloatRect(185.0f, 515.0f, 160.0f, 50.0f), 40);
	//exitbutton.Create("timesnewroman.ttf", L"Выход", Color(245, 245, 255), Color(100, 115, 127), FloatRect(15.0f, 575.0f, 330.0f, 50.0f), 30);
	sendfilesback.Create("timesnewroman.ttf", L"Вернуться", Color(245, 245, 255), Color(100, 115, 127), FloatRect(480.0f - 240.0f, 560.0f, 480.0f, 65.0f), 40, 18);
	for (int i = 0; i < 12; i++)
		filebuttons[i].Create("timesnewroman.ttf", "", Color(245, 245, 255), Color(100, 115, 127), FloatRect(480.0f - 240, 40.0f + 40.0f * i, 480.0f, 30.0f), 20);
	leftarfiles.Create("timesnewroman.ttf", L"<", Color(245, 245, 255), Color(100, 115, 127), FloatRect(140.0f, 290.0f, 60.0f, 60.0f), 60);
	rightarfiles.Create("timesnewroman.ttf", L">", Color(245, 245, 255), Color(100, 115, 127), FloatRect(760.0f, 290.0f, 60.0f, 60.0f), 60);
	takefilebut[0].Create("timesnewroman.ttf", L"Да", Color(245, 245, 255), Color(100, 115, 127), FloatRect(480.0f - 240.0f, 320.0f, 240.0f, 120.0f), 60);
	takefilebut[1].Create("timesnewroman.ttf", L"Нет", Color(245, 245, 255), Color(100, 115, 127), FloatRect(480.0f, 320.0f, 240.0f, 120.0f), 60);
	takefile.Create("timesnewroman.ttf", L"Хотите принять файл", Color(245, 245, 255), Color(100, 115, 127), FloatRect(480.0f - 240.0f, 320.0f - 120.0f, 480.0f, 120.0f), 20);
	streaminspr.Create(Vector2i(580, 325));
	streamoutspr.Create(Vector2i(580, 325));
	for (int i = 0; i < 5; i++)
	{
		contactsbuttons[i].Create("timesnewroman.ttf", "0_0", Color(245, 245, 255),
			Color(100, 115, 127), FloatRect(15.0f, 95.0f + i * 80.0f, 330.0f, 65.0f), 18);
	}
	Sleep(300);
	window.create(VideoMode(ScreenSize[0], ScreenSize[1]), "Programm", Style::Close, ContextSettings(0, 0, 8));
	if (!success)
	{
		window.close();
		working = false;
		std::cout << "Failed to bind sockets" << endl;
	}
	for (int i = 0; i < 1; i++)
	{
		wstringstream stst; stst << L"!!Local";
		String str = stst.str();
		contacts.push_back(Contact(IpAddress::getLocalAddress(), str.substring(2, str.getSize() - 1).toWideString()));
		qofcontacts++; vector<Message> need;
		need.push_back(Message(L"Диалог с " + contacts[contacts.size() - 1].name.toWideString(), L"System"));
		dialogs.push_back(need);
		need.clear();
	}
	UpdateContactButtons();
	startanimclk.restart();
}

void MonitoringFunc1()
{
	while (working)
	{
		if (monitoringclock.getElapsedTime().asMilliseconds() > 1000 && progmode != ProgMode::EnterName)
		{
			monitoringclock.restart();
			GetLanIP();
			if (name.getSize() > 0)
			{
				//wstd::cout << L"SENDING " << endl;
				for (int i = 0; i < ipadresses.size(); i++)
				{
					wstring yes = L"$@$" + name.toWideString();
					Packet pp; pp << yes;
					socket2.send(pp, ipadresses[i], 2000);
				}
			}
		}
	}
}

void MonitoringFunc2()
{
	Packet pp;
	IpAddress inIp;
	size_t received;
	unsigned short inport;
	while (working)
	{
		pp.clear();
		while(!window.hasFocus()) {}
		socket1.receive(pp, inIp, inport);
		wstring wstr;
		if (pp >> wstr && inIp != IpAddress::getLocalAddress())
		{
			//wstd::cout << L"GETTING " << wstr << endl;
			if (wstr[0] == L'$' && wstr[1] == L'@' && wstr[2] == L'$' && wstr.size() > 3 && progmode != ProgMode::EnterName)
			{
				bool hasbeen = false;
				for (int i = 0; i < contacts.size(); i++)
				{
					if (contacts[i].ip == inIp) hasbeen = true;
				}
				if (!hasbeen && wstr.substr(3, wstr.size() - 3) != name)
				{
					std::cout << "\nHOMIE DETECTED " << inIp << " " << inport << "\n";
					contacts.push_back(Contact(inIp, wstr.substr(3, wstr.size() - 3)));
					qofcontacts++;
					vector<Message> need;
					need.push_back(Message(L"Диалог с " + contacts[contacts.size() - 1].name, L"System"));
					dialogs.push_back(need);
					UpdateContactButtons();
					need.clear();
				}
			}
		}
	}
}

void MonitoringFunc3()
{
	Packet pp; IpAddress inIp; size_t received;
	wofstream sendfilestream; unsigned short inport;
	while (working)
	{
		pp.clear(); while (!window.hasFocus()) {}
		socket3.receive(pp, inIp, inport); wstring wstr;
		if (pp >> wstr)
		{
			Decode(wstr, inIp);
			if (wstr[0] == L'!' && wstr[1] == L'!' && sendingfilestate)
			{
				if (wstr.size() == 14 && wstr[2] == L's' && wstr[3] == L'e' && wstr[4] == L'n' && wstr[5] == L'd')
				{
					clkforstat1.restart();
					if (whichpartoffilesended > qofpartsofsendingfile && qofpartsofsendingfile != -1)
					{
						sendingfilestate = false;
						wstring yes = L"&*thatsall"; 
						Code(yes, IpAddress::getLocalAddress());
						Packet pp; pp << yes;
						socket4.send(pp, sendfromwho, unsigned short(2002));
						whichpartoffilesended = 0;
						qofpartsofsendingfile = -1;
						buffer.clear(); //thefile.clear();
					}
					else
					{
						if (qofpartsofsendingfile == -1)
						{
							input.open(L"Share/" + sendingfile.toWideString(), std::ios::binary);
							buffer.assign(std::istreambuf_iterator<char>(input), {});
							thefile.assign(buffer.begin(), buffer.end());
							qofpartsofsendingfile = (thefile.size() / ((UdpSocket::MaxDatagramSize - 2) / 4 - 6));
							input.close();
						}
						Packet ppp;
						wstring buf = L"&&" + thefile.substr(whichpartoffilesended *
							((UdpSocket::MaxDatagramSize) / 4 - 6),
							((UdpSocket::MaxDatagramSize) / 4 - 6));
						Code(buf, IpAddress::getLocalAddress());
						ppp << buf;
						socket4.send(ppp, sendtowho, unsigned short(2002));
						whichpartoffilesended++;
					}
				}
				if (wstr.size() == 12 && wstr[2] == L's' && wstr[3] == L'e' && wstr[4] == L'n' && wstr[5] == L'd')
					sendingfilestate = false;
			}
			if (wstr[0] == L'&' && wstr[1] == L'*' && wstr[2] == L't' && takingfile)
			{
				takingfile = false; sendfilestream.close();
				statinfobuf.time = clkforstat.getElapsedTime().asMilliseconds();
				statinfobuf.size = thefile.size();
				statinfobuf.filename = sendingfile;
				wstringstream wss;
				std::time_t t = std::time(0);
				std::tm* now = std::localtime(&t);
				wss << now->tm_hour << L':';
				if (now->tm_min < 10) wss << L'0';
				wss << now->tm_min << L':';
				if (now->tm_sec < 10) wss << L'0';
				wss << now->tm_sec;
				statinfobuf.date = wss.str();
				statistics.push_back(statinfobuf);
				thefile.clear();
			}
			if (wstr[0] == L'&' && wstr[1] == L'&' && takingfile)
			{
				int who = 0; //wcout << L"Packet received" << endl;
				for (int i = 0; i < contacts.size(); i++) if (inIp == contacts[i].ip) who = i;
				if (!sendfilestream.is_open())
				{
					wstring yess = L"Share/from " + contacts[who].name + L" " + gettingfile.toWideString();
					sendfilestream.open(yess, ios::out | ios::binary | ios::app);
				}
				sendfilestream.write((const wchar_t*)&wstr[2], wstr.size() - 2);
				wstring yes = L"!!sendfileokay";
				Code(yes, IpAddress::getLocalAddress());
				Packet pp; pp << yes;
				socket4.send(pp, sendfromwho, unsigned short(2002));
			}
			if (wstr[0] == L'@' && wstr[1] == L'@' && wstr.substr(2, 10) == L"changename")
			{
				for (int i = 1; i < contacts.size(); i++) if (inIp == contacts[i].ip)
					contacts[i].name = wstr.substr(12, wstr.size() - 12);
				UpdateContactButtons();
			}
			else
			{
				if (wstr[0] == L'@' && wstr[1] == L'@' && wstr.substr(2, 9) == L"sendfile~" && !takingfile)
				{
					int who = 0; takingfile = true;
					for (int i = 0; i < contacts.size(); i++) if (inIp == contacts[i].ip) who = i;
					gettingfile = wstr.substr(11, wstr.size() - 11);
					progmode = ProgMode::WantToTakeFile; sendfromwho = contacts[who].ip;
					takefile.SetText(L"Хотите принять файл\n" + gettingfile + L"\nот\n" + contacts[who].name + "?");
				}
				else
				{
					if (wstr[0] == L'@' && wstr[1] == L'@')
						for (int i = 0; i < contacts.size(); i++)
							if (inIp == contacts[i].ip)
							{
								dialogs[i].push_back(Message(contacts[i].name.toWideString() 
									+ L": " + wstr.substr(2, wstr.size() - 2), contacts[i].name.toWideString()));
							}
				}
			}
		}
	}
}

void TakeScreenShot(Vector2i TargetMetrics, string str)
{
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
	hdc = GetDC(0);
	memdc = CreateCompatibleDC(hdc);
	hbitmap = CreateCompatibleBitmap(hdc, width, height);
	oldbmp = SelectObject(memdc, hbitmap);
	BitBlt(memdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY | CAPTUREBLT);
	SelectObject(memdc, oldbmp);
	DeleteDC(memdc);
	GetDIBits(hdc, hbitmap, 0, height, lpPixels, &bi, DIB_RGB_COLORS);
	if (working)
	{
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				int i = (y * width + x) * 4;
				lpPixels[i + 2] += lpPixels[i + 0];
				lpPixels[i + 0] = lpPixels[i + 2] - lpPixels[i + 0];
				lpPixels[i + 2] -= lpPixels[i + 0];
			}

		txtr.create(width, height);
		txtr.update((sf::Uint8*)lpPixels, width, height, 0, 0);
		imgbuf = txtr.copyToImage();
		txtr.loadFromImage(imgbuf);
		sprbuf.setTexture(txtr);
		sprbuf.setScale(double(TargetMetrics.x) / double(width),
			-(double(TargetMetrics.y) / double(height)));
		sprbuf.setPosition(0, TargetMetrics.y);
		rndrtxtr.create(TargetMetrics.x, TargetMetrics.y);
		rndrtxtr.draw(sprbuf);
		img = rndrtxtr.getTexture().copyToImage();
		img.saveToFile(str);
	}
}

void MonitoringFunc4()
{
	bool neeedit = false;
	Packet pp;
	IpAddress inIp;
	size_t received;
	wofstream input;
	unsigned short inport;
	while (working)
	{
		if (!window.hasFocus()) receivestream = false;
		pp.clear(); socket5.receive(pp, inIp, inport); wstring wstr;
		if (pp >> wstr)
		{
			if (wstr[0] == L'*' && wstr[1] == L'*' && wstr[2] == L'*')
			{
				neeedit = true; receivestream = false;
				wofstream myfile("TMP/tmp_in" + inIp.toString() + ".jpg", std::ofstream::trunc);
				myfile.close();
			}
			if (wstr[0] == L'%' && wstr[1] == L'#' && wstr[2] == L'%') {
				wstring yes = L"!!!"; Packet pp; pp << yes;
				socket6.send(pp, inIp, unsigned short(2004)); std::cout << inIp << " STREAM" << endl;
				if (contacts[choosedcontacttotalk].ip == inIp)  receivestream = true;
			}
			if (wstr[0] == L'!' && wstr[1] == L'!' && wstr[2] == L'!')
			{
				int need = -1;
				for (int i = 0; i < contacts.size(); i++)
					if (contacts[i].ip == inIp) { need = i; break; }
				for (int i = 0; i < streamdesktowho.size(); i++)
					if (streamdesktowho[i] == need) { need = i; break; }
				if (streaminfo[need].second > streaminfo[need].first && streaminfo[need].first != -1) {
					wstringstream wss; wss << L")*( ";
					if (true)
					{
						ifstream tmpinput;
						vector<unsigned char> tmpbuffer;
						tmpinput.open("TMP/tmp" + inIp.toString() + ".jpg", ios::out | ios::binary | ios::app);
						tmpbuffer.assign(std::istreambuf_iterator<char>(tmpinput), {});
						wss << (tmpbuffer.end() - tmpbuffer.begin()); //cout << (tmpbuffer.end() - tmpbuffer.begin()) << endl;
						tmpinput.close();
					}
					Packet pp; pp << wss.str();
					socket6.send(pp, inIp, unsigned short(2004));
					streaminfo[need].first = -1;
					inputstream.close();
					bufferstream.clear();
					thefilestream.clear();
					while (clockforstream.getElapsedTime().asMilliseconds() < 100) {}
					clockforstream.restart();
				}
				else
				{
					if (streaminfo[need].first == -1 && working) {
						if (window.hasFocus()) {
							if (FileIsExist("TMP/tmp" + inIp.toString() + ".jpg"))
							{
								wofstream myfile("TMP/tmp" + inIp.toString() + ".jpg", std::ofstream::trunc);
								myfile.close();
							}
							TakeScreenShot(Vector2i(580, 325), "TMP/tmp" + inIp.toString() + ".jpg");
						}
						if (FileIsExist("TMP/tmp" + inIp.toString() + ".jpg"))
						{
							streamoutspr.Load("TMP/tmp" + inIp.toString() + ".jpg");
							inputstream.open("TMP/tmp" + inIp.toString() + ".jpg", std::ios::binary);
							bufferstream.assign(std::istreambuf_iterator<char>(inputstream), {});
							thefilestream.assign(bufferstream.begin(), bufferstream.end());
							streaminfo[need].first = (bufferstream.size() / ((UdpSocket::MaxDatagramSize) / 4 - 8));
							streaminfo[need].second = 0;
						}
						//system("pause");
					}
					Packet ppp; ppp << L"&&&" + thefilestream.substr(streaminfo[need].second *
						((UdpSocket::MaxDatagramSize) / 4 - 8), ((UdpSocket::MaxDatagramSize) / 4 - 8));
					socket6.send(ppp, inIp, unsigned short(2004)); streaminfo[need].second++;
				}
			}
			if (wstr[0] == L'&' && wstr[1] == L'&' && wstr[2] == L'&') {
				int who = 0; //cout << "yes " << receivestream << endl;
				for (int i = 0; i < contacts.size(); i++) if (inIp == contacts[i].ip) who = i;
				if (who == choosedcontacttotalk && receivestream && window.hasFocus()) {
					if (!input.is_open())
					{
						input.open("TMP/tmp_in" + inIp.toString() + ".jpg", ios::out | ios::binary | ios::app);
					}
					input.write((const wchar_t*)&wstr[3], wstr.size() - 3); //cout << "write" << endl;
					ifstream tmpinput;
					vector<unsigned char> tmpbuffer;
					tmpinput.open("TMP/tmp_in" + inIp.toString() + ".jpg", ios::out | ios::binary | ios::app);
					tmpbuffer.assign(std::istreambuf_iterator<char>(tmpinput), {});
					//std::cout << (tmpbuffer.end() - tmpbuffer.begin()) << " !!!" << endl;
				}
				wstring yes = L"!!!"; Packet pp; pp << yes;
				socket6.send(pp, inIp, unsigned short(2004));
			}
			if (wstr[0] == L')' && wstr[1] == L'*' && wstr[2] == L'(') {
				input.close();
				if (contacts[choosedcontacttotalk].ip == inIp) receivestream = true;
				wstringstream bufff;
				bufff << wstr.substr(4, wstr.size() - 4);
				int size; bufff >> size;
				if (size != 0)
				{
					ifstream tmpinput;
					vector<unsigned char> tmpbuffer;
					tmpinput.open("TMP/tmp_in" + inIp.toString() + ".jpg", ios::out | ios::binary | ios::app);
					tmpbuffer.assign(std::istreambuf_iterator<char>(tmpinput), {});
					std::cout << (tmpbuffer.end() - tmpbuffer.begin()) << " " << size << endl;
					if ((tmpbuffer.end() - tmpbuffer.begin()) == size)
					{
						tmpinput.close();
						string buff = "TMP/tmp_in" + inIp.toString() + ".jpg";
						if (FileIsExist(buff))
							if (receivestream && window.hasFocus()) streaminspr.Load(buff);
					}
				}
				if (!neeedit) {
					neeedit = false; wstring yes = L"!!!";
					Packet pp; pp << yes; socket6.send(pp, inIp, unsigned short(2004));
				}
				string buff = "TMP/tmp_in" + inIp.toString() + ".jpg";
				if (FileIsExist(buff))
				{
					wofstream myfile(buff, std::ofstream::trunc);
					myfile.close();
				}
			}
		}
	}
}

void DrawAnimThing(float value, RectangleShape& origrect)
{
	window.draw(RectAnimThing(value, origrect));
}

void SaveAllToExcel()
{
	libxl::Book* book = xlCreateXMLBookW();
	if (book)
	{
		for (int u = 0; u < contacts.size(); u++)
		{
			libxl::Sheet* sheet = book->addSheet(contacts[u].name.toWideString().c_str());
			if (sheet)
			{
				sheet->writeStr(1, 0, L"Dialog of");
				sheet->writeStr(1, 1, name.toWideString().c_str());
				sheet->writeStr(1, 2, L"and");
				sheet->writeStr(1, 3, contacts[u].name.toWideString().c_str());
				sheet->writeStr(2, 0, L"Date");
				sheet->writeStr(2, 1, L"Time");
				sheet->writeStr(2, 2, L"Sender");
				sheet->writeStr(2, 3, L"Message");
				if (true)
				{
					wstringstream wss;
					std::time_t t = std::time(0);
					std::tm* now = std::localtime(&t);
					wss << now->tm_mday << L"." << now->tm_mon + 1;
					sheet->writeStr(3, 0, wss.str().c_str());
				}
				for (int i = 0; i < dialogs[u].size(); i++)
				{
					sheet->writeStr(3 + i, 1, dialogs[u][i].date.substring(
						dialogs[u][i].date.find(L"~") + 1,
						dialogs[u][i].date.getSize() - 1 -
						dialogs[u][i].date.find(L"~")).toWideString().c_str());
					sheet->writeStr(3 + i, 2,
						dialogs[u][i].sender.toWideString().c_str());
					sheet->writeStr(3 + i, 3,
						dialogs[u][i].str.toWideString().c_str());
				}
			}
		}
		wstring ws = L"Files\\All messages " + name.toWideString() + L" ";
		if (true)
		{
			wstringstream wss;
			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);
			wss << now->tm_mday << L'-' << (now->tm_mon + 1) << L'-' << (now->tm_year + 1900)
				<< L'~' << now->tm_hour << L':';
			if (now->tm_min < 10) wss << L'0';
			wss << now->tm_min << L':';
			if (now->tm_sec < 10) wss << L'0';
			wss << now->tm_sec;
			ws += wss.str();
		}
		ws += L".xlsx";
		std::replace(ws.begin(), ws.end(), L':', L'-');
		book->save(ws.c_str());
		book->release();
	}
}

void SaveToExcel()
{
	libxl::Book* book = xlCreateXMLBookW();
	if (book)
	{
		//String hah; TakeScreenShot(hah, Vector2i(580, 325));
		libxl::Sheet* sheet = book->addSheet(L"Messages");
		if (sheet)
		{
			sheet->writeStr(1, 0, L"Dialog of");
			sheet->writeStr(1, 1, name.toWideString().c_str());
			sheet->writeStr(1, 2, L"and");
			sheet->writeStr(1, 3, contacts[choosedcontacttotalk].name.toWideString().c_str());
			sheet->writeStr(2, 0, L"Date");
			sheet->writeStr(2, 1, L"Time");
			sheet->writeStr(2, 2, L"Sender");
			sheet->writeStr(2, 3, L"Message");
			if (true)
			{
				wstringstream wss;
				std::time_t t = std::time(0);
				std::tm* now = std::localtime(&t);
				wss << now->tm_mday << L"." << now->tm_mon + 1;
				sheet->writeStr(3, 0, wss.str().c_str());
			}
			for (int i = 0; i < dialogs[choosedcontacttotalk].size(); i++)
			{
				sheet->writeStr(3 + i, 1, dialogs[choosedcontacttotalk][i].date.substring(
					dialogs[choosedcontacttotalk][i].date.find(L"~") + 1,
					dialogs[choosedcontacttotalk][i].date.getSize() - 1 -
					dialogs[choosedcontacttotalk][i].date.find(L"~")).toWideString().c_str());
				sheet->writeStr(3 + i, 2,
					dialogs[choosedcontacttotalk][i].sender.toWideString().c_str());
				sheet->writeStr(3 + i, 3,
					dialogs[choosedcontacttotalk][i].str.toWideString().c_str());
			}
		}
		wstring ws = L"Files\\Dialog " + name.toWideString() + L"-" + contacts[choosedcontacttotalk].name.toWideString()
			+ L" " + dialogs[choosedcontacttotalk][dialogs[choosedcontacttotalk].size() - 1].date + L".xlsx";
		std::replace(ws.begin(), ws.end(), L':', L'-');
		book->save(ws.c_str()); //std::cout << "hey" << endl;
		book->release();
	}
}