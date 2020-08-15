#pragma once
#include "search.h"

namespace search{
	
	double fdx1(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		return sin(atan((r * w * sin(x) + vx) / (vy + r * w * cos(x)))) + sin(atan((r * w * sin(x) + vx) / (vy - r * w * cos(x))));
	}

	// Token: 0x0600037E RID: 894 RVA: 0x00014558 File Offset: 0x00012758
	double fdx2(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		return sin(atan((r * w * sin(x) - vx) / (vy + r * w * cos(x)))) + sin(atan((r * w * sin(x) - vx) / (vy - r * w * cos(x))));
	}

	// Token: 0x0600037F RID: 895 RVA: 0x000145C8 File Offset: 0x000127C8
	double fdx3(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		return sin(atan((r * w * sin(x) + vx) / (vy - r * w * cos(x))));
	}

	// Token: 0x06000380 RID: 896 RVA: 0x00014614 File Offset: 0x00012814
	double fwx1(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		double num = r * w * sin(x) + vx;
		double num2 = r * w * cos(x) + vy;
		return (pow(num, 2.0) + pow(num2, 2.0)) * sin(atan(num / num2));
	}

	// Token: 0x06000381 RID: 897 RVA: 0x00014688 File Offset: 0x00012888
	double fwx2(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		double num = r * w * sin(x) + vx;
		double num2 = vy - r * w * cos(x);
		return (pow(num, 2.0) + pow(num2, 2.0)) * sin(atan(num / num2));
	}

	// Token: 0x06000382 RID: 898 RVA: 0x000146FC File Offset: 0x000128FC
	double fwx3(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		double num = r * w * sin(x) - vx;
		double num2 = vy + r * w * cos(x);
		return (pow(num, 2.0) + pow(num2, 2.0)) * sin(atan(num / num2));
	}

	// Token: 0x06000383 RID: 899 RVA: 0x00014770 File Offset: 0x00012970
	double fwx4(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		double num = r * w * sin(x) - vx;
		double num2 = vy - r * w * cos(x);
		return (pow(num, 2.0) + pow(num2, 2.0)) * sin(atan(num / num2));
	}

	// Token: 0x06000384 RID: 900 RVA: 0x000147E4 File Offset: 0x000129E4
	double fdy1(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		return cos(atan((r * w * sin(x) + vx) / (vy + r * w * cos(x)))) + cos(atan((r * w * sin(x) + vx) / (vy - r * w * cos(x))));
	}

	// Token: 0x06000385 RID: 901 RVA: 0x00014854 File Offset: 0x00012A54
	double fdy2(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		return cos(atan((r * w * sin(x) - vx) / (vy + r * w * cos(x)))) + cos(atan((r * w * sin(x) - vx) / (vy - r * w * cos(x))));
	}

	// Token: 0x06000386 RID: 902 RVA: 0x000148C4 File Offset: 0x00012AC4
	double fdy3(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		return cos(atan((r * w * sin(x) + vx) / (vy - r * w * cos(x))));
	}

	// Token: 0x06000387 RID: 903 RVA: 0x00014910 File Offset: 0x00012B10
	double fwy1(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		double num = r * w * sin(x) + vx;
		double num2 = r * w * cos(x) + vy;
		return (pow(num, 2.0) + pow(num2, 2.0)) * cos(atan(num / num2));
	}

	// Token: 0x06000388 RID: 904 RVA: 0x00014984 File Offset: 0x00012B84
	double fwy2(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		double num = r * w * sin(x) + vx;
		double num2 = vy - r * w * cos(x);
		return (pow(num, 2.0) + pow(num2, 2.0)) * cos(atan(num / num2));
	}

	// Token: 0x06000389 RID: 905 RVA: 0x000149F8 File Offset: 0x00012BF8
	double fwy3(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		double num = r * w * sin(x) - vx;
		double num2 = vy + r * w * cos(x);
		return (pow(num, 2.0) + pow(num2, 2.0)) * cos(atan(num / num2));
	}

	// Token: 0x0600038A RID: 906 RVA: 0x00014A6C File Offset: 0x00012C6C
	double fwy4(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		double num = r * w * sin(x) - vx;
		double num2 = vy - r * w * cos(x);
		return (pow(num, 2.0) + pow(num2, 2.0)) * cos(atan(num / num2));
	}

