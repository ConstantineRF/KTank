#pragma once
#include <vector>
#include "MapObject.h"
#include "Vec2.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <queue>

class MapInterpretation
{
public:
	MapInterpretation(int c, int r, Vei2 ingoal) : goal(ingoal), c(c), r(r)
	{
		//map = std::vector<std::vector<int>>(std::vector<int>(0, c), r);
		map.resize(c);
		for (int i = 0; i < c; i++)
		{
			map[i] = std::vector<int>(r, 0);
		}
		route.resize(c-3);
		for (int i = 0; i < c-3; i++)
		{
			route[i] = std::vector<int>(r-3, 1000000);
		}
		route[goal.x][goal.y] = 0;
		tocheck = std::vector<std::vector<bool>>(c - 3, std::vector<bool>(r - 3, false));
		q.push(goal + Vei2(-1, 0)); tocheck[goal.x - 1][goal.y] = true;
		q.push(goal + Vei2(0, -1)); tocheck[goal.x][goal.y - 1] = true;
		q.push(goal + Vei2(1, 0)); tocheck[goal.x + 1][goal.y] = true;
	}
	MapInterpretation()
	{
		(*this) = MapInterpretation(60, 60, Vei2(28, 56));
	};
	void SeekRoutesWithQueue();
public:
	int r, c;
	std::vector<std::vector<int>> map;
	Vei2 goal;
	std::vector<std::vector<int>> route;
	// Taking the below queue variable here we can split calculaitons between frames
	std::queue<Vei2> q;
	std::vector<std::vector<bool>> tocheck;

};