#pragma once
#include "strategy.h"
#include "search.h"
#include<ctime>


using namespace std;

//  get distance^2 from center of House
float get_dist(float x, float y)
{
	return pow(x - TEE_X, 2) + pow(y - TEE_Y, 2);
}

//  get distance^2 of two cordinates
float get_dist(float x1, float y1, float x2, float y2)
{
	return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}

//  is a Stone in House
bool is_in_House(float x, float y)
{
	if (get_dist(x, y) < pow(HOUSE_R + STONE_R, 2)) {
		return true;
	}
	else {
		return false;
	}
}

double l2cx(double x) {
	return x - 2.355;
}

double l2cy(double y) {
	return 27.6 + 4.88 - y;
}

double c2lx(double x) {
	return x + 2.395;
}

double c2ly(double y) {
	return 27.6 + 4.88 - y;
}
//  sort Shot number (rank[] = {0, 1, 2 ... 15})
//  by distance from center of House (TEEX, TEEY)
void get_ranking(int *rank, const GAMESTATE* const gs)
{
	// init array
	for (int i = 0; i < 16; i++) {
		rank[i] = i;
	}

	// sort
	int tmp;
	for (int i = 1; i < gs->ShotNum; i++) {
		for (int j = i; j > 0; j--) {
			if (get_dist(gs->body[rank[j]][0], gs->body[rank[j]][1]) < get_dist(gs->body[rank[j - 1]][0], gs->body[rank[j - 1]][1])) {
				// swap
				tmp = rank[j];
				rank[j] = rank[j - 1];
				rank[j - 1] = tmp;
			}
			else {
				break;
			}
		}
	}
}
double dis2vec3(double y, double desy) {

	double c = 0.42;
	return sqrt((c*(27.6 + 4.88 - y - 0.29) + c * 6 * (y - desy)));
}
double dis2vec(double y, double desy) {

	double c = 0.4;
	double c1 = 0.35;
	double c2 = 1.6;
	return sqrt((c*(27.6 + 4.88 - y-0.29) + c * 4 * (y - desy)));
}
std::vector<double> bringIn(int rank[], const GAMESTATE* const gs) {

	//cal enemy mcd
	double ecd = 4.88*4.88;
	for (size_t i = 0; i < gs->ShotNum; i++)
	{
		double dx = gs->body[rank[i]][0];
		double dy = gs->body[rank[i]][1];
		if (rank[i] % 2 != gs->ShotNum % 2&&is_in_House(dx,dy)) {
			ecd = get_dist(dx, dy);
			break;
		}
	}
	//ecd is enemy closest distance^2

	//cal mine
	double mcd = 4.88*4.88;
	double mx = 0;
	double my = 0;
	for (size_t i = 0; i < gs->ShotNum; i++)
	{
		double dx = gs->body[rank[i]][0];
		double dy = gs->body[rank[i]][1];
		if (rank[i] % 2 == gs->ShotNum % 2 && dx > 0.52&&dx<4 && dy>4.88&&dy < 14.88) {
			mcd = abs(2.375 - dx)*abs(2.375 - dx);
			mx = dx;
			my = dy;
			break;
		}
	}

	//if there is no my ball, return {0,0,0}
	if (my == 0) return { 0,0,0 };

	//our distance is lower
	//choose to send it to center
	if (mcd < ecd) {	
		double c = 2;
		return{ dis2vec(my,4.88),0,l2cx(mx) };
	}
	else
	{
		//our distance is higher
		//not send it center
		return{ 0,0,0 };
	}
}
//y 2 l axis

bool is_in_Red(float x, float y) {
	if (get_dist(x, y) < pow(0.7916 - STONE_R, 2)) {
		return true;
	}
	else {
		return false;
	}
}

