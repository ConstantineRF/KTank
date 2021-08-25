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
		map.resize(c);
		for (int i = 0; i < c; i++)
		{
			map[i].resize(r);
			for (int j = 0; j < r; j++)
				map[i][j] = 0;
		}
		route.resize(c-3);
		for (int i = 0; i < c-3; i++)
		{
			route[i].resize(r-3);
			for (int j = 0; j < r-3; j++)
				route[i][j] = 1000000;
		}
	}
	MapInterpretation()
	{
		(*this) = MapInterpretation(60, 60, Vei2(28, 56));
	};
	void SeekRoutes();
	void SeekRoutesWithQueue();
public:
	int r, c;
	std::vector<std::vector<int>> map;
	Vei2 goal;
	std::vector<std::vector<int>> route;
};