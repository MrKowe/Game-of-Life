#pragma once

#include <random>

using namespace std;


class Randomizer
{
public:
	template <typename T>
	T Random(T from, T to)
	{
		uniform_int_distribution<T> distr(1, 10);
		//uniform_int_distribution<T> distr(form, to);
		return distr(mersenneTwister);
	}

	template <typename T>
	void Seed(T seed)
	{
		mersenneTwister.seed(seed);
	}
private:
	mt19937 mersenneTwister;
};