	// Token: 0x0600038B RID: 907 RVA: 0x00014AE0 File Offset: 0x00012CE0
	double fda1(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		return sin(x + 1.5707999467849731 - atan((r * w * sin(x) + vx) / (vy + r * w * cos(x))));
	}

	// Token: 0x0600038C RID: 908 RVA: 0x00014B38 File Offset: 0x00012D38
	double fda2(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		return sin(x + 1.5707999467849731 + atan((r * w * sin(x) + vx) / (vy - r * w * cos(x))));
	}

	// Token: 0x0600038D RID: 909 RVA: 0x00014B90 File Offset: 0x00012D90
	double fda3(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		return sin(1.5707999467849731 - x + atan((r * w * sin(x) - vx) / (vy + r * w * cos(x))));
	}

	// Token: 0x0600038E RID: 910 RVA: 0x00014BE8 File Offset: 0x00012DE8
	double fda4(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		return sin(1.5707999467849731 - x - atan((r * w * sin(x) - vx) / (vy - r * w * cos(x))));
	}

	// Token: 0x0600038F RID: 911 RVA: 0x00014C40 File Offset: 0x00012E40
	double fwa1(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		double num = r * w * sin(x) + vx;
		double num2 = r * w * cos(x) + vy;
		return (pow(num, 2.0) + pow(num2, 2.0)) * sin(x + 1.5707999467849731 - atan(num / num2));
	}

	// Token: 0x06000390 RID: 912 RVA: 0x00014CC0 File Offset: 0x00012EC0
	double fwa2(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r1;
		double num = r * w * sin(x) + vx;
		double num2 = vy - r * w * cos(x);
		return (pow(num, 2.0) + pow(num2, 2.0)) * sin(x + 1.5707999467849731 + atan(num / num2));
	}

	// Token: 0x06000391 RID: 913 RVA: 0x00014D40 File Offset: 0x00012F40
	double fwa3(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		double num = r * w * sin(x) - vx;
		double num2 = vy + r * w * cos(x);
		return (pow(num, 2.0) + pow(num2, 2.0)) * sin(1.5707999467849731 - x + atan(num / num2));
	}

	// Token: 0x06000392 RID: 914 RVA: 0x00014DC0 File Offset: 0x00012FC0
	double fwa4(double x, MyParams param)
	{
		double vx = param.vx;
		double vy = param.vy;
		double w = param.w;
		double r = param.r2;
		double num = r * w * sin(x) - vx;
		double num2 = vy - r * w * cos(x);
		return (pow(num, 2.0) + pow(num2, 2.0)) * sin(1.5707999467849731 - x - atan(num / num2));
	}

