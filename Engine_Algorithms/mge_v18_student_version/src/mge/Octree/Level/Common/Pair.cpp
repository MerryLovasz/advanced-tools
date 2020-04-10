#include "Pair.hpp"


Pair::Pair(GameObject * one, GameObject * other)
{
	this->One = one;
	this->Other = other;
}

Pair::~Pair()
{
}