#pragma once
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include "strategy.h"

struct b2Vec2 {
	double x;
	double y;

	double Length()
	{
		return sqrt(x * x + y * y);
	}
};

struct speed
{
	// Token: 0x04000040 RID: 64
	b2Vec2 v;

	// Token: 0x04000041 RID: 65
	double angle;
};

struct MyParams
{

	// Token: 0x0400003B RID: 59
	double vx;

	// Token: 0x0400003C RID: 60
	double vy;

	// Token: 0x0400003D RID: 61
	double w;

	// Token: 0x0400003E RID: 62
	double r1;

	// Token: 0x0400003F RID: 63
	double r2;
};
namespace search{ 
	std::vector<double> FindSolution(double x, double y, double vx, double vy, int rank[], const GAMESTATE* const gs); 
	std::vector<double> TuneOffset(std::vector<double> input, std::vector<double> destination);
	int CheckCollided(std::vector<double> input, std::vector<double> destination, int rank[], const GAMESTATE* const gs);
	int CheckCollidedForSending(std::vector<double> input, std::vector<double> destination, int rank[], const GAMESTATE* const gs);
	std::vector<double> sendToDes(std::vector<double> destination);
	std::vector<double> sendToDesStraight(std::vector<double> destination);
	std::vector<double> sendToMidStraight(std::vector<double> destination, bool strength);
}
