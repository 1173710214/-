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

	if (gs->ShotNum == 0) {
		//first ball
		//to front 
		vec_ret->speed = 2.775f;
		vec_ret->h_x = 0;
		vec_ret->angle = 0;
		return;

	}
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
						centerPos[0] -= (i + 1) / 2 * 0.07;
						sign = 1;
					}
					else if (sign == 1) {
						centerPos[0] += (i + 1) / 2 * 0.07;
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
		else if(sum >= 2) {
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
						centerPos[0] -= (i + 1) / 2 * 0.145;
						sign = 1;
					}
					else if (sign == 1) {
						centerPos[0] += (i + 1) / 2 * 0.145;
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

			//try to hit
			double targetX = gs->body[rank[0]][0];
			double targetY = gs->body[rank[0]][1];
			double tempX = targetX;
			double tempY = targetY;
			for (size_t i = 1; i < gs->ShotNum; i++)
			{
				if (tempX - gs->body[rank[i]][0]<0.29&&tempX - gs->body[rank[i]][0]>-0.29) {
					//get close in x axis means they can hit 
					if (gs->body[rank[i]][1] > targetY) {
						//make sure ball is under target on y axis
						tempX = gs->body[rank[i]][0];
						tempY = gs->body[rank[i]][1];
					}

				}
			}

			double ex = tempX;
			double ey = tempY;
			//compare 
			if (get_dist(gs->body[rank[0]][0], gs->body[rank[0]][1]) > get_dist(c2lx(dx), c2ly(dy))) {
				//shooting near is a better policy
				return;
			}
			vec_ret->speed = 10.0f;
			vec_ret->h_x = l2cx(ex);
			vec_ret->angle = 0;
			return;
		}
		//speed h_x angle 
	}

	//find all the balls in  the house center, if is enemy.
	for (int j = 0;j < gs->ShotNum;j++) {
		if (is_in_House(gs->body[rank[j]][0], gs->body[rank[j]][1]) && rank[j] % 2 != gs->ShotNum % 2) {
			//then the first curling is in the house
			//kill it
			//for single mode

			//if our ball is ahead and our ball is closer to center
			//then not shoot the ball
			bool flag = false;
			for (int k = 0;k < j;k++) {
				if (rank[k] % 2 == gs->ShotNum % 2) {
					if (gs->body[rank[j]][0] - gs->body[rank[k]][0]<0.25&&gs->body[rank[j]][0] - gs->body[rank[k]][0]>-0.25) {
						flag = true;
					}
				}
			}
			if (flag) {
				continue;
			}
			
			double targetX = gs->body[rank[j]][0];
			double targetY = gs->body[rank[j]][1];
			//try to curling hit
			for (size_t i = j+1; i < gs->ShotNum; i++)
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
			
			bool strength = true;
			double tempX = targetX;
			double tempY = targetY;
			double angle = 0;
			for (size_t i = j+1; i < gs->ShotNum; i++)
			{
				//not an available ball
				if (gs->body[rank[i]][1] == 0) continue;
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

			//straight shoot without other ball then try to be in center
			if (tempX == targetX && tempY == targetY) {
				if (tempX < 2.3991) {
					tempX += pow(2.3991 - tempX,1.0/3)*0.025;
				}
				else
				{
					tempX -= pow(-2.3991 + tempX,1.0/3)*0.025;
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
			std::vector<double> output = search::sendToMidStraight(destination, strength);
			//execute
			vec_ret->speed = output[0];
			vec_ret->h_x = output[2];
			vec_ret->angle = output[1];
			return;


			//int flag = 0;//means soft hits succeed
			////if some other balls in the way then change shotting policy
			//for (size_t i = 1; i < gs->ShotNum; i++)
			//{
			//	if (gs->body[rank[i]][0] - gs->body[rank[0]][0]<0.29 && gs->body[rank[i]][0] - gs->body[rank[0]][0] > -0.29&&gs->body[rank[i]][1]> gs->body[rank[0]][1]) {
			//		//other ball in the way
			//		double x = gs->body[rank[0]][0] - 2.375;
			//		double y = -gs->body[rank[0]][1] + 4.88 - 0.29 + 27.6;
			//		if (get_dist(gs->body[rank[i - 1]][0], gs->body[rank[i - 1]][0], gs->body[rank[i]][0], gs->body[rank[i]][1]) < 0.1024&&rank[i]%2==gs->ShotNum%2) {
			//			x = gs->body[rank[i]][0] - 2.375;
			//			y = -gs->body[rank[i]][1] + 4.88 - 0.29 + 27.6;
			//		}
			//		std::vector<double> input = { 8.0f,-10.0f ,+1.0f };
			//		std::vector<double> output = search::TuneOffset(input, { x,y });
			//		if (search::CheckCollided(output, { x,y }, rank, gs) == 1||output[2]>2.33) {
			//			input = { 8.0f,10.0f,-1.0f };
			//			output = search::TuneOffset(input, { x,y });
			//			if (search::CheckCollided(output, { x,y }, rank, gs) == 1 || output[2] < -2.33) {
			//				input = { 4.0f,10.0f,-1.5f };
			//				output = search::TuneOffset(input, { x,y });
			//				if (search::CheckCollided(output, { x,y }, rank, gs) == 1 || output[2] < -2.33) {
			//					flag = 1;//means soft hits failed.
			//					break;
			//				}
			//				vec_ret->speed = output[0];
			//				vec_ret->h_x = output[2];
			//				vec_ret->angle = output[1];
			//				return;
			//			}
			//			vec_ret->speed = output[0];
			//			vec_ret->h_x = output[2];
			//			vec_ret->angle = output[1];
			//			return;
			//		}
			//		vec_ret->speed = output[0];
			//		vec_ret->h_x = output[2];
			//		vec_ret->angle = output[1];
			//		return;
			//	}
			//}

			//attack in curl
			/*double x = gs->body[rank[0]][0] - 2.375;
			double y = -gs->body[rank[0]][1] + 4.88 - 0.29 + 27.6;
			std::vector<double> input = { 8.0f,-10.0f ,+1.0f };
			std::vector<double> output = search::TuneOffset(input, { x,y });

			input = { 4.0f,10.0f,-1.5f };
			output = search::TuneOffset(input, { x,y });
			vec_ret->speed = output[0];
			vec_ret->h_x = output[2] + 0.2;
			vec_ret->angle = output[1];
			return;*/

			//we find straight cannot work and soft fails too so we choose to send ball
			//if (flag != 1) {
			//	//attack in straight
			//	if (gs->body[rank[0]][0] - 2.375 < 0) {
			//		vec_ret->speed = 10.0f;
			//		vec_ret->h_x = gs->body[rank[0]][0] - 2.35f;
			//		vec_ret->angle = 0;
			//	}
			//	else {
			//		vec_ret->speed = 10.0f;
			//		vec_ret->h_x = gs->body[rank[0]][0] - 2.375f;
			//		vec_ret->angle = 0;
			//	}

			//	return;
			//}

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