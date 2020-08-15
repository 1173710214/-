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

//  is in the red zone
bool is_in_Red(float x, float y) {
	if (get_dist(x, y) < pow(0.7916 - STONE_R, 2)) {
		return true;
	}
	else {
		return false;
	}
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

bool is_in_Down_house(double x, double y, double radius) {
	if (get_dist(x, y, TEE_X, TEE_Y+radius) < pow(HOUSE_R + STONE_R, 2)) {
		return true;
	}
	else {
		return false;
	}
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
double dis2vec3(double y, double desy) {

	double c = 0.4;
	return sqrt((c*(27.6 + 4.88 - y - 0.29) + c * 6 * (y - desy)));
}
double dis2vec(double y, double desy) {

	double c = 0.4;
	double c1 = 0.35;
	double c2 = 1.6;
	return sqrt((c*(27.6 + 4.88 - y) + c * 4 * (y - desy)));
}
std::vector<double> bringIn(int rank[], const GAMESTATE* const gs) {

	//cal enemy mcd
	double ecd = 4.88*4.88;
	for (size_t i = 0; i < gs->ShotNum; i++)
	{
		double dx = gs->body[rank[i]][0];
		double dy = gs->body[rank[i]][1];
		if (rank[i] % 2 != gs->ShotNum % 2 && is_in_House(dx, dy)) {
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
			//already find a ball in the zone
			bool flag = true;
			//find a ball in the way
			for (size_t j = 0; j < gs->ShotNum; j++)
			{
				if (j!=i && gs->body[rank[j]][0] - dx<0.25&&gs->body[rank[j]][0] - dx>-0.25&&gs->body[rank[j]][1] > 4.88) {
					flag = false;
					break;
				}
			}
			//if the way is stopped by an enemy ball
			if (flag) {
				mcd = abs(2.375 - dx)*abs(2.375 - dx);
				mx = dx;
				my = dy;
				break;
			}
			
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

	//if (gs->ShotNum == 0) {
	//	//first ball
	//	//to front 
	//	vec_ret->speed = 2.775f;
	//	vec_ret->h_x = 0;
	//	vec_ret->angle = 0;
	//	return;

	//}

	//first player's policy
	if (gs->ShotNum % 2 == 0) {
		//search every ball to send to end
		if (gs->ShotNum <= 3) {

			//if there exists enemy obstacle ball
			//if is close enough send it to end
			//if not close enough send a ball to ahead

			for (size_t i = 0; i < gs->ShotNum; i++)
			{
				bool flag = false;
			
				// enemy ball not in house and close
				
				if (rank[i] % 2 != gs->ShotNum % 2&&!is_in_House(gs->body[rank[i]][0], gs->body[rank[i]][1]))
				{
					//enemy's ball is not so far
					//send it to line
					//sending policy after hitting fail or not needed
					//already ball in red zone
					if (is_in_Red(gs->body[rank[0]][0], gs->body[rank[0]][1])) {
						//choose a nearby place then get shooting configuration
						double cx = gs->body[rank[0]][0];
						double cy = gs->body[rank[0]][1];

						//choose two places once a time(changed to shooting configuration)
						std::vector<double> leftPos = { l2cx(gs->body[rank[0]][0]),l2cy(gs->body[rank[0]][1]) };
						std::vector<double> rightPos = { l2cx(gs->body[rank[0]][0]),l2cy(gs->body[rank[0]][1]) };

						for (int i = 0;i < 4;i++) {
							if (i != 0) {
								leftPos[0] -= 0.60;
								rightPos[0] += 0.60;
							}
							std::vector<double> output = search::sendToDesStraight(leftPos);
							if (output[2] > -2.29&&search::CheckCollidedForSending(output, leftPos, rank, gs) == 0) {
								vec_ret->speed = output[0];
								vec_ret->h_x = output[2];
								vec_ret->angle = output[1];
								return;
							}
							output = search::sendToDesStraight(rightPos);
							if (output[2] > -2.29&&search::CheckCollidedForSending(output, rightPos, rank, gs) == 0) {
								vec_ret->speed = output[0];
								vec_ret->h_x = output[2];
								vec_ret->angle = output[1];
								return;
							}
							output = search::sendToDes(leftPos);
							if (output[2] > -2.29&&search::CheckCollidedForSending(output, leftPos, rank, gs) == 0) {
								vec_ret->speed = output[0];
								vec_ret->h_x = output[2];
								vec_ret->angle = output[1];
								return;
							}
							output = { output[0],-output[1],2 * leftPos[0] - output[2] };
							if (output[2] < 2.29&&search::CheckCollidedForSending(output, leftPos, rank, gs) == 0) {
								vec_ret->speed = output[0];
								vec_ret->h_x = output[2];
								vec_ret->angle = output[1];
								return;
							}
							output = search::sendToDes(rightPos);
							if (output[2] > -2.29&&search::CheckCollidedForSending(output, rightPos, rank, gs) == 0) {
								vec_ret->speed = output[0];
								vec_ret->h_x = output[2];
								vec_ret->angle = output[1];
								return;
							}
							output = { output[0],-output[1],2 * rightPos[0] - output[2] };
							if (output[2] < 2.29&&search::CheckCollidedForSending(output, rightPos, rank, gs) == 0) {
								vec_ret->speed = output[0];
								vec_ret->h_x = output[2];
								vec_ret->angle = output[1];
								return;
							}
						}
					}
				}
				//enemy ball in house
				else if (rank[i] % 2 != gs->ShotNum % 2 && is_in_House(gs->body[rank[i]][0], gs->body[rank[i]][1])) {
					//check close
					double mx = 0, my = 0;
					bool flag = false;
					for (int k = 0;k < gs->ShotNum;k++) {
						if (rank[k] % 2 == gs->ShotNum % 2) {
							//my ball in the house is ahead an enemy's ball
							if (gs->body[rank[i]][0] - gs->body[rank[k]][0]<0.28&&gs->body[rank[i]][0] - gs->body[rank[k]][0]>-0.28&&gs->body[rank[k]][1] < gs->body[rank[i]][1] && is_in_House(gs->body[rank[k]][0], gs->body[rank[k]][1])) {
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
						angle2 = atan((gs->body[rank[i]][1] - my) / (mx - gs->body[rank[i]][0]));

						//use angle to filter
						//if angle is too small choose to hit straight, we give up the ball ahead
						if (angle2 + 3.1415926 / 2 > 3.1415926 / 5 && 3.1415926 / 2 - angle2 > 3.1415926 / 5) {
							double bestx = gs->body[rank[i]][0] + sin(angle2)*0.25;
							double besty = gs->body[rank[i]][1] + cos(angle2)*0.29;

							double targetX = bestx;
							double targetY = besty;

							//no check obstacle
							vec_ret->speed = dis2vec3(besty, 4.88 - 1.87);
							vec_ret->h_x = bestx - 2.3506;
							vec_ret->angle = 0;
							return;
						}
						else {
							//our ahead ball is quite straight to the enemy's ball
							//no check obstacle
							vec_ret->speed = dis2vec3(gs->body[rank[i]][1], 4.88 - 1.87);
							vec_ret->h_x = gs->body[rank[i]][0] - 2.3506;
							vec_ret->angle = 0;
							return;
						}
						//continue;
					}
					
					//enemy in house
					double targetX = gs->body[rank[i]][0];
					double targetY = gs->body[rank[i]][1];
					//try to curling hit
					for (size_t l = i + 1; l < gs->ShotNum; l++)
					{
						if (gs->body[rank[l]][1] == 0) continue;
						if (gs->body[rank[l]][0] - targetX<0.29 && gs->body[rank[l]][0] - targetX > -0.29&&gs->body[rank[l]][1] > targetY) {
							//other ball in the way
							double x = targetX - 2.375;
							double y = -targetY + 4.88 - 0.29 + 27.6;
							if (get_dist(gs->body[rank[l - 1]][0], gs->body[rank[l - 1]][0], gs->body[rank[l]][0], gs->body[rank[l]][1]) < 0.1024&&rank[l] % 2 == gs->ShotNum % 2) {
								x = gs->body[rank[l]][0] - 2.375;
								y = -gs->body[rank[l]][1] + 4.88 - 0.29 + 27.6;
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
					//target is the ball l need to hit
					bool strength = true;
					double tempX = targetX;
					double tempY = targetY;
					double angle = 0;
					for (size_t l = i + 1; l < gs->ShotNum; l++)
					{
						//not an available ball
						if (gs->body[rank[l]][1] == 0) continue;
						if (tempX - gs->body[rank[l]][0]<0.29&&tempX - gs->body[rank[l]][0]>-0.29) {
							//get close in x axis means they can hit 
							if (gs->body[rank[l]][1] > targetY) {
								//make sure ball is under target on y axis
								tempX = gs->body[rank[l]][0];
								tempY = gs->body[rank[l]][1];
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
									if (rank[l] % 2 == gs->ShotNum % 2) {
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
					

					//straight shoot without other ball then try to be in center
					if (tempX == targetX && tempY == targetY) {
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
			vec_ret->speed = 3.06;
			vec_ret->h_x = -1.45;
			vec_ret->angle = 10;
			return;
		}
		else
		{
			// first player's last ball
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
			for (size_t i = 0; i < gs->ShotNum; i++)
			{
				//enemy ball in house
				//&& is_in_House(gs->body[rank[i]][0], gs->body[rank[i]][1])
				//zone's heart
				if (gs->body[rank[0]][1] == 0)continue;
				if (rank[i] % 2 != gs->ShotNum % 2) {
					//try to bring to end
					double mx = 0, my = 0;
					bool flag = false;
					for (int k = 0;k < gs->ShotNum;k++) {
						if (rank[k] % 2 == gs->ShotNum % 2) {
							//my ball in the house is ahead an enemy's ball
							if (gs->body[rank[i]][0] - gs->body[rank[k]][0]<0.28&&gs->body[rank[i]][0] - gs->body[rank[k]][0]>-0.28&&gs->body[rank[k]][1] < gs->body[rank[i]][1] && is_in_House(gs->body[rank[k]][0], gs->body[rank[k]][1])&&gs->body[rank[k]][1]-gs->body[rank[i]][1]<0.32&&gs->body[rank[k]][1] - gs->body[rank[i]][1] > -0.32) {
								bool flag2 = true;
								for (size_t m = 0; m < gs->ShotNum; m++)
								{
									if (rank[m] % 2 == gs->ShotNum % 2 && gs->body[rank[i]][0] - gs->body[rank[m]][0]<0.28&&gs->body[rank[i]][0] - gs->body[rank[m]][0]>-0.28&&gs->body[rank[m]][1] > gs->body[rank[i]][1] && gs->body[rank[m]][1] - gs->body[rank[i]][1]<0.32&&gs->body[rank[k]][1] - gs->body[rank[i]][1] > -0.32) {
										flag2 = false;
										break;
									}
								}
								if (flag2) {
									flag = true;
									mx = gs->body[rank[k]][0];
									my = gs->body[rank[k]][1];
									break;
								}
							}
						}
					}
					if (flag) {
						//means the ball ahead exists one of my ball
						//attack the ball without hurting my ball
						//first cal the best position
						double angle2 = 0;
						angle2 = atan((gs->body[rank[i]][1] - my) / (mx - gs->body[rank[i]][0]));

						//use angle to filter
						//if angle is too small choose to hit straight, we give up the ball ahead
						if (angle2 + 3.1415926 / 2 > 3.1415926 / 5 && 3.1415926 / 2 - angle2 > 3.1415926 / 5) {
							double bestx = gs->body[rank[i]][0] + sin(angle2)*0.25;
							double besty = gs->body[rank[i]][1] + cos(angle2)*0.29;

							double targetX = bestx;
							double targetY = besty;

							//no check obstacle
							vec_ret->speed = dis2vec3(besty, 4.88 - 1.87);
							vec_ret->h_x = bestx - 2.3506;
							vec_ret->angle = 0;
							return;
						}
						else {
							//our ahead ball is quite straight to the enemy's ball
							//no check obstacle
							vec_ret->speed = dis2vec3(gs->body[rank[i]][1], 4.88 - 1.87);
							vec_ret->h_x = gs->body[rank[i]][0] - 2.3506;
							vec_ret->angle = 0;
							return;
						}
						//continue;
					}

					double targetX = gs->body[rank[i]][0];
					double targetY = gs->body[rank[i]][1];
					//try to curling hit
					for (size_t l = i + 1; l < gs->ShotNum; l++)
					{
						if (gs->body[rank[l]][1] == 0) continue;
						if (gs->body[rank[l]][0] - targetX<0.29 && gs->body[rank[l]][0] - targetX > -0.29&&gs->body[rank[l]][1] > targetY) {
							//other ball in the way
							double x = targetX - 2.375;
							double y = -targetY + 4.88 - 0.29 + 27.6;
							if (get_dist(gs->body[rank[l - 1]][0], gs->body[rank[l - 1]][0], gs->body[rank[l]][0], gs->body[rank[l]][1]) < 0.1024&&rank[l] % 2 == gs->ShotNum % 2) {
								x = gs->body[rank[l]][0] - 2.375;
								y = -gs->body[rank[l]][1] + 4.88 - 0.29 + 27.6;
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
					//target is the ball l need to hit
					bool strength = true;
					double tempX = targetX;
					double tempY = targetY;
					double angle = 0;
					for (size_t l = i + 1; l < gs->ShotNum; l++)
					{
						//not an available ball
						if (gs->body[rank[l]][1] == 0) continue;
						if (tempX - gs->body[rank[l]][0]<0.29&&tempX - gs->body[rank[l]][0]>-0.29) {
							//get close in x axis means they can hit 
							if (gs->body[rank[l]][1] > targetY) {
								//make sure ball is under target on y axis
								tempX = gs->body[rank[l]][0];
								tempY = gs->body[rank[l]][1];
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
									if (rank[l] % 2 == gs->ShotNum % 2) {
										//my ball is bullet 
										strength = false;
									}
								}
								break;
							}
						}
					}
					//how many balls on the same line of target
					int hbnum = 2;


					//straight shoot without other ball then try to be in center
					if (tempX == targetX && tempY == targetY) {
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

	}

	//second player's policy

	//cal how many enemy balls is in house
	int sum = 0;
	for (size_t i = 0; i < gs->ShotNum; i++)
	{
		if (rank[i] % 2 != gs->ShotNum % 2 && is_in_House(gs->body[rank[i]][0], gs->body[rank[i]][1])) {
			sum++;
		}
	}
	if ((gs->ShotNum==15)) {
		//if it's my ball
		if (rank[0] % 2 == gs->ShotNum % 2) {
			std::vector<double> bring = bringIn(rank, gs);
			if (bring[0] != 0) {
				vec_ret->speed = bring[0];
				vec_ret->h_x = bring[2];
				vec_ret->angle = bring[1];
				return;
			}
			//send it to your nearby
			srand(unsigned(std::time(0)));
			int sign = 0;
			if ((rand() % 100) < 50) {
				sign = 1;
			}

			//try to shoot near
			for (int i = 0;i < 9;i++) {
				std::vector<double> centerPos = { 0,27.6 };
				if (i == 0) {
					centerPos[0] = 0;
				}
				else {
					if (sign == 0) {
						centerPos[0] -= (i + 1) / 2 * 0.0725;
						sign = 1;
					}
					else if (sign == 1) {
						centerPos[0] += (i + 1) / 2 * 0.0725;
						sign = 0;
					}

				}

				std::vector<double> input = { 3,0,centerPos[0] };//straight hit
				std::vector<double> output = input;
				if (search::CheckCollidedForSending(output, centerPos, rank, gs) == 1) {
					input = { 3.06f,-10.0f ,1.45f + centerPos[0] };
					output = input;
					if (search::CheckCollidedForSending(output, centerPos, rank, gs) == 1 || output[2] > 2.33) {
						input = { 3.06f,10.0f ,-1.45f + centerPos[0] };
						output = input;
						if (search::CheckCollidedForSending(output, centerPos, rank, gs) == 1 || output[2] < -2.33) {
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
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				return;
			}

			//what if no way to get here?
		}
		else if (sum >= 1) {
			std::vector<double> bring = bringIn(rank, gs);
			if (bring[0] != 0) {
				vec_ret->speed = bring[0];
				vec_ret->h_x = bring[2];
				vec_ret->angle = bring[1];
				return;
			}
			//shoot closely
			double dx = 0, dy = 0;
			srand(unsigned(std::time(0)));
			int sign = 0;
			if ((rand() % 100) < 50) {
				sign = 1;
			}

			//try to shoot near
			for (int i = 0;i < 9;i++) {
				std::vector<double> centerPos = { 0,27.6 };
				if (i == 0) {
					centerPos[0] = 0;
				}
				else {
					if (sign == 0) {
						centerPos[0] -= (i + 1) / 2 * 0.0725;
						sign = 1;
					}
					else if (sign == 1) {
						centerPos[0] += (i + 1) / 2 * 0.0725;
						sign = 0;
					}

				}

				std::vector<double> input = { 3,0,centerPos[0] };//straight hit
				std::vector<double> output = input;
				if (search::CheckCollidedForSending(output, centerPos, rank, gs) == 1) {
					input = { 3.06f,-10.0f ,1.45f + centerPos[0] };
					output = input;
					if (search::CheckCollidedForSending(output, centerPos, rank, gs) == 1 || output[2] > 2.33) {
						input = { 3.06f,10.0f ,-1.45f + centerPos[0] };
						output = input;
						if (search::CheckCollidedForSending(output, centerPos, rank, gs) == 1 || output[2] < -2.33) {
							dx = 0;
							dy = 0;
							continue;
						}
						vec_ret->speed = output[0];
						vec_ret->h_x = output[2];
						vec_ret->angle = output[1];
						dx = centerPos[0];
						dy = centerPos[1];
						break;
					}
					vec_ret->speed = output[0];
					vec_ret->h_x = output[2];
					vec_ret->angle = output[1];
					dx = centerPos[0];
					dy = centerPos[1];
					break;
				}
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				dx = centerPos[0];
				dy = centerPos[1];
				break;
			}

			//judge whether i need to hit, or shoot is good enough
			if (get_dist(gs->body[rank[0]][0], gs->body[rank[0]][1]) > get_dist(c2lx(dx), c2ly(dy))) {
				//shooting near is a better policy
				return;
			}


		}
		//speed h_x angle 
	}

	//find all the balls in  the house center, if is enemy.
	for (int j = 0;j < gs->ShotNum;j++) {
		if (is_in_House(gs->body[rank[j]][0], gs->body[rank[j]][1]) && rank[j] % 2 != gs->ShotNum % 2) {
			//then the first curling is in the house
			//kill it
			//for single mode


			if (rank[j] % 2 != gs->ShotNum % 2) {
				//try to bring to end
				double mx = 0, my = 0;
				bool flag = false;
				for (int k = 0;k < gs->ShotNum;k++) {
					if (rank[k] % 2 == gs->ShotNum % 2) {
						//my ball in the house is ahead an enemy's ball
						if (gs->body[rank[j]][0] - gs->body[rank[k]][0]<0.28&&gs->body[rank[j]][0] - gs->body[rank[k]][0]>-0.28&&gs->body[rank[k]][1] < gs->body[rank[j]][1] && is_in_House(gs->body[rank[k]][0], gs->body[rank[k]][1]) && gs->body[rank[k]][1] - gs->body[rank[j]][1]<0.32&&gs->body[rank[k]][1] - gs->body[rank[j]][1] > -0.32) {
							bool flag2 = true;
							for (size_t m = 0; m < gs->ShotNum; m++)
							{
								if (rank[m] % 2 == gs->ShotNum % 2 && gs->body[rank[j]][0] - gs->body[rank[m]][0]<0.28&&gs->body[rank[j]][0] - gs->body[rank[m]][0]>-0.28&&gs->body[rank[m]][1] > gs->body[rank[j]][1] && gs->body[rank[m]][1] - gs->body[rank[j]][1]<0.32&&gs->body[rank[k]][1] - gs->body[rank[j]][1] > -0.32) {
									flag2 = false;
									break;
								}
							}
							if (flag2) {
								flag = true;
								mx = gs->body[rank[k]][0];
								my = gs->body[rank[k]][1];
								break;
							}
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
	
	//sending policy after hitting fail or not needed
	//already ball in red zone
	if (is_in_Red(gs->body[rank[0]][0], gs->body[rank[0]][1])) {
		//choose a nearby place then get shooting configuration
		double cx = gs->body[rank[0]][0];
		double cy = gs->body[rank[0]][1];

		//choose two places once a time(changed to shooting configuration)
		std::vector<double> leftPos = { l2cx(gs->body[rank[0]][0]),l2cy(gs->body[rank[0]][1]) };
		std::vector<double> rightPos = { l2cx(gs->body[rank[0]][0]),l2cy(gs->body[rank[0]][1]) };

		for (int i = 0;i < 4;i++) {
			if (i != 0) {
				leftPos[0] -= 0.31;
				rightPos[0] += 0.31;
			}
			std::vector<double> output = search::sendToDesStraight(leftPos);
			if (output[2] > -2.29&&search::CheckCollidedForSending(output, leftPos, rank, gs) == 0) {
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				return;
			}
			output = search::sendToDesStraight(rightPos);
			if (output[2] > -2.29&&search::CheckCollidedForSending(output, rightPos, rank, gs) == 0) {
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				return;
			}
			output = search::sendToDes(leftPos);
			if (output[2] > -2.29&&search::CheckCollidedForSending(output, leftPos, rank, gs) == 0) {
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				return;
			}
			output = { output[0],-output[1],2 * leftPos[0] - output[2] };
			if (output[2] < 2.29&&search::CheckCollidedForSending(output, leftPos, rank, gs) == 0) {
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				return;
			}
			output = search::sendToDes(rightPos);
			if (output[2] > -2.29&&search::CheckCollidedForSending(output, rightPos, rank, gs) == 0) {
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				return;
			}
			output = { output[0],-output[1],2 * rightPos[0] - output[2] };
			if (output[2] < 2.29&&search::CheckCollidedForSending(output, rightPos, rank, gs) == 0) {
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2];
				vec_ret->angle = output[1];
				return;
			}
		}


	}
	/*
	if ( is_in_Red(gs->body[rank[0]][0], gs->body[rank[0]][1])) {
		//choose a nearby place then get shooting configuration
		double cx = gs->body[rank[0]][0];
		double cy = gs->body[rank[0]][1];

		//choose two places once a time(changed to shooting configuration)
		std::vector<double> leftPos = { gs->body[rank[0]][0] - 0.295-2.375,-gs->body[rank[0]][1] + 4.88 + 27.6 };
		std::vector<double> rightPos = { gs->body[rank[0]][0] + 0.295-2.375,-gs->body[rank[0]][1] + 4.88 + 27.6 };

		for (int i = 0;i < 4;i++) {
			if (i != 0) {
				leftPos[0] -= 0.145;
				rightPos[0] += 0.145;
			}
			std::vector<double> input = { 3.06f,10.0f ,-1.45f };
			std::vector<double> output = search::TuneOffset(input, leftPos);
			if (search::CheckCollided(output, leftPos, rank, gs) == 1 || output[2] < -2.33) {
				input = { 3.06f,-10.0f ,1.45f };
				output = search::TuneOffset(input, leftPos);
				if (search::CheckCollided(output, leftPos, rank, gs) == 1 || output[2] > 2.33) {
					input = { 3.06f,-10.0f ,1.45f };
					output = search::TuneOffset(input, rightPos);
					if (search::CheckCollided(output, rightPos, rank, gs) == 1 || output[2] > 2.33) {
						input = { 3.06f,10.0f ,-1.45f };
						output = search::TuneOffset(input, rightPos);
						if (search::CheckCollided(output, rightPos, rank, gs) == 1 || output[2] < -2.33) {
							input = { 3,0,0 };//straight hit
							output = search::TuneOffset(input, rightPos);
							if (search::CheckCollided(output, rightPos, rank, gs) == 1) {
								input = { 3,0,0 };//straight hit
								output = search::TuneOffset(input, leftPos);
								if (search::CheckCollided(output, leftPos, rank, gs) == 1) {
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
						vec_ret->speed = output[0];
						vec_ret->h_x = output[2] + 0.2;
						vec_ret->angle = output[1];
						return;
					}
					vec_ret->speed = output[0];
					vec_ret->h_x = output[2] - 0.2;
					vec_ret->angle = output[1];
					return;
				}
				vec_ret->speed = output[0];
				vec_ret->h_x = output[2] - 0.02;
				vec_ret->angle = output[1];
				return;
			}
			vec_ret->speed = output[0];
			vec_ret->h_x = output[2] + 0.02;
			vec_ret->angle = output[1];
			return;
		}
			

	}*/
	/*vec_ret->speed = 3.06f;
	vec_ret->h_x = -1.45;
	vec_ret->angle = 10.0f;*/
		


	// its the last ball so shoot center
	//no ball in center so shoot to it
	//send to existing balls back
	//set destination
	double radius = 2.9;

	//need to filter input ball
	//1. not the first ball
	//2. nearest ball is not 0,0
	//3. nearest ball should be in good zone
	if (gs->ShotNum!=0&&gs->body[rank[0]][1]!=0&&is_in_Down_house(gs->body[rank[0]][0], gs->body[rank[0]][1],radius)) {
		std::vector<double> destination = { gs->body[rank[0]][0] - 0.145 / 2-2.375,27.6+4.88-(gs->body[rank[0]][1] - radius) };
		std::vector<double> input = search::sendToDes(destination);
		//search::CheckCollidedForSending(input, destination, rank, gs) == 1
		if (input[2] < -2.29|| search::CheckCollidedForSending(input, destination, rank, gs) == 1) {
			// search::CheckCollidedForSending({ input[0],-input[1],2 * (destination[0] + 0.145 / 2) - input[2] }, { destination[0] + 0.145,destination[1] }, rank, gs) == 1 
			if (2 * (destination[0] + 0.145 / 2) - input[2] > 2.29|| search::CheckCollidedForSending({ input[0],-input[1],2 * (destination[0] + 0.145 / 2) - input[2] }, { destination[0] + 0.145,destination[1] }, rank, gs) == 1) {
				//cannot send here from left or right
				//sad story 
				//then random shoot to around
				srand(unsigned(std::time(0)));
				for (int i = 0;i < 20;i++) {
					double offset = -1 + rand() % 200 / 100.0;
					double speed = 2.86 + (offset > 0 ? offset / 10 : offset / -10);
					std::vector<double> output = { speed,0,offset };
					if (search::CheckCollidedForSending(output, { 0,27.6 }, rank, gs) == 1) {
						continue;
					}
					vec_ret->speed = speed;
					vec_ret->h_x = offset;
					vec_ret->angle = 0;
					return;
				}
				double offset = -1 + rand() % 200 / 100.0;
				double speed = 2.86 + (offset > 0 ? offset / 10 : offset / -10);
				vec_ret->speed = speed;
				vec_ret->h_x = offset;
				vec_ret->angle = 0;
				return;

			}
			vec_ret->speed = input[0];
			vec_ret->h_x = 2 * (destination[0] + 0.145 / 2) - input[2];
			vec_ret->angle = -input[1];
			return;
		}
		vec_ret->speed = input[0];
		vec_ret->h_x = input[2];
		vec_ret->angle = input[1];
		return;
	}
		

	//not last ball and have no item to shoot
	// then random shoot to around
	srand(unsigned(std::time(0)));
	for (int i = 0;i < 20;i++) {
		double offset = -1 + rand() % 200 / 100.0;
		double speed = 2.86 + (offset > 0 ? offset / 10 : offset / -10);
		std::vector<double> output = { speed,0,offset };
		if (search::CheckCollidedForSending(output, {0,27.6}, rank, gs) == 1 ) {
			continue;
		}
		vec_ret->speed = speed;
		vec_ret->h_x = offset;
		vec_ret->angle = 0;
		return;
	}
	double offset = -1 + rand() % 200 / 100.0;
	double speed = 2.86 + (offset > 0 ? offset / 10 : offset / -10);
	vec_ret->speed = speed;
	vec_ret->h_x = offset;
	vec_ret->angle = 0;
	return;
			
	
}