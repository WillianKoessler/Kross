#pragma once
#include <string>

class Item
{
public:
	static int worldid;
	int id;
	std::string name;
	std::string description;
	int w;
	int qtd;

	virtual bool Use() { return true; }
};

class Consu : public Item
{

};

class Equip : public Item
{

};

class Junk : public Item
{

};