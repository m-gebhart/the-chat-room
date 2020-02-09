#pragma once
#include "GameManager.h"
#include "Character.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "pch.h"
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <chrono> 
#include "rapidxml.hpp"
#include <map>

using namespace rapidxml;
using namespace std;

class GameManager {
	vector<string> yesInput = {"yes", "Yes", "y", "Y", "ja", "Ja", "YES"};
	vector<string> noInput = {"no", "No", "n", "N", "nein", "Nein", "NO"};
public:
	map<string, int> rating;
	char* nextScene;
	string currentChar;
	string* ptr_currentChar;
	string nextChar;
	string* ptr_nextChar;
	string altNextChar;
	string* ptr_altNextChar;

	int count;
	int* ptr_count;

	bool disconnecting;
	bool useDefaultChar;
public:
	GameManager();
	void Sleep(float time);
	void BeginningAnim();
	void EndingAnim();
	void ConnectingAnim(string* name);
	string* GetNewCharacter(bool dflt);
	void Clear();
	bool GetInput();
	rapidxml::xml_node<>* GetQuestion(rapidxml::xml_node<>* scene_node, string id);
	void PrintQuestion(rapidxml::xml_node<>* question_node);
	void PrintLines(rapidxml::xml_node<>* base_node);
	void PrintReaction(rapidxml::xml_node<>* question_node, bool yes);
	string GetNextScene();
};

GameManager::GameManager()
{
	nextScene = NULL;

	currentChar = "";
	ptr_currentChar = &currentChar;
	nextChar = "";
	ptr_nextChar = &nextChar;
	altNextChar = "";
	ptr_altNextChar = &altNextChar;

	disconnecting = false;
	useDefaultChar = true;

	count = 0;
	ptr_count = &count;
}

void GameManager::Sleep(float time)
{
	int sleepTime = static_cast<int>(time * 1000);
	this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
}

void GameManager::BeginningAnim()
{
	cout << "\n\n Welcome to the ChatRoom!\n __________________________________________________________________________________\n\n";
	Sleep(0.3);
	cout << " RULES: \n In the ChatRoom, two persons get randomly connected with each other.\n";
	Sleep(0.3);
	cout << " One is the MASTER, the other the DOG.\n";
	Sleep(0.3);
	cout << " The MASTER can state questions about anything. The DOG can only answer with yes or no.\n";
	Sleep(0.3);
	cout << " You can always leave the current chat by typing DISCONNECT.\n";
	Sleep(0.3);
	cout << " Type START, if you want to begin today's ChatRoom session.\n";
	//Asking for Start Input
	bool init = true;
	while (init) {
		string start;
		cin >> start;
		if (start == "START")
		{
			init = false;
			Clear();
			cout << "\n Today you are:";
			Sleep(2);
			cout << " Dog \n You can only answer with yes or no.\n";
			Sleep(3.5);
		}
	}
}

void GameManager::ConnectingAnim(string* name)
{
	Clear();
	cout << "    Connecting";
	for (int i = 0; i < 3; i++)
	{
		Sleep(1);
		cout << ".";
	}
	cout << "\n    CONNECTED \n__________________________________________________________________________________ \n\n";
	cout << " You are connected with " + *name + "\n\n";
}

string* GameManager::GetNewCharacter(bool defaultChar)
{
	if (!defaultChar)
	{
		defaultChar = true;
		if (*ptr_altNextChar != "none")
			return ptr_altNextChar;
	}
	return ptr_nextChar;
}

void GameManager::Clear()
{
	system("CLS");
}


bool GameManager::GetInput()
{
	string input;
	cin >> input;

	//if input ~ yes
	for (int y = 0; y < yesInput.size(); y++)
		if (input == yesInput[y])
			return true;

	//if input ~ no
	for (int n = 0; n < noInput.size(); n++)
		if (input == noInput[n])
			return false;

	//if input ~ DISCONNECT
	if (input == "DISCONNECT")
	{
		useDefaultChar = false;
		disconnecting = true;
		cout << "YOU DISCONNECTED";
		Sleep(1);

		return false;
	}

	//if invalid Input
	else
	{
		cout << "ChatRoom: REPLY NOT SENT DUE TO INVALID INPUT - TRY AGAIN \n";
		return GameManager::GetInput();
	}
}


xml_node<>* GameManager::GetQuestion(xml_node<>* scene_node, string id)
{
	//finding the right question by id from .xml-file
	for (xml_node<>* question_node = scene_node->first_node("question"); question_node; question_node = question_node->next_sibling())
	{
		if (question_node->first_attribute("id")->value() == id)
			return question_node;
	}
	return NULL;
}

void GameManager::PrintQuestion(xml_node<>* question_node)
{
	PrintLines(question_node);
}

void GameManager::PrintLines(xml_node<>* base_node)
{
	//printing every <line> between base_node <lines> from .xml-file
	//wait Sleep(time) before printing next <line>, dependent on its length
	for (xml_node<>* line_node = base_node->first_node("lines")->first_node("line"); line_node; line_node = line_node->next_sibling())
	{
		Sleep(line_node->value_size()*0.068);
		cout << " - " << line_node->value() << "\n";
	}
}

void GameManager::PrintReaction(xml_node<>* question_node, bool yes)
{
	//NPC's reaction to specific question according to bool: yes
	for (xml_node<>* reaction_node = question_node->first_node("reaction"); reaction_node; reaction_node = reaction_node->next_sibling())
	{
		if (yes && reaction_node->first_attribute("id")->value() == (string)"Yes")
		{
			nextScene = reaction_node->first_attribute("nextScene")->value();
			PrintLines(reaction_node);
		}
		if (!yes && reaction_node->first_attribute("id")->value() == (string)"No")
		{
			nextScene = reaction_node->first_attribute("nextScene")->value();
			PrintLines(reaction_node);
		}
	}
}

string GameManager::GetNextScene()
{
	return string(nextScene);
}

void GameManager::EndingAnim()
{
	Clear();
	cout << " End of Session \n \n Summary of today's session:\n";

	//Output rating list
	for (auto i = rating.cbegin(); i != rating.cend(); i++)
	{
		cout << "  " << (*i).first << ": " << (*i).second << " Questions" << endl;
	}

	getchar();
	getchar();
}