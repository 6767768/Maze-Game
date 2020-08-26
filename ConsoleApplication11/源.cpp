#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<time.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<math.h>
#include"AStar.h"
#pragma comment(lib."WINMM.LIB")

#define N 1000       //////////////////��ͼ��
#define M 700          //////////////////��ͼ��
#define NN 120        ////////////////ѡ��������������
#define MM 80          //////////////////////ѡ����������������
#define K 20             //////////////////////ѡ���������Ĵ�С
#define obstacle1 30   //////////////////////////��һ���ϰ�����Ŀ
#define obstacle2 40  ////////////////////�ڶ����ϰ�����Ŀ
#define Mapx 240         /////////////////////////////����ĺ�����
#define Mapy 100          ///////////////////////////////�����������
#define time1 20             //////////////////////��һ��ʱ������
#define time2 10             //////////////////////�ڶ���ʱ������
#define score 20               ///////////////////////��Ϸ�ܷ�

void DrawMap(GRID my_map[10][10])
{
	int i, j;
	int d = 50;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (my_map[i][j].type == 0 || my_map[i][j].type == 4)
			{
				setlinecolor(RGB(0, 0, 0));
				setfillcolor(RGB(144, 238, 144));
				fillrectangle(Mapx + my_map[i][j].x*d, Mapy + my_map[i][j].y*d,
					Mapx + my_map[i][j].x*d + d, Mapy + my_map[i][j].y*d + d);
			}
			else if (my_map[i][j].type == 1)
			{
				setlinecolor(RGB(0, 0, 0));
				rectangle(Mapx + my_map[i][j].x*d, Mapy + my_map[i][j].y*d,
					Mapx + my_map[i][j].x*d + d, Mapy + my_map[i][j].y*d + d);
			}
		}
	}
}
/////////////////////////////ǽ1��δ�߹��ĵط�0����� 2,3 �յ�,4Ϊ��ʾ·��
void DrawRoad(GRID my_map[10][10])
	{
		int i, j;
		int d = 50;
		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 10; j++)
			{
				if (my_map[i][j].type == 4)
				{
					setfillcolor(YELLOW);
					fillrectangle(Mapx + my_map[i][j].x*d, Mapy + my_map[i][j].y*d,
						Mapx + my_map[i][j].x*d + d, Mapy + my_map[i][j].y*d + d);
				}
			}
		}
	}

