#define MAXSIZE 10 //地图规模10*10
#define N 30//障碍物个数最大30

typedef struct Grid
{
	int x;
	int y;
	int type;//0表示通路，1障碍物，2起点，3终点，4路径
	int F;
	int G;
	int H;
	struct Grid *prev;//上一个节点
}GRID, *PGRID;
typedef struct List
{
	PGRID r[MAXSIZE*MAXSIZE];
	int length;
}List;

void InitMap(GRID map[MAXSIZE][MAXSIZE])
{
	int i, j;
	for (i = 0; i < MAXSIZE; i++)
	{
		for (j = 0; j < MAXSIZE; j++)
		{
			map[i][j].x = j;
			map[i][j].y = i;
			map[i][j].type = 0;
			map[i][j].F = 0;
			map[i][j].G = 0;
			map[i][j].H = 0;
			map[i][j].prev = NULL;
		}
	}
}
void CreatWall1(GRID map[MAXSIZE][MAXSIZE])
{
	srand((unsigned)time(NULL));//设置随机种子
	int k;
	int i, j;
	for (k = 0; k < 30; k++)//障碍物的个数30个
	{
		i = rand() % MAXSIZE;
		j = rand() % MAXSIZE;
		map[i][j].type = 1;
	}

}
void CreatWall2(GRID map[MAXSIZE][MAXSIZE])
{
	srand((unsigned)time(NULL));//设置随机种子
	int k;
	int i, j;
	for (k = 0; k < 50; k++)//障碍物的个数50个
	{
		i = rand() % MAXSIZE;
		j = rand() % MAXSIZE;
		map[i][j].type = 1;
	}
}
void PrintMap(GRID map[MAXSIZE][MAXSIZE])
{
	int i, j;
	for (i = 0; i < MAXSIZE; i++)
	{
		for (j = 0; j < MAXSIZE; j++)
		{
			printf("%d ", map[i][j].type);
		}
		printf("\n");
	}
	printf("\n");
}
PGRID chooseStart(GRID my_map[MAXSIZE][MAXSIZE])
{//从左上角选起点
	int i, j;
	for (i = 0; i < MAXSIZE; i++)
	{
		for (j = 0; j < MAXSIZE; j++)
		{
			if (my_map[i][j].type == 0)
			{
				my_map[i][j].type = 2;
				return &my_map[i][j];
			}
		}
	}
	return NULL;
}
PGRID chooseEnd(GRID my_map[MAXSIZE][MAXSIZE])
{//从右下角选终点
	int i, j;
	for (i = MAXSIZE - 1; i >= 0; i--)
	{
		for (j = MAXSIZE - 1; j >= 0; j--)
		{
			if (my_map[i][j].type == 0)
			{
				my_map[i][j].type = 3;
				return &my_map[i][j];
			}
		}
	}
	return NULL;
}
void CountH(GRID map[MAXSIZE][MAXSIZE], PGRID end)
{
	int i, j;
	for (i = 0; i < MAXSIZE; i++)
	{
		for (j = 0; j < MAXSIZE; j++)
		{
			map[i][j].H = (abs(map[i][j].x - end->x)
				+ abs(map[i][j].y - end->y)) * 10;
		}
	}
}
//判断一个点是否在列表中,0表示不在，1表示在
int isGRIDInList(GRID g, List L)
{
	for (int i = 0; i < L.length; i++)
	{
		if (L.r[i] != NULL && g.x == L.r[i]->x &&
			g.y == L.r[i]->y)
		{
			return 1;
		}
	}
	return 0;
}
int MinInOpenlist(List openlist)
{
	int min = 65535;//定义一个足够大的数
	int i;
	int k = -1;
	for (i = 0; i < openlist.length; i++)
	{
		if (openlist.r[i]->F < min)
		{
			min = openlist.r[i]->F;
			k = i;//记录最小值的下标
		}
	}
	PGRID temp = openlist.r[k];
	openlist.r[k] = openlist.r[openlist.length - 1];
	openlist.r[openlist.length - 1] = temp;
	k = openlist.length - 1;
	return k;
}
/*    A*算法子函数
5 6 7
4   0
3 2 1
对相邻的8格中的每一个,剔除掉不能够到达的
*/
void fineNeibor(GRID Neibor[8], GRID my_map[MAXSIZE][MAXSIZE], 
	GRID *cur)
{
	Neibor[0].x = cur->x + 1;
	Neibor[0].y = cur->y;
	Neibor[1].x = cur->x + 1;
	Neibor[1].y = cur->y + 1;
	Neibor[2].x = cur->x;
	Neibor[2].y = cur->y + 1;
	Neibor[3].x = cur->x - 1;
	Neibor[3].y = cur->y + 1;
	Neibor[4].x = cur->x - 1;
	Neibor[4].y = cur->y;
	Neibor[5].x = cur->x - 1;
	Neibor[5].y = cur->y - 1;
	Neibor[6].x = cur->x;
	Neibor[6].y = cur->y - 1;
	Neibor[7].x = cur->x + 1;
	Neibor[7].y = cur->y - 1;

	int i;

	for (i = 0; i < 8; i++)
	{
		if (Neibor[i].x < 0 || Neibor[i].x >= MAXSIZE ||
			Neibor[i].y < 0 || Neibor[i].y >= MAXSIZE)
		{//检测越界
			Neibor[i].x = -1;
			Neibor[i].y = -1;
		}
		else
		{
			if (my_map[Neibor[i].y][Neibor[i].x].type == 1)
			{

				if (i == 0)
				{
					Neibor[1].x = -1;
					Neibor[1].y = -1;
					Neibor[7].x = -1;
					Neibor[7].y = -1;
				}
				else if (i == 2)
				{
					Neibor[1].x = -1;
					Neibor[1].y = -1;
					Neibor[3].x = -1;
					Neibor[3].y = -1;
				}
				else if (i == 4)
				{
					Neibor[3].x = -1;
					Neibor[3].y = -1;
					Neibor[5].x = -1;
					Neibor[5].y = -1;
				}
				else if (i == 6)
				{
					Neibor[5].x = -1;
					Neibor[5].y = -1;
					Neibor[7].x = -1;
					Neibor[7].y = -1;
				}
				Neibor[i].x = -1;
				Neibor[i].y = -1;
			}
		}
	}
}
void PutIn(List *openlist, GRID *p)
{
	int i;
	i = openlist->length - 1;
	while (i >= 0 && p->F > openlist->r[i]->F)
	{
		openlist->r[i + 1] = openlist->r[i];
		i--;
	}
	openlist->r[i + 1] = p;
	openlist->length++;
}
void getGridGF(GRID *p)
{
	int t;
	if (p->prev != NULL)
	{
		t = abs(p->x - p->prev->x) + abs(p->y - p->prev->y);
		if (t == 1)
		{
			p->G = p->prev->G + 10;
		}
		else
		{
			p->G = p->prev->G + 14;
		}
	}
	else
	{
		p->G = 0;
	}
	p->F = p->G + p->H;
}

