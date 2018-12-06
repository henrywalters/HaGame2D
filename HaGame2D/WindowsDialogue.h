#pragma once
#include <iostream>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

class WindowsDialogue
{
public:
	WindowsDialogue();
	~WindowsDialogue();

	static char * load();
	static char * save();
};

