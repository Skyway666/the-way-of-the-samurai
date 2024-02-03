#pragma once
#include<string>
using namespace std;

#define MAX_INVALID_CHARS 1
// Virtual class with base structure for any module in the app
class Module
{
public:
	// Virtual methods
	virtual void Init() { }
	virtual void Start() {  }
	virtual void Update() { }
	virtual void CleanUp() { }

public:
	string name = "Module";
};