int main()
{
	///////////////////////////////////////��������
	int h = 0, m = 0, s = 0;
	int i, j;
	int d = 30;
	int x = 100, y = 100;
	int x1 = 80, y1 = 80;
	char key;
	int choose = 2;
	int picture = 0;
	int score1, score2;

	clock_t start, ending;
	TCHAR a_num[] = { _T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'), _T('7'), _T('8'), _T('9') };
	IMAGE img_bk1;
	IMAGE img_icon;
	IMAGE img_icon1;
	IMAGE img_story;
	IMAGE img_two;
	IMAGE my_two[13], msk_two[13];
	IMAGE img_one;
	IMAGE my_one[10], msk_one[10];
	IMAGE img_three;
	IMAGE my_three[5], msk_three[5];
	IMAGE img_suc11;
	IMAGE img_suc12;
	IMAGE img_suc21;
	IMAGE img_suc22;
	IMAGE img_fail1;
	IMAGE img_fail3;
	IMAGE img_obstacle1;
	IMAGE img_obstacle11;
	IMAGE img_obstacle2;
	IMAGE img_obstacle22;
	IMAGE img_obstacle3;
	IMAGE img_obstacle33;
	IMAGE img_start;
	IMAGE img_1;
	IMAGE img_king;
	IMAGE img_medicine;
	IMAGE img_medicine1;
	RECT rc = { 850, 400, 950, 450 };


	///////////////////////////////////////////////////////////////////
	loadimage(&img_icon, _T("icon.jpg"), K, K);
	loadimage(&img_icon1, _T("icon1.jpg"), K, K);
	loadimage(&img_bk1, _T("qingwa.jpg"), N, M);
	loadimage(&img_story, _T("story.jpg"), N, M);
	loadimage(&img_two, _T("two.bmp"));
	loadimage(&img_one, _T("one.bmp"));
	loadimage(&img_three, _T("three.gif"));
	loadimage(&img_suc11, _T("success1-1.jpg"), N, M);
	loadimage(&img_suc12, _T("success1-2.jpg"), N, M);
	loadimage(&img_suc21, _T("success2-1.jpg"), N, M);
	loadimage(&img_suc22, _T("success2-2.jpg"), N, M);
	loadimage(&img_fail1, _T("fail-1.jpg"), N, M);
	loadimage(&img_fail3, _T("fail3.jpg"), N, M);
	loadimage(&img_obstacle1, _T("obstacle1.jpg"),50,50);
	loadimage(&img_obstacle11, _T("obstacle11.jpg"), 50, 50);
	loadimage(&img_obstacle2, _T("obstacle2.jpg"), 50,50);
	loadimage(&img_obstacle22, _T("obstacle22.jpg"), 50, 50);
	loadimage(&img_obstacle3, _T("obstacle3.jpg"), 50,50);
	loadimage(&img_obstacle33, _T("obstacle33.jpg"), 50, 50);
	loadimage(&img_1, _T("1.jpg"), N, M);
	loadimage(&img_start, _T("start.jpg"));
	loadimage(&img_king, _T("king.jpg"),50,50);
	loadimage(&img_medicine, _T("medicine.jpg"),50,50);
	loadimage(&img_medicine1, _T("medicine1.jpg"),50,50);
	//////////////////////////////////////////////��һ��

	while (1)//0Ϊ�����棬1Ϊ��һ����Ϸ���棬2Ϊ������棬3Ϊ�˳�,4Ϊѡ��ؿ�����,5Ϊ�ڶ�����Ϸ����,6Ϊ�ڶ���ʤ������,7Ϊʧ�ܽ���,8��һ�سɹ�����,9�ڶ���ʧ�ܽ���,10Ϊ�Ʒֽ���
	{
		if (picture == 0)
		{
			initgraph(N, M);

			mciSendString(_T("open startmusic.mp3 alias mainmusic"), NULL, 0, NULL);    
			mciSendString(_T("play mainmusic repeat"), NULL, 0, NULL);

			picture = 1;
			setbkmode(TRANSPARENT);
			putimage(0, 0, &img_bk1, SRCCOPY);
			putimage(80, 80, &img_icon1, SRCAND);
			putimage(80, 80, &img_icon, SRCINVERT);
			settextcolor(RGB(0, 0, 0));
			settextstyle(45, 0, _T("��������"));
			outtextxy(360, 65, _T("��������Ѱ����"));
			settextstyle(K, 0, _T("��Բ"));
			settextcolor(RGB(0, 0, 0));
			outtextxy(NN, MM - K, _T("ѡ��ؿ�"));
			outtextxy(NN, MM, _T("����ģʽ"));
			outtextxy(NN, MM + K, _T("�˳�"));
			settextcolor(RGB(0, 0, 0));
			settextstyle(20, 0, _T("����"));
			moveto(700, 500);
			outtext(_T("��16С�飺��ʲô����"));
			moveto(700, 530);
			outtext(_T("�����ݣ���������ɺɺ������Ϊ"));
			settextstyle(20, 0, _T("��Բ"));
			settextcolor(RGB(0, 0, 0));
			outtextxy(0, M - 20, _T("�������ѡ�񣬰�ENTER��ȷ��"));

			x1 = 80, y1 = 80;

			while (1)
			{
				if (kbhit())
				{
					BeginBatchDraw();
					cleardevice();
					putimage(0, 0, &img_bk1, SRCCOPY);
					settextcolor(RGB(0, 0, 0));
					settextstyle(45, 0, _T("��������"));
					outtextxy(360, 65, _T("��������Ѱ����"));
					settextstyle(K, 0, _T("��Բ"));
					settextcolor(RGB(0, 0, 0));
					outtextxy(NN, MM - K, _T("ѡ��ؿ�"));
					outtextxy(NN, MM, _T("����ģʽ"));
					outtextxy(NN, MM + K, _T("�˳�"));
					settextcolor(RGB(0, 0, 0));
					settextstyle(20, 0, _T("����"));
					moveto(700, 500);
					outtext(_T("��16С�飺��ʲô����"));
					moveto(700, 530);
					outtext(_T("�����ݣ���������ɺɺ������Ϊ"));
					settextstyle(20, 0, _T("��Բ"));
					settextcolor(RGB(0, 0, 0));
					outtextxy(0, M - 20, _T("�������ѡ�񣬰�ENTER��ȷ��"));

					EndBatchDraw();

					key = getch();

					if (key == 80 || key == 72)
					{
						switch (key)
						{
						case 80:
						{
								   if (choose == 2 && y1 <= MM)
								   {
									   choose = 1;
									   y1 = y1 + K;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (choose == 3 && y1 <= MM)
								   {
									   y1 = y1 + K;
									   choose = 2;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (choose = 1 && y1 <= MM)
								   {
									   choose = 2;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (y1 > MM)
								   {
									   choose = 1;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   break;
						}
						case 72:
						{
								   if (choose == 1 && y1 >= MM)
								   {
									   choose = 2;
									   y1 = y1 - K;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (choose == 2 && y1 >= MM)
								   {
									   choose = 3;
									   y1 = y1 - K;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (choose == 3 && y1 >= MM)
								   {
									   choose = 3;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (y1 < MM)
								   {
									   choose = 3;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   break;
						}
						default:
							break;
						}
					}
					else if (key == 13)
					{
						if (choose == 1)

							picture = 3;

						else if (choose == 2)

							picture = 2;


						else if (choose == 3)

							picture = 4;


						break;
					}
					else
					{
						putimage(x1, y1, &img_icon1, SRCAND);
						putimage(x1, y1, &img_icon, SRCINVERT);
						picture = 0;
					}
				}
			}
			mciSendString(_T("stop mainmusic"), NULL, 0, NULL);                       //ֹͣ����
			mciSendString(_T("close mainmusic"), NULL, 0, NULL);
		}
		////////////////////////////////////////////////////////

		/////////////////////////////////////////�ؿ�ѡ�����
		else if (picture == 4)
		{
			initgraph(N, M);

			mciSendString(_T("open startmusic.mp3 alias mainmusic"), NULL, 0, NULL);
			mciSendString(_T("play mainmusic repeat"), NULL, 0, NULL);

			choose = 2;
			x1 = 80;
			y1 = 80;
			setbkmode(TRANSPARENT);
			putimage(0, 0, &img_start, SRCCOPY);
			settextstyle(K, 0, _T("��Բ"));
			settextcolor(RGB(0, 0, 0));
			outtextxy(NN, MM + K, _T("�ڶ��� �����ѣ�"));
			outtextxy(NN, MM, _T("��һ�� ���򵥣�"));
			putimage(80, 80, &img_icon1, SRCAND);
			putimage(80, 80, &img_icon, SRCINVERT);
			settextstyle(20, 0, _T("��Բ"));
			setlinecolor(RGB(0, 0, 0));
			settextcolor(RGB(0, 0, 0));
			outtextxy(0, M - 20, _T("��ESC����������"));

			while (1)
			{
				if (kbhit())
				{
					BeginBatchDraw();
					cleardevice();
					putimage(0, 0, &img_start, SRCCOPY);
					settextstyle(K, 0, _T("��Բ"));
					settextcolor(RGB(0, 0, 0));
					outtextxy(NN, MM + K, _T("�ڶ��� �����ѣ�"));
					outtextxy(NN, MM, _T("��һ�� ���򵥣�"));
					settextstyle(20, 0, _T("��Բ"));
					setlinecolor(RGB(0, 0, 0));
					settextcolor(RGB(0, 0, 0));
					outtextxy(0, M - 20, _T("��ESC����������"));
					EndBatchDraw();

					key = getch();

					if (key == 72 || key == 80)
					{
						switch (key)
						{
						case 72:
						{
								   if (choose == 2 && y1 >= MM)
								   {
									   choose = 2;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (choose == 1 && y1 >= MM)
								   {
									   choose = 2;
									   y1 = y1 - K;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (y1 < MM)
								   {
									   choose = 2;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   break;
						}
						case 80:
						{
								   if (choose == 2 && y1 <= MM)
								   {
									   choose = 1;
									   y1 = y1 + K;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (choose == 1 && y1 <= MM)
								   {
									   y1 = y1 + K;
									   choose = 1;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   else if (y1 > MM)
								   {
									   choose = 1;
									   putimage(x1, y1, &img_icon1, SRCAND);
									   putimage(x1, y1, &img_icon, SRCINVERT);
								   }
								   break;
						}
						}
					}
					else if (key == 13)

					{
						if (choose == 1)
							picture = 5;
						else if (choose == 2)
							picture = 1;
						break;
					}
					else if (key == 27)
					{
						picture = 0;
						choose = 2;
						break;
					}
					else
					{
						putimage(x1, y1, &img_icon1, SRCAND);
						putimage(x1, y1, &img_icon, SRCINVERT);
						picture = 4;
					}
				}
			}
			mciSendString(_T("stop mainmusic"), NULL, 0, NULL);                       //ֹͣ����
			mciSendString(_T("close mainmusic"), NULL, 0, NULL);
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////////��һ����Ϸ����
		else if (picture == 1)
		{

			GRID my_map[10][10];
			InitMap(my_map);
			CreatWall1(my_map);
			PGRID begin = chooseStart(my_map);
			PGRID end = chooseEnd(my_map);

			mciSendString(_T("open mymusic.mp3 alias mymusic"), NULL, 0, NULL);
			mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);
			CountH(my_map, end);
			while (!AStar(my_map, begin, end))
			{
				InitMap(my_map);
				CreatWall1(my_map);
				begin = chooseStart(my_map);
				end = chooseEnd(my_map);
			}

			int deflag11 = 1;
			int k1 = 0, k2 = 0;
			int p1 = 1;
			int i1, j1;
			int i2, j2;
			int d1 = 50;
			int flag1 = 0;
			int dx1 = 0, dy1 = 0;
			int difftime;
			int s = 0, m = 0, h = 0;
			int total = 0;
			
			initgraph(N, M);

			putimage(0, 0, &img_1, SRCCOPY);
			start = clock();

			settextstyle(20, 0, _T("��Բ"));
			setlinecolor(RGB(0, 0, 0));
			settextcolor(RGB(255, 255, 255));
			outtextxy(0, M - 20, _T("��ESC����������"));

			for (i = 0; i < 10; i++)
			{
				for (j = 0; j < 10; j++)
				{
					if (my_map[i][j].type == 4 && my_map[i + 1][j].type != 4 && my_map[i][j + 1].type != 4)
						total = total + 2;
					else if (my_map[i][j].type == 4 && (my_map[i + 1][j].type == 4 || my_map[i][j + 1].type == 4))
						total = total + 1;
				}
			}

			putimage(0, 0, &img_two, SRCCOPY);
			for (i1 = 0; i1 < 13; i1++)
			{
				getimage(&msk_two[i1], i1 * 50, 50, 50, 50);
				getimage(&my_two[i1], i1 * 50, 0, 50, 50);
			}

			putimage(0, 0, &img_three, SRCCOPY);
			for (i1 = 0; i1 < 5; i1++)
			{
				getimage(&msk_three[i1], i1 * 200, 200, 200, 200);
				getimage(&my_three[i1], i1 * 200, 0, 200, 200);
			}

			start = clock();

			while (1)
			{
				i1 = begin->y;
				j1 = begin->x;

				i2 = end->y;
				j2 = end->x;


				BeginBatchDraw();
				cleardevice();
				putimage(0, 0, &img_1, SRCCOPY);

				settextcolor(RGB(0, 0, 0));
				setlinecolor(RGB(0, 0, 0));
				rectangle(850, 400, 950, 450);
				drawtext(_T("��ʾ"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				settextstyle(20, 0, _T("��Բ"));
				settextcolor(RGB(255, 255, 255));
				outtextxy(0, M - 20, _T("��ESC����������"));

				settextcolor(BLACK);
				settextstyle(20, 0, _T("����"));
				ending = clock();
				difftime = (ending - start) / CLK_TCK;
				moveto(800, 200);
				s = (time1-difftime) % 60;
				m = ((time1-difftime) / 60) % 60;
				h = ((time1-difftime) / 3600);
				outtext(_T(""));
				outtext(a_num[h / 10]);
				outtext(a_num[h % 10]);
				outtext(_T(":"));
				outtext(a_num[m / 10]);
				outtext(a_num[m % 10]);
				outtext(_T(":"));
				outtext(a_num[s / 10]);
				outtext(a_num[s % 10]);

				settextcolor(BLACK);
				settextstyle(20, 0, _T("����"));
				score1 = (score-difftime);
				moveto(800, 300);
				outtext(_T("��ĵ÷�"));
				outtext(a_num[score1 / 1000]);
				outtext(a_num[(score1 % 1000) / 100]);
				outtext(a_num[((score1 % 100)) / 10]);
				outtext(a_num[((score1 % 100)) % 10]);

				settextcolor(BLACK);
				settextstyle(20, 0, _T("����"));
				moveto(800, 100);
				outtext(_T("ʣ�ಽ��"));
				outtext(_T(":"));
				outtext(a_num[total / 10]);
				outtext(a_num[total % 10]);



				DrawMap(my_map);

				setbkmode(TRANSPARENT);
				for (i = 0; i < 5; i++)
				{
					for (j = 0; j < 10; j++)
					{
						if (my_map[i][j].type == 1)
						{
							setfillcolor(RGB(144, 238, 144));
							fillrectangle(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, Mapx + my_map[i][j].x*d1 + d1, Mapy + my_map[i][j].y*d1 + d1);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle11, SRCAND);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle1, SRCINVERT);
						}
					}
				}

				for (i = 5; i < 10; i++)
				{
					for (j = 0; j < 4; j++)
					{
						if (my_map[i][j].type == 1)
						{
							setfillcolor(RGB(144, 238, 144));
							fillrectangle(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, Mapx + my_map[i][j].x*d1 + d1, Mapy + my_map[i][j].y*d1 + d1);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle22, SRCAND);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle2, SRCINVERT);
						}
					}
				}

				for (i = 5; i < 10; i++)
				{
					for (j = 4; j < 10; j++)
					{
						if (my_map[i][j].type == 1)
						{
							setfillcolor(RGB(144, 238, 144));
							fillrectangle(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, Mapx + my_map[i][j].x*d1 + d1, Mapy + my_map[i][j].y*d1 + d1);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle33, SRCAND);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle3, SRCINVERT);
						}
					}
				}
				if (deflag11 == 1)
				{
					setfillcolor(RGB(144, 238, 144));
					setbkmode(TRANSPARENT);
					fillrectangle(Mapx + begin->x*d1, Mapy + begin->y*d1, Mapx + begin->x*d1 + d1, Mapy + begin->y*d1 + d1);
					putimage(Mapx + begin->x*d1, Mapy + begin->y*d1, &msk_two[k1], SRCAND);
					putimage(Mapx + begin->x*d1, Mapy + begin->y*d1, &my_two[k1], SRCINVERT);
				}

				putimage(N - 200, M - 200, &msk_three[k2], SRCAND);
				putimage(N - 200, M - 200, &my_three[k2], SRCINVERT);

				setfillcolor(RGB(144, 238, 144));
				fillrectangle(Mapx + end->x*d1, Mapy + end->y*d1, Mapx + end->x*d1 + d1, Mapy + end->y*d1 + d1);
				putimage(Mapx + end->x*d1, Mapy + end->y*d1, &img_medicine1, SRCAND);
				putimage(Mapx + end->x*d1, Mapy + end->y*d1, &img_medicine, SRCINVERT);
				EndBatchDraw();

				/////////////////////////////////////////////////////////////////////

				if (MouseHit())
				{
					MOUSEMSG msg = GetMouseMsg();
					if (msg.uMsg == WM_LBUTTONDOWN)
					{
						if (msg.x >= rc.left&&msg.x <= rc.right&&
							msg.y >= rc.top&&msg.y <= rc.bottom)
						{
							DrawRoad(my_map);
							Sleep(1000);
						}
					}
					void FlushMouseMsgBuffer();
				}
				///////////////////////////////////////////////////////////

				/////////////////////////////////////////////////////////////���̲���
				if (kbhit())
				{
					key = getch();
					if (key == 'a' || key == 's' || key == 'w' || key == 'd')
					{

						switch (key)
						{
						case'a':
							if (j1 - 1 >= 0 && my_map[i1][j1 - 1].type == 0)
							{
								my_map[i1][j1 - 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 - 1];
								deflag11 = 1;
								total--;
							}
							else if (j1 - 1 >= 0 && my_map[i1][j1 - 1].type == 4)
							{
								my_map[i1][j1 - 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 - 1];
								deflag11 = 1;
								total--;
							}
							else if (j1 - 1 >= 0 && my_map[i1][j1 - 1].type == 3)
							{
								my_map[i1][j1 - 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 - 1];
								flag1 = 1;
								deflag11 = 1;
								total--;
							}
							break;
						case'd':
							if ((j1 + 1) < 10 && my_map[i1][j1 + 1].type == 0)
							{
								my_map[i1][j1 + 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 + 1];
								deflag11 = 1;
								total--;
							}
							else if ((j1 + 1) < 10 && my_map[i1][j1 + 1].type == 4)
							{
								my_map[i1][j1 + 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 + 1];
								deflag11 = 1;
								total--;
							}
							else if ((j1 + 1) < 10 && my_map[i1][j1 + 1].type == 3)
							{
								my_map[i1][j1 + 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 + 1];
								flag1 = 1;
								deflag11 = 1;
								total--;
							}
							break;
						case's':
							if ((i1 + 1) < 10 && my_map[i1 + 1][j1].type == 0)
							{
								my_map[i1 + 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 + 1][j1];
								deflag11 = 1;
								total--;
							}
							else if ((i1 + 1) < 10 && my_map[i1 + 1][j1].type == 4)
							{
								my_map[i1 + 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 + 1][j1];
								deflag11 = 1;
								total--;
							}
							else if ((i1 + 1) < 10 && my_map[i1 + 1][j1].type == 3)
							{
								my_map[i1 + 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 + 1][j1];
								flag1 = 1;
								deflag11 = 1;
								total--;
							}
							break;
						case'w':
							if (i1 - 1 >= 0 && my_map[i1 - 1][j1].type == 0)
							{
								my_map[i1 - 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 - 1][j1];
								deflag11 = 1;
								total--;
							}
							else if (i1 - 1 >= 0 && my_map[i1 - 1][j1].type == 4)
							{
								my_map[i1 - 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 - 1][j1];
								deflag11 = 1;
								total--;
							}
							else if (i1 - 1 >= 0 && my_map[i1 - 1][j1].type == 3)
							{
								my_map[i1 - 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 - 1][j1];
								flag1 = 1;
								deflag11 = 1;
								total--;
							}
							break;
						default:
							break;
						}
					}
					else if (key == 27)
					{
						picture = 0;
						choose = 2;
						break;
					}
				}
				if (flag1 == 1)
				{
					picture = 8;
					break;
				}
				else if (total <0||difftime==time1)
				{
					score1 = 0;
					picture = 7;
					break;
				}
				k1 = (k1 + 1) % 12;
				k2 = (k2 + 1) % 5;
				Sleep(50);
			}
			mciSendString(_T("stop mymusic"), NULL, 0, NULL);
			mciSendString(_T("close mymusic"), NULL, 0, NULL);
		}

		
		////////////////////////////////////////////

		////////////////////////////////////////////////////�ڶ�����Ϸ����

		else if (picture == 5)
		{
			GRID my_map[10][10];
			InitMap(my_map);
			CreatWall2(my_map);
			PGRID begin = chooseStart(my_map);
			PGRID end = chooseEnd(my_map);

			mciSendString(_T("open mymusic.mp3 alias mymusic"), NULL, 0, NULL);
			mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);
			CountH(my_map, end);
			while (!AStar(my_map, begin, end))
			{
				InitMap(my_map);
				CreatWall2(my_map);
				begin = chooseStart(my_map);
				end = chooseEnd(my_map);
			}

			int deflag11 = 1;
			int k1 = 0, k2 = 0, k3 = 0;
			int p1 = 1;
			int i1, j1;
			int d1 = 50;
			int flag2 = 0;
			int dx1 = 0, dy1 = 0;
			int difftime;
			int s = 0, m = 0, h = 0;
			int i2, j2;
			int total = 0;

			initgraph(N, M);
			start = clock();
			putimage(0, 0, &img_1, SRCCOPY);

			settextcolor(RGB(255, 255, 255));
			settextstyle(20, 0, _T("��Բ"));
			outtextxy(0, M - 20, _T("��ESC������"));
		
			putimage(0, 0, &img_one, SRCCOPY);
			for (i1 = 0; i1 < 10; i1++)
			{
				getimage(&msk_one[i1], i1 * 50, 50, 50, 50);
				getimage(&my_one[i1], i1 * 50, 0, 50, 50);
			}
			putimage(0, 0, &img_three);
			for (i1 = 0; i1 < 5; i1++)
			{
				getimage(&msk_three[i1], i1 * 200, 200, 200, 200);
				getimage(&my_three[i1], i1 * 200, 0, 200, 200);
			}

			for (i = 0; i < 10; i++)
			{
				for (j = 0; j < 10; j++)
				{
					if (my_map[i][j].type == 4 && my_map[i + 1][j].type != 4 && my_map[i][j + 1].type != 4)
						total = total + 2;
					else if (my_map[i][j].type == 4 && (my_map[i + 1][j].type == 4 || my_map[i][j + 1].type == 4))
						total = total + 1;
				}
			}


			start = clock();

			while (1)
			{
				i1 = begin->y;
				j1 = begin->x;

				i2 = end->y;
				j2 = end->x;


				BeginBatchDraw();
				cleardevice();
				putimage(0, 0, &img_1, SRCCOPY);
				settextcolor(RGB(0, 0, 0));
				setlinecolor(RGB(0, 0, 0));
				rectangle(850, 400, 950, 450);
				drawtext(_T("��ʾ"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				settextstyle(20, 0, _T("��Բ"));
				settextcolor(RGB(255, 255, 255));
				outtextxy(0, M - 20, _T("��ESC������"));

				settextcolor(BLACK);
				settextstyle(20, 0, _T("����"));
				ending = clock();
				difftime = (ending - start) / CLK_TCK;
				moveto(800, 200);
				s = (time2-difftime) % 60;
				m = ((time2-difftime) / 60) % 60;
				h = ((time2-difftime) / 3600);
				outtext(_T(""));
				outtext(a_num[h / 10]);
				outtext(a_num[h % 10]);
				outtext(_T(":"));
				outtext(a_num[m / 10]);
				outtext(a_num[m % 10]);
				outtext(_T(":"));
				outtext(a_num[s / 10]);
				outtext(a_num[s % 10]);

				settextcolor(BLACK);
				settextstyle(20, 0, _T("����"));
				score2 = (score - difftime*2);
				moveto(800, 300);
				outtext(_T("��ĵ÷�"));
				outtext(a_num[score2 / 1000]);
				outtext(a_num[(score2 % 1000) / 100]);
				outtext(a_num[((score2 % 100)) / 10]);
				outtext(a_num[((score2 % 100)) % 10]);

				settextcolor(BLACK);
				settextstyle(20, 0, _T("����"));
				moveto(800, 100);
				outtext(_T("ʣ�ಽ��"));
				outtext(_T(":"));
				outtext(a_num[total / 10]);
				outtext(a_num[total % 10]);

				DrawMap(my_map);
				setbkmode(TRANSPARENT);

				for (i = 0; i < 5; i++)
				{
					for (j = 0; j < 10; j++)
					{
						if (my_map[i][j].type == 1)
						{
							setfillcolor(RGB(144, 238, 144));
							fillrectangle(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, Mapx + my_map[i][j].x*d1 + d1, Mapy + my_map[i][j].y*d1 + d1);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle11, SRCAND);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle1, SRCINVERT);
						}
					}
				}

				for (i = 5; i < 10; i++)
				{
					for (j = 0; j < 4; j++)
					{
						if (my_map[i][j].type == 1)
						{
							setfillcolor(RGB(144, 238, 144));
							fillrectangle(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, Mapx + my_map[i][j].x*d1 + d1, Mapy + my_map[i][j].y*d1 + d1);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle22, SRCAND);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle2, SRCINVERT);
						}
					}
				}

				for (i = 5; i < 10; i++)
				{
					for (j = 4; j < 10; j++)
					{
						if (my_map[i][j].type == 1)
						{
							setfillcolor(RGB(144, 238, 144));
							fillrectangle(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, Mapx + my_map[i][j].x*d1 + d1, Mapy + my_map[i][j].y*d1 + d1);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle33, SRCAND);
							putimage(Mapx + my_map[i][j].x*d1, Mapy + my_map[i][j].y*d1, &img_obstacle3, SRCINVERT);
						}
					}
				}
				EndBatchDraw();


				if (deflag11 == 1)
				{
					setfillcolor(RGB(144, 238, 144));
					fillrectangle(Mapx + begin->x*d1, Mapy + begin->y*d1, Mapx + begin->x*d1 + d1, Mapy + begin->y*d1 + d1);
					putimage(Mapx + begin->x*d1, Mapy + begin->y*d1, &img_king, SRCCOPY);
				}
				{	setfillcolor(WHITE);
				fillrectangle(Mapx + end->x*d1, Mapy + end->y*d1, Mapx + end->x*d1 + d1, Mapy + end->y*d1 + d1);
				putimage(Mapx + end->x*d1, Mapy + end->y*d1, &msk_one[k2], SRCAND);
				putimage(Mapx + end->x*d1, Mapy + end->y*d1, &my_one[k2], SRCINVERT);
			    }
				putimage(N - 200, M - 200, &msk_three[k3], SRCAND);
				putimage(N - 200, M - 200, &my_three[k3], SRCINVERT);

				/////////////////////////////////////////////////////////////////////

				if (MouseHit())
				{
					MOUSEMSG msg = GetMouseMsg();
					if (msg.uMsg == WM_LBUTTONDOWN)
					{
						if (msg.x >= rc.left&&msg.x <= rc.right&&
							msg.y >= rc.top&&msg.y <= rc.bottom)
						{
							//flag = 1;
							DrawRoad(my_map);
							Sleep(200);
						}
					}
					void FlushMouseMsgBuffer();
				}

				if (kbhit())
				{
					key = getch();
					if (key == 'a' || key == 's' || key == 'w' || key == 'd')
					{

						switch (key)
						{
						case'a':
							if (j1 - 1 >= 0 && my_map[i1][j1 - 1].type == 0)
							{
								my_map[i1][j1 - 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 - 1];
								deflag11 = 1;
								total--;
							}
							else if (j1 - 1 >= 0 && my_map[i1][j1 - 1].type == 4)
							{
								my_map[i1][j1 - 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 - 1];
								deflag11 = 1;
								total--;
							}
							else if (j1 - 1 >= 0 && my_map[i1][j1 - 1].type == 3)
							{
								my_map[i1][j1 - 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 - 1];
								flag2 = 1;
								deflag11 = 1;
								total--;
							}
							break;
						case'd':
							if ((j1 + 1) < 10 && my_map[i1][j1 + 1].type == 0)
							{
								my_map[i1][j1 + 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 + 1];
								deflag11 = 1;
								total--;
							}
							else if ((j1 + 1) < 10 && my_map[i1][j1 + 1].type == 4)
							{
								my_map[i1][j1 + 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 + 1];
								deflag11 = 1;
								total--;
							}
							else if ((j1 + 1) < 10 && my_map[i1][j1 + 1].type == 3)
							{
								my_map[i1][j1 + 1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1][j1 + 1];
								flag2 = 1;
								deflag11 = 1;
								total--;
							}
							break;
						case's':
							if ((i1 + 1) < 10 && my_map[i1 + 1][j1].type == 0)
							{
								my_map[i1 + 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 + 1][j1];
								deflag11 = 1;
								total--;
							}
							else if ((i1 + 1) < 10 && my_map[i1 + 1][j1].type == 4)
							{
								my_map[i1 + 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 + 1][j1];
								deflag11 = 1;
								total--;
							}
							else if ((i1 + 1) < 10 && my_map[i1 + 1][j1].type == 3)
							{
								my_map[i1 + 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 + 1][j1];
								flag2 = 1;
								deflag11 = 1;
								total--;
							}
							break;
						case'w':
							if (i1 - 1 >= 0 && my_map[i1 - 1][j1].type == 0)
							{
								my_map[i1 - 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 - 1][j1];
								deflag11 = 1;
								total--;
							}
							else if (i1 - 1 >= 0 && my_map[i1 - 1][j1].type == 4)
							{
								my_map[i1 - 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 - 1][j1];
								deflag11 = 1;
								total--;
							}
							else if (i1 - 1 >= 0 && my_map[i1 - 1][j1].type == 3)
							{
								my_map[i1 - 1][j1].type = 2;
								my_map[i1][j1].type = 0;
								begin = &my_map[i1 - 1][j1];
								flag2 = 1;
								deflag11 = 1;
								total--;
							}
							break;
						default:
							break;
						}
					}
					else if (key == 27)
					{
						picture = 0;
						choose = 2;
						break;
					}
				}
				if (flag2 == 1)
				{
					picture = 6;
					break;
				}
				else if (total<0||difftime==time2)
				{
					score2 = 0;
					picture = 9;
					break;
				}
				k1 = (k1 + 1) % 12;
				k2 = (k2 + 1) % 10;
				k3 = (k3 + 1) % 5;
				Sleep(50);
			}
			mciSendString(_T("stop mymusic"), NULL, 0, NULL);
			mciSendString(_T("close mymusic"), NULL, 0, NULL);
		}

		
		///////////////////////////////////////////////

		/////////////////////////////////////////////////////////�������

		else if (picture == 2)
		{
			initgraph(N, M);

			mciSendString(_T("open musicstory1.mp3 alias storymusic"), NULL, 0, NULL);
			mciSendString(_T("play storymusic repeat"), NULL, 0, NULL);

			setbkmode(TRANSPARENT);
			putimage(0, 0, &img_story, SRCCOPY);
			settextstyle(20, 0, _T("��Բ"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(0, M - 20, _T("��ESC����������"));

			while (1)
			{
				if (kbhit())
				{
					BeginBatchDraw();
					cleardevice();
					putimage(0, 0, &img_story, SRCCOPY);
					settextstyle(20, 0, _T("��Բ"));
					settextcolor(RGB(255, 255, 255));
					outtextxy(0, M - 20, _T("��ESC����������"));
					EndBatchDraw();

					key = getch();

					if (key == 27)
					{
						picture = 0;
						choose = 2;
						break;
					}
					else
					{
						picture = 2;
					}
				}
			}
			mciSendString(_T("stop storymusic"), NULL, 0, NULL);
			mciSendString(_T("close storymusic"), NULL, 0, NULL);
		}
		///////////////////////////////////////////////////////////////

		///////////////////////////////////////////////��һ��ʤ������
		else if (picture == 8)
		{
			initgraph(N, M);

			mciSendString(_T("open success.mp3 alias sucmusic"), NULL, 0, NULL);
			mciSendString(_T("play sucmusic repeat"), NULL, 0, NULL);

			setbkmode(TRANSPARENT);

			putimage(0, 0, &img_suc11, SRCCOPY);
			moveto(0, 600);
			settextcolor(WHITE);
			settextstyle(30, 0, _T("����"));
			outtext(_T("��ĵ÷֣�"));
			outtext(a_num[score1 / 1000]);
			outtext(a_num[(score1 % 1000) / 100]);
			outtext(a_num[((score1 % 100)) / 10]);
			outtext(a_num[((score1 % 100)) % 10]);
			Sleep(3000);
			putimage(0, 0, &img_suc12, SRCCOPY);
			while (1)
			{
				BeginBatchDraw();
				setbkmode(TRANSPARENT);
				putimage(0, 0, &img_suc11, SRCCOPY);

				moveto(0, 600);
				settextcolor(WHITE);
				settextstyle(30, 0, _T("����"));
				outtext(_T("��ĵ÷֣�"));
				outtext(a_num[score1 / 1000]);
				outtext(a_num[(score1 % 1000) / 100]);
				outtext(a_num[((score1 % 100)) / 10]);
				outtext(a_num[((score1 % 100)) % 10]);
				Sleep(3000);
				putimage(0, 0, &img_suc12, SRCCOPY);

				picture = 5;
				break;

			}
			mciSendString(_T("stop sucmusic"), NULL, 0, NULL);
			mciSendString(_T("close sucmusic"), NULL, 0, NULL);
		}
		//////////////////////////////////////////////////////

		///////////////////////////////////////////////////�ڶ���ʤ������

		else if (picture == 6)
		{
			initgraph(N, M);

			mciSendString(_T("open success.mp3 alias sucmusic"), NULL, 0, NULL);
			mciSendString(_T("play sucmusic repeat"), NULL, 0, NULL);

			putimage(0, 0, &img_suc21, SRCCOPY);
			setbkmode(TRANSPARENT);
	
			moveto(0, 600);
			settextcolor(BLACK);
			settextstyle(30, 0, _T("����"));
			outtext(_T("��ĵ÷֣�"));
			outtext(a_num[score2 / 1000]);
			outtext(a_num[(score2 % 1000) / 100]);
			outtext(a_num[((score2 % 100)) / 10]);
			outtext(a_num[((score2 % 100)) % 10]);
			Sleep(3000);
			putimage(0, 0, &img_suc22, SRCCOPY);


			while (1)
			{
				BeginBatchDraw();
				cleardevice();
				putimage(0, 0, &img_suc21, SRCCOPY);
				setbkmode(TRANSPARENT);

				moveto(0,600);
				settextcolor(BLACK);
				settextstyle(30, 0, _T("����"));
				outtext(_T("��ĵ÷֣�"));
				outtext(a_num[score2 / 1000]);
				outtext(a_num[(score2 % 1000) / 100]);
				outtext(a_num[((score2 % 100)) / 10]);
				outtext(a_num[((score2 % 100)) % 10]);
				Sleep(3000);
				putimage(0, 0, &img_suc22, SRCCOPY);
				EndBatchDraw();
				Sleep(1500);
				picture = 0;
				choose = 2;
				break;
			}
			mciSendString(_T("stop sucmusic"), NULL, 0, NULL);
			mciSendString(_T("close sucmusic"), NULL, 0, NULL);
		}

		////////////////////////////////////////////////////��һ��ʧ�ܽ���
		else if (picture == 7)
		{
			initgraph(N, M);

			mciSendString(_T("open fail.mp3 alias failmusic"), NULL, 0, NULL);
			mciSendString(_T("play failmusic repeat"), NULL, 0, NULL);

			putimage(0, 0, &img_fail1, SRCCOPY);

			while (1)
			{
				BeginBatchDraw();

				putimage(0, 0, &img_fail1, SRCCOPY);

				EndBatchDraw();
				Sleep(3000);
					choose = 2;
					picture = 0;
					break;
			}
			mciSendString(_T("stop failmusic"), NULL, 0, NULL);
			mciSendString(_T("close failmusic"), NULL, 0, NULL);
		}
		//////////////////////////////////////////////////////////

		/////////////////////////////////////////////////�ڶ���ʧ�ܽ���

		else if (picture == 9)
		{
			initgraph(N, M);

			mciSendString(_T("open fail.mp3 alias failmusic"), NULL, 0, NULL);
			mciSendString(_T("play failmusic repeat"), NULL, 0, NULL);

			putimage(0, 0, &img_fail3, SRCCOPY);

			while (1)
			{
				BeginBatchDraw();
				putimage(0, 0, &img_fail3, SRCCOPY);
				EndBatchDraw();
				Sleep(3000);
				picture = 0;
				choose = 2;
				break;
			}
			mciSendString(_T("stop failmusic"), NULL, 0, NULL);
			mciSendString(_T("close failmusic"), NULL, 0, NULL);
		}
		/////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////�˳�
		else if (picture == 3)
		{
			closegraph();
			return 0;
		}
		////////////////////////////////////////////////////////////
	}
}
