#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>

//蛇的状态：U上，D下，L左，R右
#define U 1
#define D 2
#define L 3
#define R 4

typedef struct SNAKE {     //用链表结构蛇身的一个节点
	int x;
	int y;
	struct SNAKE *next;
} snake;

//全局变量
int score = 0, add = 10; 
int status, sleeptime = 400; //每次运行的时间间隔
snake *head, *food; //蛇头指针，实物指针
snake *q, *p; //遍历蛇用到的指针
int endgamestatus = 0; //游戏状态，1：撞墙；2：咬到自己；3：主动退出游戏

//声明全部函数
void pos(int x,int y);
void creatMap(void);
void initsnake(void);
int biteself(void);
snake* createfood(void);
void cantcrosswall(void);
void snakemove(void);
void pause(void);
void gamecircle(void);
void welcometogame(void);
void endgame(void);
void gamestart(void);

void pos(int x, int y) {   //设置光标位置,根据这个函数实现在屏幕上特定位置输出
	COORD pos;        //定义光标位置结构实例
	HANDLE hOutput;   //定义句柄实例
	pos.X = x;        
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE); //获取标准输出句柄
	SetConsoleCursorPosition(hOutput, pos); //根据x，y和句柄通过内置函数设定光标位置
}

void creatMap(void) {
	int i;
	for (i = 0; i < 58; i += 2) { //打印上下边框
		pos(i, 0);
		printf("■");
		pos(i, 26);
		printf("■");
	}
	for (i = 1; i < 26; i++) { //打印左右边框
		pos(0, i);
		printf("■");
		pos(56, i);
		printf("■");
	}
}

void initsnake(void) { //初始化蛇身
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake)); //从蛇尾开始，头插法，以x,y设定开始的位置
	tail->x = 24;
	tail->y = 5;
	tail->next = NULL;
	for (i = 1; i <= 4; i++) {
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + 2 * i;
		head->y = 5;
		tail = head;
	}
	while (tail != NULL) {
		pos(tail->x, tail->y);
		printf("■");
		tail = tail->next;
	}
}


int biteself(void) {    //利用self节点从头开始遍历身体，判断头身位置是否在同一点
	int result = 0;
	snake *self;
	self = head->next;
	while (self != NULL) {
		if (self->x == head->x && self->y == head->y) {
			result = 1;
		}
		self = self->next;
	}
	return result;
}

snake* createfood(void) {
	snake *food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	while ((food_1->x % 2) != 0) {         //随机生成食物位置
		food_1->x = rand() % 52 + 2;
	}
	food_1->y = rand() % 24 + 1;
	q = head;
	while (q != NULL) {
		if (q->x == food_1->x && q->y == food_1->y) { //食物和身体重合就再生成一次
			free(food_1);
			food_1 = createfood();
			break;
		}
		q = q->next;
	}
	pos(food_1->x, food_1->y);
	food = food_1;
	printf("■");
}

void cantcrosswall(void) {
	if (head->x <= 0 || head->x >= 58 || head->y <= 0 || head->y >= 26) {
		endgamestatus = 1;
		endgame();
	}
}

void snakemove(void) {
	snake *nexthead;
	cantcrosswall();

	nexthead = (snake*)malloc(sizeof(snake));
	if (status == U) {
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		if (nexthead->x == food->x && nexthead->y == food->y) {
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score += add;
			createfood();
		}
		else {
			nexthead->next = head;
			head = nexthead;
			p = head;
			q = head->next;
			while (q->next != NULL) {
				pos(q->x, q->y);
				printf("■");
				p = p->next;
				q = q->next;
			}
			pos(q->x, q->y);
			printf("  ");
			free(q);
			p->next = NULL;
		}
	}
	if (status == D) {
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		if (nexthead->x == food->x && nexthead->y == food->y) {
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score += add;
			createfood();
		}
		else {
			nexthead->next = head;
			head = nexthead;
			p = head;
			q = head->next;
			while (q->next != NULL) {
				pos(q->x, q->y);
				printf("■");
				p = p->next;
				q = q->next;
			}
			pos(q->x, q->y);
			printf("  ");
			free(q);
			p->next = NULL;
		}
	}
	if (status == L) {
		nexthead->x = head->x - 2;
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y) {
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score += add;
			createfood();
		}
		else {
			nexthead->next = head;
			head = nexthead;
			p = head;
			q = head->next;
			while (q->next != NULL) {
				pos(q->x, q->y);
				printf("■");
				p = p->next;
				q = q->next;
			}
			pos(q->x, q->y);
			printf("  ");
			free(q);
			p->next = NULL;
		}
	}
	if (status == R) {
		nexthead->x = head->x + 2;
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y) {
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score += add;
			createfood();
		}
		else {
			nexthead->next = head;
			head = nexthead;
			p = head;
			q = head->next;
			while (q->next != NULL) {
				pos(q->x, q->y);
				printf("■");
				p = p->next;
				q = q->next;
			}
			pos(q->x, q->y);
			printf("  ");
			free(q);
			p->next = NULL;
		}
	}
	if (biteself()) {
		endgamestatus = 2;
		endgame();
	}
}

void pause(void) {
	while (1) {
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE)) {
			break;
		}
	}
}

void gamecircle(void) {
	pos(64, 15);
	printf("不能穿墙，不能咬到自己\n");
	pos(64, 16);
	printf("用↑.↓.←.→来控制蛇的移动");
	pos(64, 17);
	printf("F1为加速，F2为减速\n");
	pos(64, 18);
	printf("ESC：退出游戏  空格键：暂停游戏");
	pos(64, 20);
	printf("非正常人类研究中心");
	status = R;
	while (1) {
		pos(64, 10);
		printf("得分：%d", score);
		pos(64, 11);
		printf("每个食物得分：%d", add);
		if (GetAsyncKeyState(VK_UP) && status != D) {
			status = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != U) {
			status = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R) {
			status = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != L) {
			status = R;
		}
		else if (GetAsyncKeyState(VK_SPACE)) {
			pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			endgamestatus = 3;
			break;
		}
		else if (GetAsyncKeyState(VK_F1)) {
			if (sleeptime >= 50) {
				sleeptime -= 30;
				add += 2;
			}
		}
		else if (GetAsyncKeyState(VK_F2)) {
			if (sleeptime <= 290) {
				sleeptime -= 30;
				add -= 2;
			}
		}
		Sleep(sleeptime);
		snakemove();
	}
}

void welcometogame(void) {
	pos(40, 12);
	system("title 贪吃蛇");
	printf("欢迎来到贪吃蛇游戏！\n");
	pos(40, 25);
	system("pause");
	system("cls");
	pos(25, 12);
	printf("用↑.↓.←.→来控制蛇的移动\n");
	pos(25, 13);
	printf("F1为加速，F2为减速，加速能获得更高的分数\n");
	pos(25, 14);
	system("pause");
	system("cls");
}

void endgame(void) {
	system("cls");
	pos(24, 12);
	switch (endgamestatus) {
	case 1:printf("对不起，您撞墙了！"); break;
	case 2:printf("对不起，您撞到自己了！"); break;
	case 3:printf("您退出了游戏"); break;
	}
	pos(24, 13);
	printf("您的得分是%d！", score);
	exit(0);
}

void gamestart(void) {
	system("mode con cols=100 lines=30");
	welcometogame();
	creatMap();
	initsnake();
	createfood();
}

int main(void) {
	gamestart();
	gamecircle();
	endgame();
	return 0;
}