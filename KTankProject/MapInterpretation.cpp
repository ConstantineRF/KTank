#include "MapInterpretation.h"
void MapInterpretation::SeekRoutes()
{
	int rightcost, leftcost, upcost, downcost, leftrightcost, updowncost, movecost;
			for (int i = 0; i < c-3; i++)
		{
			route[i].resize(r-3);
			for (int j = 0; j < r-3; j++)
				route[i][j] = 1000000;
		}

	route[goal.x][goal.y] = 0;
	for (int counter = 0; counter < 10; counter++)
	{
		for (int i = 0; i < c - 3; i++)
			for (int j = 0; j < r - 3; j++)
			{
				rightcost = (i<c-4) ? (route[i+1][j] + map[i + 4][j] + map[i + 4][j + 1] + map[i + 4][j + 2] + map[i + 4][j + 3]) : 1000000;
				leftcost = (i>0) ? (route[i-1][j] + map[i - 1][j] + map[i - 1][j + 1] + map[i - 1][j + 2] + map[i - 1][j + 3]) : 1000000;
				upcost = (j>0) ? (route[i][j-1] + map[i][j - 1] + map[i + 1][j - 1] + map[i + 2][j - 1] + map[i + 3][j - 1]) : 1000000;
				downcost = (j<r-4) ? (route[i][j+1] + map[i][j + 4] + map[i + 1][j + 4] + map[i + 2][j + 4] + map[i + 3][j + 4]) : 1000000;
				leftrightcost = rightcost < leftcost ? rightcost : leftcost;
				updowncost = upcost < downcost ? upcost : leftcost;
				movecost = leftrightcost < updowncost ? leftrightcost : updowncost;
				route[i][j] = route[i][j] < movecost ? route[i][j] : movecost;
			}
	}
}

void MapInterpretation::SeekRoutesWithQueue()
{
	std::queue<Vei2> q;
	std::vector<std::vector<bool>> tocheck(c-3, std::vector<bool>(r-3, false));
	Vei2 f;
	int i, j, rightcost, leftcost, upcost, downcost, leftrightcost, updowncost, movecost;
	bool improved, degraded;
	route[goal.x][goal.y] = 0;
	q.push(goal + Vei2(-1, 0)); tocheck[goal.x - 1][goal.y] = true;
	q.push(goal + Vei2(0, -1)); tocheck[goal.x][goal.y - 1] = true;
	q.push(goal + Vei2(1, 0)); tocheck[goal.x + 1][goal.y] = true;
	// and one more random point
	int zi = rand() % 57;
	int zj = rand() % 57;
	q.push(Vei2(zi, zj)); tocheck[zi][zj] = true;
	int counter = 0;
	while ((counter < 30000) && (!q.empty()))
	{
		i = q.front().x;
		j = q.front().y;
		// calculate effort to get to eagle if going in various directions
		/*
		rightcost = (i < (c - 4)) ? (route[i + 1][j] + map[i + 4][j] + map[i + 4][j + 1] + map[i + 4][j + 2] + map[i + 4][j + 3]) : 1000000;  // if go right
		leftcost = (i > 0) ? (route[i - 1][j] + map[i - 1][j] + map[i - 1][j + 1] + map[i - 1][j + 2] + map[i - 1][j + 3]) : 1000000;
		upcost = (j > 0) ? (route[i][j - 1] + map[i][j - 1] + map[i + 1][j - 1] + map[i + 2][j - 1] + map[i + 3][j - 1]) : 1000000;
		downcost = (j < (r - 4)) ? (route[i][j + 1] + map[i][j + 4] + map[i + 1][j + 4] + map[i + 2][j + 4] + map[i + 3][j + 4]) : 1000000;
		*/
		rightcost = (i < (c - 4)) ? (route[i + 1][j] + map[i][j] + map[i][j + 1] + map[i][j + 2] + map[i][j + 3]) : 1000000;  // from right
		leftcost = (i > 0) ? (route[i - 1][j] + map[i + 3][j] + map[i + 3][j + 1] + map[i + 3][j + 2] + map[i + 3][j + 3]) : 1000000;  // from left
		upcost = (j > 0) ? (route[i][j - 1] + map[i][j + 3] + map[i + 1][j + 3] + map[i + 2][j + 3] + map[i + 3][j + 3]) : 1000000;  // from up 
		downcost = (j < (r - 4)) ? (route[i][j + 1] + map[i][j] + map[i + 1][j] + map[i + 2][j] + map[i + 3][j]) : 1000000;  // from down
		leftrightcost = (rightcost < leftcost) ? rightcost : leftcost;
		updowncost = (upcost < downcost) ? upcost : downcost;
		movecost = (leftrightcost < updowncost) ? leftrightcost : updowncost;
		improved = route[i][j] > movecost;
		degraded = route[i][j] < movecost;
		q.pop(); tocheck[i][j] = false;
		if (improved)
		{
			route[i][j] = movecost;
			if (i > 0) { if (tocheck[i - 1][j] == false) { q.push(Vei2(i - 1, j)); tocheck[i - 1][j] = true; } };
			if (i < c - 4) { if (tocheck[i + 1][j] == false) { q.push(Vei2(i + 1, j)); tocheck[i + 1][j] = true; } };
			if (j > 0) { if (tocheck[i][j - 1] == false) { q.push(Vei2(i, j - 1)); tocheck[i][j - 1] = true; } };
			if (j < r - 4) { if (tocheck[i][j + 1] == false) { q.push(Vei2(i, j + 1)); tocheck[i][j + 1] = true; } };
		}
		/*
		else if (degraded)
		{
			// likely fort was rebuilt, must recalculate from the beginning
			for (int k = 0; k < c - 3; k++)
			{
				route[k].resize(r - 3);
				for (int l = 0; l < r - 3; l++)
				{
					route[k][l] = 1000000;
					tocheck[k][l] = false;
				}
			}
			while (!q.empty())
			{
				q.pop();
			}
			route[goal.x][goal.y] = 0;
			q.push(goal + Vei2(-1, 0)); tocheck[goal.x - 1][goal.y] = true;
			q.push(goal + Vei2(0, -1)); tocheck[goal.x][goal.y - 1] = true;
			q.push(goal + Vei2(1, 0)); tocheck[goal.x + 1][goal.y] = true;
		}
		*/
		counter++;
	}
}


