#include "pch.h"
#include <fstream>
#include <vector>
#include "GameManager.h"
#include "Character.h"

using namespace std;

GameManager manager;
void ConnectToCharacter(string* currentCharacter);
void Dialogue(Character* character);

Character* character;
xml_document<> doc;
xml_node<>* scene_node;
xml_node<>* current_question_node;


int main()
{
	manager.BeginningAnim();

	//Starting Character 0: Amber
	*manager.ptr_currentChar = "Amber";
	ConnectToCharacter(manager.ptr_currentChar);

	//GameLoop: Connect to new Character
	while (manager.nextChar != "none")
	{
		ConnectToCharacter(manager.GetNewCharacter(manager.useDefaultChar));
	}

	manager.EndingAnim();
}

void ConnectToCharacter(string* currentCharacter)
{
	manager.ConnectingAnim(currentCharacter);

	//Setting current character
	character = new Character(*currentCharacter);
	//Setting character's .xml-file
	ifstream currentFile(character->GetCharacterFileName());
	vector<char> buffer((istreambuf_iterator<char>(currentFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	//Start Dialogue
	Dialogue(character);
	delete(character);
}

void Dialogue(Character* character) 
{
	//reset data for new character
	manager.disconnecting = false;
	int questionCount = 1;

	//declaring new <scenes>_node (base_node of questions)
	scene_node = doc.first_node("scenes");
	*manager.ptr_nextChar = scene_node->first_attribute("nextCharacter")->value();
	*manager.ptr_altNextChar = scene_node->first_attribute("altNextCharacter")->value();

	//stating first / start question
	current_question_node = manager.GetQuestion(scene_node, "1");
	manager.PrintQuestion(current_question_node);

	for (int i = 0; character->connected; i)
	{
		//getting player's input (yes, no, DISCONNECT or >invalid< input)
		//bool GetInput() = Yes or No
		bool recentInput = manager.GetInput();
		

		//if player didn't DISCONNECT before
		if (!manager.disconnecting)
		{
			manager.PrintReaction(current_question_node, recentInput);

			//Getting next question
			if (character->connected && manager.GetNextScene() != "DISCONNECT")
			{
				current_question_node = manager.GetQuestion(scene_node, manager.GetNextScene());
				manager.PrintQuestion(current_question_node);
				questionCount++;
			}
			//Disconnection by NPC
			else if (manager.GetNextScene() == "DISCONNECT")
			{
				cout << "\n DISCONNECTED \n\n";
				manager.Sleep(5);
				character->connected = false;
			}
		}
		else
		{
			//Disconnection by Player
			character->connected = false;
		}
	}

	//result screen at the end of the session
	manager.rating[character->name] = questionCount;
}