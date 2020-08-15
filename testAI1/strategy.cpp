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


	if (gs->ShotNum==0) {
		//means u are the first one to shoot
		//put your curling on the middle of way to house
		vec_ret->speed = 2.85f;
		vec_ret->h_x = -0.3f;
		vec_ret->angle = 0;
		return;
	}
	else  {
		// the second one
		// bloody red wanna kill every small blue

		//find the closest curling to the house center, if is enemy and in 3 circles distance.
		if (is_in_House(gs->body[rank[0]][0], gs->body[rank[0]][1])&&rank[0]%2!=gs->ShotNum%2&&get_dist(gs->body[rank[0]][0], gs->body[rank[0]][1])<10) {
			//then the first curling is in the house
			//kill it


			//for single mode

			int flag = 0;//means soft hits succeed
			//if some other balls in the way then change shotting policy
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
					std::vector<double> input = { 8.0f,-10.0f ,+1.0f };
					std::vector<double> output = search::TuneOffset(input, { x,y });
					if (search::CheckCollided(output, { x,y }, rank, gs) == 1||output[2]>2.33) {
						input = { 8.0f,10.0f,-1.0f };
						output = search::TuneOffset(input, { x,y });
						if (search::CheckCollided(output, { x,y }, rank, gs) == 1 || output[2] < -2.33) {
							input = { 4.0f,10.0f,-1.5f };
							output = search::TuneOffset(input, { x,y });
							if (search::CheckCollided(output, { x,y }, rank, gs) == 1 || output[2] < -2.33) {
								flag = 1;//means soft hits failed.
								break;
							}
							vec_ret->speed = output[0];
							vec_ret->h_x = output[2] + 0.2;
							vec_ret->angle = output[1];
							return;
						}
						vec_ret->speed = output[0];
						vec_ret->h_x = output[2] + 0.05;
						vec_ret->angle = output[1];
						return;
					}
					vec_ret->speed = output[0];
					vec_ret->h_x = output[2] - 0.02;
					vec_ret->angle = output[1];
					return;
				}
			}

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
			if (flag != 1) {
				//attack in straight
				if (gs->body[rank[0]][0] - 2.375 < 0) {
					vec_ret->speed = 10.0f;
					vec_ret->h_x = gs->body[rank[0]][0] - 2.35f;
					vec_ret->angle = 0;
				}
				else {
					vec_ret->speed = 10.0f;
					vec_ret->h_x = gs->body[rank[0]][0] - 2.375f;
					vec_ret->angle = 0;
				}

				return;
			}
			
		}
		//sending policy
		//already ball in red zone
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
			

		}
		/*vec_ret->speed = 3.06f;
		vec_ret->h_x = -1.45;
		vec_ret->angle = 10.0f;*/
		


		// its the last ball so shoot center
		//no ball in center so shoot to it
		if (gs->ShotNum == 14) {
			srand(unsigned(std::time(0)));
			int sign = 0;
			if ((rand() % 100) < 50) {
				sign = 1;
			}

			for (int i = 0;i < 9;i++) {
				std::vector<double> centerPos = { 0,27.3 };
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
					input = { 3.06f,-10.0f ,1.45f };
					output = input;
					if (search::CheckCollidedForSending(output, centerPos, rank, gs) == 1 || output[2] > 2.33) {
						input = { 3.06f,10.0f ,-1.45f };
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
		}
		else if (gs->ShotNum == 15) {
			srand(unsigned(std::time(0)));
			int sign = 0;
			if ((rand() % 100) < 50) {
				sign = 1;
			}

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
					input = { 3.06f,-10.0f ,1.45f };
					output = input;
					if (search::CheckCollidedForSending(output, centerPos, rank, gs) == 1 || output[2] > 2.33) {
						input = { 3.06f,10.0f ,-1.45f };
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
		}
		else {
			//not last ball and have no item to shoot
			// then shoot to around
			srand(unsigned(std::time(0)));
			for (int i = 0;i < 20;i++) {
				double offset = -1 + rand() % 200 / 100.0;
				std::vector<double> output = { 2.85,0,offset };
				if (search::CheckCollidedForSending(output, {0,27.6}, rank, gs) == 1 ) {
					continue;
				}
				vec_ret->speed = 2.85;
				vec_ret->h_x = offset;
				vec_ret->angle = 0;
				return;
			}
			vec_ret->speed = 2.85;
			vec_ret->h_x = -1 + rand() % 200 / 100.0;
			vec_ret->angle = 0;
			
		}
		
		
	}


	// create Shot according to condition of No.1 Stone
	//if (is_in_House(gs->body[rank[0]][0], gs->body[rank[0]][1]))
	//{
	//	if (rank[0] % 2 != gs->ShotNum % 2) {
	//		// choose Shot 1. this case your opponent's curling is in the house
	//		// means the first curling in the rank belongs to the opponent
	//		vec_ret->speed = 3.0f;
	//		vec_ret->h_x = -0.1f;
	//		vec_ret->angle = 3.0f;
	//	}
	//	else {
	//		// choose Shot 2.
	//		// this case your curling is in the house
	//		vec_ret->speed = 2.9f;
	//		vec_ret->h_x = -0.1f;
	//		vec_ret->angle = 3.4f;
	//	}
	//}
	//else {
	//	// choose Shot 3.
	//	// this case no curling is in the house
	//	
	//	vec_ret->speed = 3.0f;
	//	vec_ret->h_x = -0.2f;
	//	vec_ret->angle = 3.0f;
	//}
	//  all bellow code is just for test
	//  you need to make your good logic or model
	//if (gs->ShotNum > 10)
	//{
	//	if (gs->ShotNum % 2 == 0)
	//	{
	//		vec_ret->speed = 3.0f;
	//		vec_ret->h_x = 1.5f;
	//		vec_ret->angle = -5.0f;
	//	}
	//	if (gs->ShotNum % 2 == 1)
	//	{
	//		vec_ret->speed = 3.0f;
	//		vec_ret->h_x = -1.5f;
	//		vec_ret->angle = 5.0f;
	//	}
	//}
	//// last shot
	//if (gs->ShotNum > 14)
	//{
	//	vec_ret->speed = 3.0f;
	//	vec_ret->h_x = -1.0f;
	//	vec_ret->angle = 4.0f;
	//}
	//// presentation for free defense zone rule
	//if (gs->ShotNum < 5)
	//{
	//	if (gs->ShotNum % 2 == 0)
	//	{
	//		vec_ret->speed = 2.5f;
	//		vec_ret->h_x = 0.0f;
	//		vec_ret->angle = 0.0f;
	//	}
	//	else
	//	{
	//		vec_ret->speed = 5.0f;
	//		vec_ret->h_x = 0.1f;
	//		vec_ret->angle = -1.0f;
	//	}
	//}
}