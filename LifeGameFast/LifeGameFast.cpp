// LifeGameFast.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "WorldContext.h"
#include "WindowManager.h"


int main(int argc, char *argv[])
{
	WindowManager::getInstance()->init();

	WorldContext worldContext1(200, 200);
	WorldContext worldContext2(200, 200);
	WorldContext worldContext3(200, 200);

	WindowManager::getInstance()->startLoop();
	// never reaches here

	return 0;
}
