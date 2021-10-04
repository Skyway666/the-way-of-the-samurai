#pragma once

// Virtual class with base structure for any module in the app
class Module
{
public:
	// Virtual methods
	virtual bool Init() { return true; }
	virtual bool Start() { return true; }
	virtual bool Update() { return true; }
	virtual bool CleanUp() { return true; }
};

