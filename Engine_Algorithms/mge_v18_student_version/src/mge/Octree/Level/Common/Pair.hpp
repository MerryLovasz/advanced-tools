#ifndef PAIR_HPP
#define PAIR_HPP

#include "mge/core/GameObject.hpp"

struct Pair
{

	public:
		struct Pair(GameObject * one, GameObject * other);
		virtual ~Pair();
		GameObject * One;
		GameObject  * Other;

};

#endif