bool is_in_Down_house(double x, double y, double radius) {
	if (get_dist(x, y, TEE_X, TEE_Y + radius) < pow(HOUSE_R + STONE_R, 2)) {
		return true;
	}
	else {
		return false;
	}
}
// make your decision here
void getBestShot(const GAMESTATE* const gs, SHOTINFO* vec_ret)
{
	// ranking of Shot number
	// rank[n] = x;
	//   n : the n th Stone from the center of House
	//   x : the x th Shot in this End (corresponding to the number of GAMESTATE->body[x])
	int rank[16];

	// sort by distance from center
	get_ranking(rank, gs);

	//means u are the first one to shoot
	//put your curling on the middle of way to house
	
	/*vec_ret->speed = 3.1f-0.05*gs->ShotNum;
	vec_ret->h_x = 0;
	vec_ret->angle = 0;
	return;*/

	//straight line
	/*double of = -0.5;
	if (gs->ShotNum > 5) {
		vec_ret->speed = 3.2 - (gs->ShotNum-6) / 2 * 0.1;
		vec_ret->h_x = of;
		vec_ret->angle = 0;
		return;
	}
	vec_ret->speed = 3.2 -  gs->ShotNum/2*0.1;
	vec_ret->h_x = -of;
	vec_ret->angle = 0;*/

	/*
	if (Math.Abs(angle) <= 9.9999999747524271E-07)
			{
				angle = 0.0099999997764825821;
			}
	*/
	//give destination and test search accuracy
	srand(unsigned(std::time(0)));
	//first ball
	if (gs->ShotNum%2 == 0) {
		if (gs->ShotNum == 0) {
			vec_ret->speed = 3;
			vec_ret->h_x = 0;
			vec_ret->angle = 0;
			return;
		}
		if (gs->ShotNum == 14) {
			//my last ball

			//find enemy's closest ball's distance
			double dist = 4.88*4.88;
			for (size_t i = 0; i < gs->ShotNum; i++)
			{
				if (rank[i] % 2 != gs->ShotNum % 2) {
					dist = get_dist(gs->body[rank[i]][0], gs->body[rank[i]][1]);
				}
			}


			//one of my ball is out of the house but to center
			for (size_t i = 0; i < gs->ShotNum; i++)
			{
				if (rank[i] % 2 == gs->ShotNum % 2 && gs->body[rank[i]][0] - 2.375<0.2&&gs->body[rank[i]][0] - 2.375>-0.2 && !is_in_House(gs->body[rank[i]][0], gs->body[rank[i]][1]) && (gs->body[rank[i]][1] > 4.88 + 1.27) && dist > gs->body[rank[i]][0] * gs->body[rank[i]][0]) {
					//exist the ball
					vec_ret->speed = dis2vec(gs->body[rank[i]][1], 4.88);
					vec_ret->h_x = gs->body[rank[i]][0] - 2.3506;
					vec_ret->angle = 0;
					return;
				}
			}

		}

		//attacking policy
		for (int j = 0;j < gs->ShotNum;j++) {
			if (is_in_House(gs->body[rank[j]][0], gs->body[rank[j]][1]) && rank[j] % 2 != gs->ShotNum % 2) {
				//then the first curling is in the house
				//kill it
				//for single mode


				double mx = 0, my = 0;
				bool flag = false;
				for (int k = 0;k < gs->ShotNum;k++) {
					if (rank[k] % 2 == gs->ShotNum % 2) {
						//my ball in the house is ahead an enemy's ball
						if (gs->body[rank[j]][0] - gs->body[rank[k]][0]<0.28&&gs->body[rank[j]][0] - gs->body[rank[k]][0]>-0.28&&gs->body[rank[k]][1] < gs->body[rank[j]][1] && is_in_House(gs->body[rank[k]][0], gs->body[rank[k]][1])) {
							flag = true;
							mx = gs->body[rank[k]][0];
							my = gs->body[rank[k]][1];
							break;
						}
					}
				}
				if (flag) {
					//means the ball ahead exists one of my ball
					//attack the ball without hurting my ball

					//first cal the best position
					double angle2 = 0;
					angle2 = atan((gs->body[rank[j]][1] - my) / (mx - gs->body[rank[j]][0]));

					//use angle to filter
					//if angle is too small choose to hit straight, we give up the ball ahead
					if (angle2 + 3.1415926 / 2 > 3.1415926 / 5 && 3.1415926 / 2 - angle2 > 3.1415926 / 5) {
						double bestx = gs->body[rank[j]][0] + sin(angle2)*0.25;
						double besty = gs->body[rank[j]][1] + cos(angle2)*0.29;

						double targetX = bestx;
						double targetY = besty;

						//no check obstacle
						vec_ret->speed = dis2vec3(besty, 4.88 - 1.87);
						vec_ret->h_x = bestx - 2.3506;
						vec_ret->angle = 0;
						return;

						//reaching here means
						//we cannot hit the ball by curling




					}
					else {
						//our ahead ball is quite straight to the enemy's ball
						//no check obstacle
						vec_ret->speed = dis2vec3(gs->body[rank[j]][1], 4.88 - 1.87);
						vec_ret->h_x = gs->body[rank[j]][0] - 2.3506;
						vec_ret->angle = 0;
						return;
					}


					//continue;
				}

				double targetX = gs->body[rank[j]][0];
				double targetY = gs->body[rank[j]][1];
				//try to curling hit
				for (size_t i = j + 1; i < gs->ShotNum; i++)
				{
					if (gs->body[rank[i]][1] == 0) continue;
					if (gs->body[rank[i]][0] - targetX<0.29 && gs->body[rank[i]][0] - targetX > -0.29&&gs->body[rank[i]][1] > targetY) {
						//other ball in the way
						double x = targetX - 2.375;
						double y = -targetY + 4.88 - 0.29 + 27.6;
						if (get_dist(gs->body[rank[i - 1]][0], gs->body[rank[i - 1]][0], gs->body[rank[i]][0], gs->body[rank[i]][1]) < 0.1024&&rank[i] % 2 == gs->ShotNum % 2) {
							x = gs->body[rank[i]][0] - 2.375;
							y = -gs->body[rank[i]][1] + 4.88 - 0.29 + 27.6;
						}
						for (size_t k = 0; k < 15; k++)
						{
							double offset = k * 0.01;
							std::vector<double> input = { 6.0f,-10.0f ,+1.0f };
							std::vector<double> output = search::TuneOffset(input, { x + offset,y });
							if (search::CheckCollided(output, { x + offset,y }, rank, gs) == 1 || output[2] > 2.33) {
								input = { 6.0f,10.0f,-1.0f };
								output = search::TuneOffset(input, { x - offset,y });
								if (search::CheckCollided(output, { x - offset,y }, rank, gs) == 1 || output[2] < -2.33) {
									continue;
								}
								vec_ret->speed = output[0];
								vec_ret->h_x = output[2];
								vec_ret->angle = output[1];
								return;
							}
							vec_ret->speed = output[0];
							vec_ret->h_x = output[2];
							vec_ret->angle = output[1];
							return;
						}

					}
				}

				//hit straight and with la
				//target is the ball i need to hit
				bool strength = true;
				double tempX = targetX;
				double tempY = targetY;
				double angle = 0;
				for (size_t i = j + 1; i < gs->ShotNum; i++)
				{
					//not an available ball
					if (gs->body[rank[i]][1] == 0) continue;
					if (tempX - gs->body[rank[i]][0]<0.29&&tempX - gs->body[rank[i]][0]>-0.29) {
						//get close in x axis means they can hit 
						if (gs->body[rank[i]][1] > targetY) {
							//make sure ball is under target on y axis
							tempX = gs->body[rank[i]][0];
							tempY = gs->body[rank[i]][1];
							angle = atan((targetX - tempX) / (tempY - targetY));
							if (angle<-3.1415926 / 4 || angle>3.1415926 / 4) {
								//angle is too large try to hit the middle of two balls
								tempX = (targetX + tempX) / 2;
								angle = 0;
								break;
							}
							else {
								//normal ball 
								//if my ball is the bullet should not too fast
								// enemy ball is the bullet should be fast
								if (rank[i] % 2 == gs->ShotNum % 2) {
									//my ball is bullet 
									strength = false;
								}
							}
							break;
						}
					}
				}
				//how many balls on the same line of target
				int hbnum = 1;
				double mtdis = get_dist(mx, my, targetX, targetY);
				if (mtdis < 0.085264) {
					hbnum = 2;
				}

				//straight shoot without other ball then try to be in center
				if (tempX == targetX && tempY == targetY && my == 0) {
					if (tempX < 2.3991) {
						tempX += pow(2.3991 - tempX, 1.0 / 3)*0.025;
					}
					else
					{
						tempX -= pow(-2.3991 + tempX, 1.0 / 3)*0.025;
					}
				}
				//cal theta
				double ex = tempX - sin(angle)*0.29;
				double ey = tempY;
				//compare 

				std::vector<double> destination = { ex,ey };
				//format 
				//destination[0] = destination[0] - 2.3494;
				destination[0] = destination[0] - 2.355;
				destination[1] = -destination[1] + 27.6 + 4.88;
				//to search function
				destination[0] = destination[0];
				destination[1] = destination[1];
				//std::vector<double> output = search::sendToDesStraight(destination);
				std::vector<double> output = search::sendToMidStraight(destination, strength, hbnum);
				//execute
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				return;
			}
		}
		
	}
	//enemy's ball is in house
	//search every ball 

	//second player
	if (gs->ShotNum == 1) {
		vec_ret->speed = 2.41;
		vec_ret->h_x = 0;
		vec_ret->angle = 0;
		return;
	}
	//bool flag = true;
	//for (size_t i = 0; i < gs->ShotNum; i++)
	//{
	//	//enemy ball in house
	//	if (gs->body[rank[i]][0]>2.3&&gs->body[rank[i]][0]<2.45&&gs->body[rank[i]][1]>10&& gs->body[rank[i]][1]<11) {
	//		//try to bring to end
	//		flag = false;
	//		break;
	//		
	//	}
	//}
	//if (flag) {
	//	vec_ret->speed = 2.41;
	//	vec_ret->h_x = 0;
	//	vec_ret->angle = 0;
	//	return;
	//}

	//hit enemy's ball
	for (size_t i = 0; i < gs->ShotNum; i++)
	{
		//enemy ball in house
		if (rank[i] % 2 != gs->ShotNum % 2 && is_in_House(gs->body[rank[i]][0], gs->body[rank[i]][1])) {
			//try to bring to end
			vec_ret->speed = dis2vec(gs->body[rank[i]][1], 4.88 );
			vec_ret->h_x = gs->body[rank[i]][0] - 2.35060;
			vec_ret->angle = 0;
			return;
		}
	}
	//no eneymy's ball is in house
	//try to bring my ball in
	//remember check hit
	std::vector<double> output = bringIn(rank, gs);
	if (output[0] != 0) {
		vec_ret->speed = output[0];
		vec_ret->h_x = output[2];
		vec_ret->angle = output[1];
		return;
	}
	
	if(rank[0] % 2 == gs->ShotNum % 2&&is_in_House(gs->body[rank[0]][0], gs->body[rank[0]][1])){
		//check hit
		vec_ret->speed = dis2vec(gs->body[rank[0]][1],4.88);
		vec_ret->h_x = gs->body[rank[0]][0] - 2.3506;
		vec_ret->angle = 0;
		return;
	}
	else {
		//check hit
		vec_ret->speed = 3.0;
		vec_ret->h_x = 0;
		vec_ret->angle = 0;
		return;
	}

	//std::vector<double> destination = { 2.375-(gs->ShotNum%2==0?0.4*(gs->ShotNum/2+1): -0.4*(gs->ShotNum / 2 + 1)),6 };
	//std::vector<double> destination = { gs->body[rank[0]][0],gs->body[rank[0]][1]+0.29 };
	////format 
	////destination[0] = destination[0] - 2.3494;
	//destination[0] = destination[0] - 2.355;
	//destination[1] = -destination[1] + 27.6 + 4.88;
	////to search function
	//destination[0] = destination[0];
	//destination[1] = destination[1];
	////std::vector<double> output = search::sendToDesStraight(destination);
	//std::vector<double> output = search::sendToMidStraight(destination);
	////execute
	//vec_ret->speed = output[0];
	//vec_ret->h_x = output[2];
	//vec_ret->angle = output[1];
	//return;


	/*
	if (gs->ShotNum % 2 == 0) {
		//vec_ret->speed = 3.2 - (gs->ShotNum ) / 2 * 0.1;
		vec_ret->speed = 3;
		vec_ret->h_x = -2+gs->ShotNum/2 *0.5;
		vec_ret->angle = 0;
		return;
	}*/

	//use curling to hit the ball
	/*
	for (size_t i = 1; i < gs->ShotNum; i++)
	{
		if (gs->body[rank[i]][0] - gs->body[rank[0]][0]<0.29 && gs->body[rank[i]][0] - gs->body[rank[0]][0] > -0.29&&gs->body[rank[i]][1]> gs->body[rank[0]][1]) {
			//other ball in the way
			double x = gs->body[rank[0]][0] - 2.375;
			double y = -gs->body[rank[0]][1] + 4.88 - 0.29 + 27.6;
			if (get_dist(gs->body[rank[i - 1]][0], gs->body[rank[i - 1]][0], gs->body[rank[i]][0], gs->body[rank[i]][1]) < 0.1024&&rank[i]%2==gs->ShotNum%2) {
				x = gs->body[rank[i]][0] - 2.375;
				y = -gs->body[rank[i]][1] + 4.88 - 0.29 + 27.6;
			}
			std::vector<double> input = { 6.0f,-10.0f ,+1.0f };
			std::vector<double> output = search::TuneOffset(input, { x+0.14,y });
			if (search::CheckCollided(output, { x+0.14,y }, rank, gs) == 1||output[2]>2.33) {
				input = { 6.0f,10.0f,-1.0f };
				output = search::TuneOffset(input, { x-0.14,y });
				if (search::CheckCollided(output, { x-0.14,y }, rank, gs) == 1 || output[2] < -2.33) {
					input = { 4.0f,10.0f,-1.5f };
					output = search::TuneOffset(input, { x,y });
					if (search::CheckCollided(output, { x,y }, rank, gs) == 1 || output[2] < -2.33) {
						break;
					}
					vec_ret->speed = output[0];
					vec_ret->h_x = output[2];
					vec_ret->angle = output[1];
					return;
				}
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				return;
			}
			vec_ret->speed = output[0];
			vec_ret->h_x = output[2];
			vec_ret->angle = output[1];
			return;
		}
	}
	*/

	/*
	//use straight to hit the ball
	bool strength = true;
	double targetX = gs->body[rank[0]][0];
	double targetY = gs->body[rank[0]][1];
	double tempX = targetX;
	double tempY = targetY;
	double angle = 0;
	for (size_t i = 1; i < gs->ShotNum; i++)
	{
		//shoot around
		if (gs->body[rank[i]][1] > targetY&&rank[i] % 2 == gs->ShotNum % 2) {
			//my ball in zone
			//if (!is_in_House(gs->body[rank[i]][0], gs->body[rank[i]][1]) && gs->body[rank[i]][1] < 7.2777&&abs(gs->body[rank[i]][0]-2.375)<1.054) {
			//	//my ball is in the zone

			//	if (get_dist(targetX, targetY) > 2.5069) {
			//		if (gs->body[rank[i]][0] < 2.375) {
			//			tempX = gs->body[rank[i]][0] + 0.14;
			//			strength = false;
			//			break;
			//		}
			//		else {
			//			tempX = gs->body[rank[i]][0] - 0.14;
			//			strength = false;
			//			break;
			//		}
			//	}
			//	
			//}

		}
		if (tempX - gs->body[rank[i]][0]<0.29&&tempX - gs->body[rank[i]][0]>-0.29) {
			//get close in x axis means they can hit 
			if (gs->body[rank[i]][1] > targetY) {
				//make sure ball is under target on y axis
				tempX = gs->body[rank[i]][0];
				tempY = gs->body[rank[i]][1];
				angle = atan((gs->body[rank[0]][0] - tempX) / (tempY - gs->body[rank[0]][1]));
				if (angle<-3.1415926 / 5 || angle>3.1415926 / 5) {
					//angle is too large try to hit the middle of two balls
					tempX = (targetX + tempX) / 2;
					angle = 0;
					if(rank[i]%2==gs->ShotNum%2)
					strength = false;
					break;
				}
				break;
			}
			
			
		}
		
	}
	//cal theta
	double ex = tempX-sin(angle)*0.29;
	double ey = tempY;
	//compare 
	
	std::vector<double> destination = { ex,ey };
	//format 
	//destination[0] = destination[0] - 2.3494;
	destination[0] = destination[0] - 2.3506;
	destination[1] = -destination[1] + 32.5074-0.4;
	//to search function
	destination[0] = destination[0];
	destination[1] = destination[1];
	//std::vector<double> output = search::sendToDesStraight(destination);
	std::vector<double> output = search::sendToMidStraight(destination,strength);
	//execute
	vec_ret->speed = output[0];
	vec_ret->h_x = output[2];
	vec_ret->angle = output[1];
	return;
	*/

	//if my ball stops my way to hit


	//random in house
	//speed from 2.9 to 3.1
	//angular from -5,5
	//rand()%200
	
	/*srand(unsigned(std::time(0)));
	vec_ret->speed = 2.8 + rand() % 400 / 1000.0;
	vec_ret->h_x = 0;
	vec_ret->angle = -5+rand()%1000/100.0;*/

	//hitting test
	/*if (gs->ShotNum % 2 == 0) {
		vec_ret->speed = 3.0f;
		vec_ret->h_x = 0;
		vec_ret->angle = 0;
	}
	else {
		vec_ret->speed = 5.0f+gs->ShotNum/2;
		vec_ret->h_x = 0;
		vec_ret->angle = 0;
	}*/
	return;
}