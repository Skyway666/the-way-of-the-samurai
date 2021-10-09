#include "ContainerUtils.h"

bool ContainerUtils::stringVectorAContainsB(vector<string>& a, vector<string>& b)
{
	bool ret = true;
	for (vector<string>::iterator it = b.begin(); it != b.end(); it++)
	{
		if (find(a.begin(), a.end(), (*it)) == a.end())
		{
			ret = false;
			break;
		}
	}

	return ret;
}
