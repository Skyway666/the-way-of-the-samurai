#pragma once
#include <vector>
#include <string>
using namespace std;

// Class with methods to compare containers 
// (this is probably already supported by some standard library, but I don't find it and it's pretty simple)
class ContainerUtils
{
public:
	static bool stringVectorAContainsB(vector<string>& a, vector<string>& b);
};
