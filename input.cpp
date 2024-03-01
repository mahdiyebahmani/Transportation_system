#include <iostream>
#include <string>
#include <queue>
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
}