#include "ContainerUtils.h"

bool ContainerUtils::StringVectorAContainsB(vector<string>& a, vector<string>& b)
{
	bool ret = true;
	for (string& str : b)
	{
		if (find(a.begin(), a.end(), str) == a.end())
		{
			ret = false;
			break;
		}
	}

	return ret;
}
