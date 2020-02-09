#pragma once
#include <string>

using namespace std;

class Character {
public:
	std::string name;
	bool connected = true;
	std::string characterFileName;
public:
	Character();
	Character(std::string initName);
	std::string GetName();
	std::string GetCharacterFileName();
};
 
Character::Character() 
{
	name = "";
	characterFileName = "";
	connected = false;
}

Character::Character(string initName) 
{
	name = initName;
	characterFileName = name + ".xml";
	connected = true;
}

string Character::GetName()
{
	return name;
}

string Character::GetCharacterFileName()
{
	return characterFileName;
}
