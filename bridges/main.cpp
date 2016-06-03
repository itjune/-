#include <vector>
#include <algorithm> 
#include <iostream>

long int min_cost = 1000000001;
int timer = 0;

struct cost 
{
	int dest;
	long int money;
};

void dfs (std::vector<std::vector<cost> > &road_map, 
			std::vector<int> &used,
			std::vector<int> &tin,
			std::vector<int> &low,
			int v, int parent = -1)
{
	used[v] = 1;
	++timer;
	tin[v] = low[v] = timer;
	for (int i = 0; i < road_map[v].size(); ++i)
	{
		int to = road_map[v][i].dest;
		long int to_cost = road_map[v][i].money;
		if (to != parent)
		{
			if (used[to])
				low[v] = std::min(low[v], tin[to]);
			else
			{
				dfs(road_map, used, tin, low, to, v);
				low[v] = std::min(low[v], low[to]);
				if ((low[to] > tin[v]) and (to_cost < min_cost))
					min_cost = to_cost;
			}
		}
	}
 }

int main()
{
    int n, m;
	std::cin >> n >> m;
	std::vector<std::vector<cost> > road_map(n, std::vector<cost>());
	std::vector<int> tin (n, 0);
	std::vector<int> low (n, 0);
	std::vector<int> used (n, 0);
	int a, b;
	long int c;
	for (int i = 0; i < m; ++i)
	{
		std::cin >> a >> b >> c;
		struct cost tmp;
		tmp.dest = b - 1;
		tmp.money = c;
		road_map[a-1].push_back(tmp);
		tmp.dest = a - 1;
		road_map[b-1].push_back(tmp);
	}
	for (int i = 0; i < n; ++i)
	{
		if (!used[i])
			dfs(road_map, used, tin, low, i);
	}
	if (min_cost == 1000000001)
		std::cout << -1;
	else
		std::cout << min_cost;
	return 0;
}