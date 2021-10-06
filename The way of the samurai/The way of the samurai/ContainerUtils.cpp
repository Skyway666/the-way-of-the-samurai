#include "ContainerUtils.h"

bool ContainerUtils::stringVectorAContainsB(vector<string>& a, vector<string>& b)
{
	bool ret = true;
	for (vector<string>::iterator it = a.begin(); it != a.end(); it++)
	{
		if (find(b.begin(), b.end(), (*it)) == b.end())
		{
			ret = false;
			break;
		}
	}

	return ret;
}
