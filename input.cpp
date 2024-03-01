#include <iostream>
#include <string>
#include <queue>
#include "brain.cpp"
using namespace std;
void input()
{
	queue<string> input;
	string n;
	for(int i = 0; i < 3;i++)
	{
		cin >> n;
		input.push(n);
	}
	dijkstra(input.front(),input.back() ,input[1]);
}