	// Token: 0x06000393 RID: 915 RVA: 0x00014E40 File Offset: 0x00013040
	double fsimp(double a, double b, double eps, MyParams param, int type, int i)
	{
		double num = 0.0;
		double num2 = 0.0;
		int num3 = 1;
		double num4 = b - a;
		if (type == 1)
		{
			if (i == 1)
			{
				num = num4 * (fdx1(a, param) + fdx1(b, param)) / 2.0;
			}
			if (i == 2)
			{
				num = num4 * (fdx2(a, param) + fdx2(b, param)) / 2.0;
			}
			if (i == 3)
			{
				num = num4 * (fwx1(a, param) + fwx1(b, param)) / 2.0;
			}
			if (i == 4)
			{
				num = num4 * (fwx2(a, param) + fwx2(b, param)) / 2.0;
			}
			if (i == 5)
			{
				num = num4 * (fwx3(a, param) + fwx3(b, param)) / 2.0;
			}
			if (i == 6)
			{
				num = num4 * (fwx4(a, param) + fwx4(b, param)) / 2.0;
			}
			if (i == 7)
			{
				num = num4 * (fdx3(a, param) + fdx3(b, param)) / 2.0;
			}
		}
		else if (type == 2)
		{
			if (i == 1)
			{
				num = num4 * (fdy1(a, param) + fdy1(b, param)) / 2.0;
			}
			if (i == 2)
			{
				num = num4 * (fdy2(a, param) + fdy2(b, param)) / 2.0;
			}
			if (i == 3)
			{
				num = num4 * (fwy1(a, param) + fwy1(b, param)) / 2.0;
			}
			if (i == 4)
			{
				num = num4 * (fwy2(a, param) + fwy2(b, param)) / 2.0;
			}
			if (i == 5)
			{
				num = num4 * (fwy3(a, param) + fwy3(b, param)) / 2.0;
			}
			if (i == 6)
			{
				num = num4 * (fwy4(a, param) + fwy4(b, param)) / 2.0;
			}
			if (i == 7)
			{
				num = num4 * (fdy3(a, param) + fdy3(b, param)) / 2.0;
			}
		}
		else if (type == 3)
		{
			if (i == 1)
			{
				num = num4 * (fda1(a, param) + fda1(b, param)) / 2.0;
			}
			if (i == 2)
			{
				num = num4 * (fda2(a, param) + fda2(b, param)) / 2.0;
			}
			if (i == 3)
			{
				num = num4 * (fda3(a, param) + fda3(b, param)) / 2.0;
			}
			if (i == 4)
			{
				num = num4 * (fda4(a, param) + fda4(b, param)) / 2.0;
			}
			if (i == 5)
			{
				num = num4 * (fwa1(a, param) + fwa1(b, param)) / 2.0;
			}
			if (i == 6)
			{
				num = num4 * (fwa2(a, param) + fwa2(b, param)) / 2.0;
			}
			if (i == 7)
			{
				num = num4 * (fwa3(a, param) + fwa3(b, param)) / 2.0;
			}
			if (i == 8)
			{
				num = num4 * (fwa4(a, param) + fwa4(b, param)) / 2.0;
			}
		}
		double num5 = num;
		double num6 = eps + 1.0;
		while (num6 >= eps)
		{
			double num7 = 0.0;
			for (int j = 0; j <= num3 - 1; j++)
			{
				double x = a + ((double)j + 0.5) * num4;
				if (type == 1)
				{
					if (i == 1)
					{
						num7 += fdx1(x, param);
					}
					if (i == 2)
					{
						num7 += fdx2(x, param);
					}
					if (i == 3)
					{
						num7 += fwx1(x, param);
					}
					if (i == 4)
					{
						num7 += fwx2(x, param);
					}
					if (i == 5)
					{
						num7 += fwx3(x, param);
					}
					if (i == 6)
					{
						num7 += fwx4(x, param);
					}
					if (i == 7)
					{
						num7 += fdx3(x, param);
					}
				}
				else if (type == 2)
				{
					if (i == 1)
					{
						num7 += fdy1(x, param);
					}
					if (i == 2)
					{
						num7 += fdy2(x, param);
					}
					if (i == 3)
					{
						num7 += fwy1(x, param);
					}
					if (i == 4)
					{
						num7 += fwy2(x, param);
					}
					if (i == 5)
					{
						num7 += fwy3(x, param);
					}
					if (i == 6)
					{
						num7 += fwy4(x, param);
					}
					if (i == 7)
					{
						num7 += fdy3(x, param);
					}
				}
				else if (type == 3)
				{
					if (i == 1)
					{
						num7 += fda1(x, param);
					}
					if (i == 2)
					{
						num7 += fda2(x, param);
					}
					if (i == 3)
					{
						num7 += fda3(x, param);
					}
					if (i == 4)
					{
						num7 += fda4(x, param);
					}
					if (i == 5)
					{
						num7 += fwa1(x, param);
					}
					if (i == 6)
					{
						num7 += fwa2(x, param);
					}
					if (i == 7)
					{
						num7 += fwa3(x, param);
					}
					if (i == 8)
					{
						num7 += fwa4(x, param);
					}
				}
			}
			double num8 = (num + num4 * num7) / 2.0;
			num2 = (4.0 * num8 - num) / 3.0;
			num6 = abs(num2 - num5);
			num = num8;
			num5 = num2;
			num3 += num3;
			num4 /= 2.0;
		}
		return num2;
	}