int AStar(GRID my_map[MAXSIZE][MAXSIZE], PGRID start, PGRID end)
//A*算法，返回0表示无解，返回1表示有解
{
	List openlist = { 0 };
	List closelist = { 0 };
	GRID *cur, *p;
	int k;
	GRID Neibor[8];//8个相邻点

	//把起始格添加到开启列表
	openlist.r[0] = start;
	openlist.length++;
	//重复如下的工作
	while (openlist.length != 0)
	{
		if (isGRIDInList(*end, openlist))
		{
			break;
		}
		//a) 寻找开启列表中F值最低的格子。我们称它为当前格。
		k = MinInOpenlist(openlist);
		cur = openlist.r[k];
		//b) 把它切换到关闭列表。
		closelist.r[closelist.length] = cur;
		closelist.length++;
		openlist.r[k] = NULL;
		openlist.length--;
		//对相邻的8格中的每一个
		fineNeibor(Neibor, my_map, cur);
		for (int i = 0; i < 8; i++)
		{
			if (Neibor[i].x == -1 || 
				isGRIDInList(Neibor[i], closelist))
			{
				//如果它不可通过或者已经在关闭列表中，略过它。
				;
			}
			else
			{
				p = &my_map[Neibor[i].y][Neibor[i].x];
				if (!isGRIDInList(Neibor[i], openlist))
				{//如果它不在开启列表中，把它添加进去。
					p->prev = cur;//把当前格作为这一格的父节点。
					getGridGF(p);//记录这一格的F, G, 和H值。
					PutIn(&openlist, p);
				}
				else
				{//如果它已经在开启列表中，用G值为参考检查新的路径是否更好。
					int t = abs(p->x - cur->x) 
						+ abs(p->y - cur->y);
					int G;
					if (t == 1)
					{
						G = cur->G + 10;
					}
					else
					{
						G = cur->G + 14;
					}
					//更低的G值意味着更好的路径。
					//如果是这样，就把这一格的父节点改成当前格
					//并且重新计算这一格的G和F值。
					if (G < p->G)
					{
						p->prev = cur;
						getGridGF(p);
					}
				}
			}
		}
	}
	if (isGRIDInList(*end, openlist))
	{
		p = end;
		while (p->prev != start)
		{
			p = p->prev;
			p->type = 4;
		}
		return 1;
	}
	return 0;
}