	// Token: 0x06000394 RID: 916 RVA: 0x000153BC File Offset: 0x000135BC
	speed Newfrictionstep(double friction, b2Vec2 vec, double angle, double steptime)
	{
		if (abs(angle) <= 9.9999999747524271E-07)
		{
			angle = 0.0099999997764825821;
		}
		double vx = abs(vec.x);
		double vy = abs(vec.y);
		double num = vec.Length();
		speed result;
		if (num > 0.01)
		{
			double w = abs(angle);
			double num14;
			double num17;
			double num20;
			if (num >= 1.5)
			{
				MyParams param = { vx, vy, w, 0.125, 0.125 };
				double num2 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 1, 1);
				double num3 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 1, 5);
				double num4 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 1, 6);
				double num5 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 2, 1);
				double num6 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 2, 5);
				double num7 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 2, 6);
				double num8 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 3, 1);
				double num9 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 3, 2);
				double num10 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 3, 7);
				double num11 = fsimp(0.0, 1.5707999467849731, 1E-05, param, 3, 8);
				if (angle > 0.0)
				{
					double num12 = -100.0 * friction / 6.2831997871398926 * num2;
					double num13 = 0.20000000298023224 * (num3 + num4) / 19.0;
					num14 = num12 + num13;
					double num15 = 100.0 * friction / 6.2831997871398926 * num5;
					double num16 = 0.20000000298023224 * (num6 + num7) / 19.0;
					num17 = num15 + num16;
					double num18 = 0.125 * (1900.0 * friction / 6.2831997871398926) * (-num8 + num9);
					double num19 = 0.02500000037252903 * (-num10 + num11);
					num20 = num18 + num19;
				}
				else
				{
					double num21 = 100.0 * friction / 6.2831997871398926 * num2;
					double num13 = -0.20000000298023224 * (num3 + num4) / 19.0;
					num14 = num21 + num13;
					double num22 = 100.0 * friction / 6.2831997871398926 * num5;
					double num16 = 0.20000000298023224 * (num6 + num7) / 19.0;
					num17 = num22 + num16;
					double num23 = 0.125 * (1900.0 * friction / 6.2831997871398926) * (num8 - num9);
					double num19 = 0.02500000037252903 * (num10 - num11);
					num20 = num23 + num19;
				}
			}
			else if (num <= 1.5 && num >= 1.0)
			{
				double num24 = 0.12200000137090683;
				double num25 = 0.12800000607967377;
				MyParams param2 = { vx, vy, w, num24, num25 };
				double num26 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 1, 1);
				double num27 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 1, 2);
				double num28 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 1, 3);
				double num29 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 1, 4);
				double num30 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 1, 5);
				double num31 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 1, 6);
				double num32 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 2, 1);
				double num33 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 2, 2);
				double num34 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 2, 3);
				double num35 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 2, 4);
				double num36 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 2, 5);
				double num37 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 2, 6);
				double num38 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 3, 1);
				double num39 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 3, 2);
				double num40 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 3, 3);
				double num41 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 3, 4);
				double num42 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 3, 5);
				double num43 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 3, 6);
				double num44 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 3, 7);
				double num45 = fsimp(0.0, 1.5707999467849731, 1E-05, param2, 3, 8);
				if (angle > 0.0)
				{
					double num46 = -100.0 * friction / 12.566399574279785 * num26 + 100.0 * friction / 12.566399574279785 * num27;
					double num13 = -0.10000000149011612 * (num28 + num29) / 19.0 + 0.10000000149011612 * (num30 + num31) / 19.0;
					num14 = num46 + num13;
					double num47 = 100.0 * friction / 12.566399574279785 * num32 + 100.0 * friction / 12.566399574279785 * num33;
					double num16 = 0.10000000149011612 * (num34 + num35) / 19.0 + 0.10000000149011612 * (num36 + num37) / 19.0;
					num17 = num47 + num16;
					double num48 = num25 * (1900.0 * friction / 12.566399574279785) * (-num38 + num39) + num24 * (1900.0 * friction / 12.566399574279785) * (-num40 + num41);
					double num19 = num24 * 0.20000000298023224 * (-num42 + num43) + num25 * 0.20000000298023224 * (-num44 + num45);
					num20 = num48 + num19;
				}
				else
				{
					double num49 = 100.0 * friction / 12.566399574279785 * num26 + -100.0 * friction / 12.566399574279785 * num27;
					double num13 = 0.10000000149011612 * (num28 + num29) / 19.0 - 0.10000000149011612 * (num30 + num31) / 19.0;
					num14 = num49 + num13;
					double num50 = 100.0 * friction / 12.566399574279785 * num32 + 100.0 * friction / 12.566399574279785 * num33;
					double num16 = 0.10000000149011612 * (num34 + num35) / 19.0 + 0.10000000149011612 * (num36 + num37) / 19.0;
					num17 = num50 + num16;
					double num51 = num25 * (1900.0 * friction / 12.566399574279785) * (num38 - num39) + num24 * (1900.0 * friction / 12.566399574279785) * (num40 - num41);
					double num19 = num24 * 0.20000000298023224 * (num42 - num43) + num25 * 0.20000000298023224 * (num44 - num45);
					num20 = num51 + num19;
				}
			}
			else
			{
				MyParams param3 = { vx, vy, w, 0.125, 0.125 };
				double num52 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 1, 3);
				double num53 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 1, 2);
				double num54 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 1, 7);
				double num55 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 2, 3);
				double num56 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 2, 2);
				double num57 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 2, 7);
				double num58 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 3, 5);
				double num59 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 3, 2);
				double num60 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 3, 3);
				double num61 = fsimp(0.0, 1.5707999467849731, 1E-05, param3, 3, 4);
				if (angle > 0.0)
				{
					double num62 = 100.0 * friction / 6.2831997871398926 * (num53 - num54);
					double num13 = -0.20000000298023224 * num52 / 19.0;
					num14 = num62 + num13;
					double num63 = 100.0 * friction / 6.2831997871398926 * (num56 + num57);
					double num16 = 0.20000000298023224 * num55 / 19.0;
					num17 = num63 + num16;
					double num64 = 0.125 * (1900.0 * friction / 6.2831997871398926) * (num59 - num60 + num61);
					double num19 = -0.02500000037252903 * num58;
					num20 = num64 + num19;
				}
				else
				{
					double num65 = 100.0 * friction / 6.2831997871398926 * (-num53 + num54);
					double num13 = 0.20000000298023224 * num52 / 19.0;
					num14 = num65 + num13;
					double num66 = 100.0 * friction / 6.2831997871398926 * (num56 + num57);
					double num16 = 0.20000000298023224 * num55 / 19.0;
					num17 = num66 + num16;
					double num67 = 0.125 * (1900.0 * friction / 6.2831997871398926) * (-num59 + num60 - num61);
					double num19 = 0.02500000037252903 * num58;
					num20 = num67 + num19;
				}
			}
			vec.x += 10.0 * steptime * num14;
			vec.y += 10.0 * steptime * num17;
			angle += 20.0 * steptime * (num20 / 0.39947499999999997);
			result.v = vec;
			result.angle = angle;
			return result;
		}
		result.v = { 0.0, 0.0 };
		result.angle = 0.0;
		return result;
	}

	// Token: 0x040002DD RID: 733
	const float PI = 3.1416f;

	// Token: 0x040002DE RID: 734
	const float R = 0.125f;

	// Token: 0x040002DF RID: 735
	const float DR = 0.006f;

	// Token: 0x040002E0 RID: 736
	const float FAI = 2.0f;

	// Token: 0x040002E1 RID: 737
	const float K = 0.2f;

	// Token: 0x040002E2 RID: 738
	const float STONEINFO_NEWFRICTION = 1.0f;

	// Token: 0x040002E3 RID: 739
	const float SWEEP_EFFECT = 0.4f;

	double distance(double x1, double x2, double y1, double y2) {
		double x = x1 - x2;
		double y = y1 - y2;
		return sqrt(x * x + y * y);
	}

	//search the most close point to destination and check if hitting happened
	//x,y to starting line
	std::vector<double> search(b2Vec2 inputSpeed, double angular, double offset, double dx, double dy, double vx, double vy, int rank[], const GAMESTATE* const gs) {
		double lower_bound = -0.0002f;
		double upper_bound = 0.0002f;
		std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
		std::default_random_engine re;
		double a_random_double;
		b2Vec2 vec = inputSpeed;
		//double time = 0.007581;
		double time = 0.01;
		double x = 0, y = 0;
		double minDistance = 50;
		double minx = 0;
		double miny = 0;
		double minvx = 0;
		double minvy = 0;
		//CurlingMotion curl;
		for (size_t i = 0; i < 10000 && vec.y != 0; i++)
		{
			a_random_double = unif(re);


			x += time * vec.x;
			y -= time * vec.y;
			//std::cout << a_random_double << std::endl;

			speed speed = Newfrictionstep((double)((1.0f - 0.4f * (float)(0)) * 0.001f + a_random_double), vec, angular, 0.0010000000474974513);
			vec.x = speed.v.x;
			vec.y = speed.v.y;
			angular = speed.angle;
			//std::cout << (float)speed.v.x << " " << -(float)speed.v.y <<" "<<angular <<" "<<x<<" "<<y<<" "<<i<<std::endl;
			//file << (float)speed.v.x << " " << -(float)speed.v.y << " " << angular << " " << x << " " << y << " " << i << std::endl;

			//check time
			if (i % 1 == 0) {
				//check hitting
				for (int j = 0;j < gs->ShotNum;j++) {
					//get hitted then return {0,0}
					if (distance(x+2.375, gs->body[rank[j]][0], y, 27.6 + 4.88 - gs->body[rank[j]][1]) < 0.29) return{ 0,0,0,0 };
					if (distance(x, dx, y, dy) < minDistance) {
						minDistance = distance(x, dx, y, dy);
						minx = x;
						miny = y;
						minvx = vec.x;
						minvy = -vec.y;
					}
				}

			}

		}
		//file.close();
		//std::cout << x << " " << y << std::endl;
		return { minx,miny,minvx,minvy };
	}

	//a function used to get input with destination speed and position
	std::vector<double> FindSolution(double x, double y, double vx, double vy, int rank[], const GAMESTATE* const gs) {
		//vector return vy a offset
		//first step random b2vec and angular for 100 times
		std::uniform_real_distribution<double> s(2, 6);
		std::uniform_real_distribution<double> a(-5, 5);
		std::uniform_real_distribution<double> o(-2, 2);
		std::default_random_engine re;
		for (int i = 0;i < 100;i++) {
			double speed = s(re);
			double angular = a(re);
			double offset = o(re);

			b2Vec2 v = { 0,-speed };
			std::vector<double> answer = search(v, angular, offset, x, y, vx, vy, rank, gs);
			if (distance(x, answer[0], y, answer[1]) < 0.5) {
				if (distance(vx, answer[2], vy, answer[3]) < 0.2)
				{
					std::cout << speed << " " << angular << std::endl;
					return { speed,angular,offset };
				}

			}
		}
		return  { 3,0,x };

	}


	/*
	Check if collided with other balls.
	@parameter
	*/
	int CheckCollided(std::vector<double> input, std::vector<double> destination, int rank[], const GAMESTATE* const gs) {
		b2Vec2 vec{ 0,-input[0] };
		double angular = input[1];
		double lower_bound = -0.0002f;
		double upper_bound = 0.0002f;
		std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
		std::default_random_engine re;
		double a_random_double;
		double x = 0, y = 0;
		//std::ofstream file;
		//file.open("D:\\workspace\\testAI1\\testAI1\\Debug\\result.txt");

		double time = 0.01;

		for (size_t i = 0; i < 10000 && vec.y != 0; i++)
		{
			a_random_double = unif(re);


			x += time * vec.x;
			y -= time * vec.y;
			//std::cout << a_random_double << std::endl;

			speed speed = Newfrictionstep((double)((1.0f - 0.4f * (float)(0)) * 0.001f + a_random_double), vec, angular, 0.0010000000474974513);
			vec.x = speed.v.x;
			vec.y = speed.v.y;
			angular = speed.angle*0.9995;
			//std::cout << (float)speed.v.x << " " << -(float)speed.v.y <<" "<<angular <<" "<<x<<" "<<y<<" "<<i<<std::endl;
			//file << (float)speed.v.x << " " << -(float)speed.v.y << " " << angular << " " << x << " " << y << " " << i << std::endl;

			//check if hitted
			if (i % 1 == 0) {
				//check hitting
				for (int j = 1;j < gs->ShotNum;j++) {
					//get hitted then return {0,0}
					if (distance(x + input[2] + 2.351, gs->body[rank[j]][0], y, 27.6+4.88-gs->body[rank[j]][1]) < 0.30&&gs->body[rank[j]][1] != 0) {
						return 1;
					}
				}

			}

			//this time still not get the line but next time it will
			if (y - destination[1] < 0 && y - destination[1] - vec.y*time>0) {
				//get to the horizon line
				return 0;
			}
		}
		//file.close();
		return 0;
	}

	/*
	Tune input offset, speed, angular by predicating the input and compare with destination
	@parameter input [0] is speed, [1] is angular. [2] is offset
	@parameter destination [0] is x, [1] is y
	*/
	std::vector<double> TuneOffset(std::vector<double> input, std::vector<double> destination) {
		b2Vec2 vec{ 0,-input[0] };
		double angular = input[1];
		double lower_bound = -0.0002f;
		double upper_bound = 0.0002f;
		std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
		std::default_random_engine re;
		double a_random_double;
		double x = 0, y = 0;
		//std::ofstream file;
		//file.open("D:\\workspace\\testAI1\\testAI1\\Debug\\result.txt");

		double time = 0.01;

		for (size_t i = 0; i < 10000 && vec.y != 0; i++)
		{
			a_random_double = unif(re);


			x += time * vec.x;
			y -= time * vec.y;
			//std::cout << a_random_double << std::endl;

			speed speed = Newfrictionstep((double)((1.0f - 0.4f * (float)(0)) * 0.001f + a_random_double), vec, angular, 0.0010000000474974513);
			vec.x = speed.v.x;
			vec.y = speed.v.y;
			angular = speed.angle*0.9995;
			//std::cout << (float)speed.v.x << " " << -(float)speed.v.y <<" "<<angular <<" "<<x<<" "<<y<<" "<<i<<std::endl;
			//file << (float)speed.v.x << " " << -(float)speed.v.y << " " << angular << " " << x << " " << y << " " << i << std::endl;
			
			
			//this time still not get the line but next time it will
			if (y - destination[1]<0&&y - destination[1]-vec.y*time>0) {
				//get to the horizon line
				x+=(destination[1] - y) / (-vec.y)*vec.x;

				//add offset
				x += input[2];
				//compare destination and input
				double temp = destination[0] - x;
				//file.close();
				return std::vector<double>{input[0], input[1], input[2] + temp};
			}
		}
		//file.close();
		return input;
		
	}

	/*
	Check if collided with other balls for sending policy
	@parameter
	*/
	int CheckCollidedForSending(std::vector<double> input, std::vector<double> destination, int rank[], const GAMESTATE* const gs) {
		b2Vec2 vec{ 0,-input[0] };
		double angular = input[1];
		double lower_bound = -0.0002f;
		double upper_bound = 0.0002f;
		std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
		std::default_random_engine re;
		double a_random_double;
		double x = 0, y = 0;
		//std::ofstream file;
		//file.open("D:\\workspace\\testAI1\\testAI1\\Debug\\result.txt");

		double time = 0.01;

		for (size_t i = 0; i < 10000 && vec.y != 0; i++)
		{
			a_random_double = 0;


			x += time * vec.x;
			y -= time * vec.y;
			//std::cout << a_random_double << std::endl;

			speed speed = Newfrictionstep((double)((1.0f - 0.4f * (float)(0)) * 0.001f + a_random_double), vec, angular, 0.0010000000474974513);
			vec.x = speed.v.x;
			vec.y = speed.v.y;
			angular = speed.angle*0.9995;
			//std::cout << (float)speed.v.x << " " << -(float)speed.v.y <<" "<<angular <<" "<<x<<" "<<y<<" "<<i<<std::endl;
			//file << (float)speed.v.x << " " << -(float)speed.v.y << " " << angular << " " << x << " " << y << " " << i << std::endl;

			//check if hitted
			if (i % 1 == 0) {
				//check hitting
				for (int j = 0;j < gs->ShotNum;j++) {
					//get hitted then return {0,0}
					double x1 = x + input[2];
					double x2 = gs->body[rank[j]][0] - 2.351;
					double y1 = y;
					double y2 = 27.6 + 4.88 - gs->body[rank[j]][1];
					double d = distance(x1, x2, y1, y2);
					if (d < 0.29&&gs->body[rank[j]][1] != 0) {
						return 1;
					}
				}

			}

			//this time still not get the line but next time it will
			if (y - destination[1] < 0 && y - destination[1] - vec.y*time>0) {
				//get to the horizon line
				return 0;
			}
		}
		//file.close();
		return 0;
	}


	//send ball to destination
	//destination[0]x [1]y to sending ball line
	std::vector<double> sendToDes(std::vector<double> destination) {
		//speed, angle, offset
		std::vector<double> input = { 3.06,10,-1.45 };
		

		//loop to update input
		for (size_t j = 0; j < 20; j++)
		{
			//all parameters
			b2Vec2 vec{ 0,-input[0] };
			double angular = input[1];
			double x = input[2], y = 0;
			double time = 0.01;

			//all time to find last position
			for (size_t i = 0; i < 10000 && vec.y != 0; i++)
			{

				x += time * vec.x;
				y -= time * vec.y;
				//std::cout << a_random_double << std::endl;

				speed speed = Newfrictionstep((double)((1.0f - 0.4f * (float)(0)) * 0.001f), vec, angular, 0.0010000000474974513);
				vec.x = speed.v.x;
				vec.y = speed.v.y;
				angular = speed.angle*0.9995;
				//std::cout << (float)speed.v.x << " " << -(float)speed.v.y <<" "<<angular <<" "<<x<<" "<<y<<" "<<i<<std::endl;
				//file << (float)speed.v.x << " " << -(float)speed.v.y << " " << angular << " " << x << " " << y << " " << i << std::endl;

			}
			//use destination update input
			double disx = destination[0] - x;
			double disy = destination[1] - y;
			input = { input[0] + disy * 1.0 / 9,input[1] ,input[2] + disx };
			if (abs(disy * 1.0 / 9) + abs(disx)  < 0.0001) break;
		}

		return input;
		
	}

	//like send to destination straight we try to find a solution for high velocity ball
	std::vector<double> sendToMidStraight(std::vector<double> destination, bool strength) {
		//speed, angle, offset
		std::vector<double> input = { 8,0,0 };

		//loop to update input
		for (size_t j = 0; j < 20; j++)
		{
			//all parameters
			b2Vec2 vec{ 0,-input[0] };
			double angular = input[1];
			double x = input[2], y = 0;
			double time = 0.01;
			double sv = (strength ? 4 : 2.5);

			//all time to find last position
			for (size_t i = 0; i < 10000 && vec.y != 0; i++)
			{

				x += time * vec.x;
				y -= time * vec.y;
				//std::cout << a_random_double << std::endl;

				speed speed = Newfrictionstep((double)((1.0f - 0.4f * (float)(0)) * 0.001f), vec, angular, 0.0010000000474974513);
				vec.x = speed.v.x;
				vec.y = speed.v.y;
				angular = speed.angle*0.9995;
				//std::cout << (float)speed.v.x << " " << -(float)speed.v.y <<" "<<angular <<" "<<x<<" "<<y<<" "<<i<<std::endl;
				//file << (float)speed.v.x << " " << -(float)speed.v.y << " " << angular << " " << x << " " << y << " " << i << std::endl;

				//exit
				if (y - destination[1] < 0 && y - destination[1] - vec.y*time>0) {
					//get to the horizon line
					x += (destination[1] - y) / (-vec.y)*vec.x;
					y += (destination[1] - y) / (-vec.y)*(-vec.y);
					break;
				}
			}
			//use destination update input
			double disx = destination[0] - x;
			double disy = destination[1] - y;
			double disvy = -vec.y - sv;
			input = { input[0] - disvy,input[1] ,input[2] + disx };
			if (abs(disy * 1.0 / 9) + abs(disx) + abs(disvy) < 0.0001) break;
		}

		return input;
	}

	//destination to center axis
	std::vector<double> sendToDesStraight(std::vector<double> destination) {
		//speed, angle, offset
		std::vector<double> input = { 3.06,0,0 };


		//loop to update input
		for (size_t j = 0; j < 20; j++)
		{
			//all parameters
			b2Vec2 vec{ 0,-input[0] };
			double angular = input[1];
			double x = input[2], y = 0;
			double time = 0.01;

			//all time to find last position
			for (size_t i = 0; i < 10000 && vec.y != 0; i++)
			{

				x += time * vec.x;
				y -= time * vec.y;
				//std::cout << a_random_double << std::endl;

				speed speed = Newfrictionstep((double)((1.0f - 0.4f * (float)(0)) * 0.001f), vec, angular, 0.0010000000474974513);
				vec.x = speed.v.x;
				vec.y = speed.v.y;
				angular = speed.angle*0.9995;
				//std::cout << (float)speed.v.x << " " << -(float)speed.v.y <<" "<<angular <<" "<<x<<" "<<y<<" "<<i<<std::endl;
				//file << (float)speed.v.x << " " << -(float)speed.v.y << " " << angular << " " << x << " " << y << " " << i << std::endl;

			}
			//use destination update input
			double disx = destination[0] - x;
			double disy = destination[1] - y;
			input = { input[0] + disy * 1.0 / 9,input[1] ,input[2] + disx };
			if (abs(disy * 1.0 / 9) + abs(disx) < 0.0001) break;
		}

		return input;

	}
}

