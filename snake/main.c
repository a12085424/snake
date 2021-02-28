#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>

#define keydown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)?1:0)
#define keyup(vk_code) ((GetAsyncKeyState(vk_code) & 0x0001)?1:0)

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
int score = 0, count = 0, add = 10; 
int status, sleeptime = 400; //每次运行的时间间隔
int mode = 0,gamestatus = 0;
snake *head, *food; //蛇头指针，实物指针
snake *q, *p; //遍历蛇用到的指针
int endgamestatus = 0; //游戏状态，1：撞墙；2：咬到自己；3：主动退出游戏

//声明全部函数
void pos(int x,int y);
void creatMap(int mode);
void initsnake(void);
int biteself(void);
void createfood(void);
int cantcrosswall(int mode);
void snakemove(void);
void pause(void);
void gamecircle(void);
void welcometogame(void);
int endgame(void);
void gamestart(void);

void pos(int x, int y) {   //设置光标位置,根据这个函数实现在屏幕上特定位置输出
	COORD pos;        //定义光标位置结构实例
	HANDLE hOutput;   //定义句柄实例
	pos.X = x;        
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE); //获取标准输出句柄
	SetConsoleCursorPosition(hOutput, pos); //根据x，y和句柄通过内置函数设定光标位置
}

void creatMap(int mode) {
	if (mode == 0) {
		int i;
		for (i = 0; i <= 56; i += 2) { //打印上下边框
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
	else if (mode == 1) {
		int i;
		for (i = 0; i <= 56; i += 2) { //打印上下边框
			pos(i, 0);
			printf("□");
			pos(i, 26);
			printf("□");
		}
		for (i = 1; i < 26; i++) { //打印左右边框
			pos(0, i);
			printf("□");
			pos(56, i);
			printf("□");
		}
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
	snake *self;
	self = head->next;
	while (self != NULL) {
		if (self->x == head->x && self->y == head->y) {
			endgamestatus = 2;
			return 1;
		}
		self = self->next;
	}
	return 0;
}

void createfood(void) {
	snake *food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	do  {                                 
		food_1->x = (rand() % 52) + 2;    //随机生成食物位置
	} while (((food_1->x % 2) != 0));
	food_1->y = (rand() % 24) + 1;
	q = head;
	while (q != NULL) {
		while (q->x == food_1->x && q->y == food_1->y) { //食物和身体重合就改变食物坐标
			food_1->x = ((food_1->x * food_1->x) % 52) + 2;
			food_1->y = ((food_1->y * food_1->y) % 24) + 1;
		}
		q = q->next;
	}
	pos(food_1->x, food_1->y);
	food = food_1;
	printf("●");
}

int cantcrosswall(int mode) {
	if (mode == 0) {
		if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26) {
			endgamestatus = 1;
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (mode == 1) {
		if (head->x == 56) {
			head->x = 2;
		}
		else if (head->x == 0) {
			head->x = 54;
		}
		else if (head->y == 26) {
			head->y = 1;
		}
		else if (head->y == 0) {
			head->y = 25;
		}
		return 0;
	}
}

void snakemove(void) {
	snake *nexthead;
	
	nexthead = (snake*)malloc(sizeof(snake));
	if (status == U) {
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		if (nexthead->x == food->x && nexthead->y == food->y) {
			nexthead->next = head;
			head = nexthead;
			if (cantcrosswall(mode) || biteself()) {
				return;
			}
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score += add;
			count++;
			createfood();
		}
		else {
			nexthead->next = head;
			head = nexthead;
			if (cantcrosswall(mode) || biteself()) {
				return;
			}
			p = head;
			q = head->next;
			while (q->next != NULL) {
				pos(p->x, p->y);
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
			if (cantcrosswall(mode) || biteself()) {
				return;
			}
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score += add;
			count++;
			createfood();
		}
		else {
			nexthead->next = head;
			head = nexthead;
			if (cantcrosswall(mode) || biteself()) {
				return;
			}
			p = head;
			q = head->next;
			while (q->next != NULL) {
				pos(p->x, p->y);
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
			if (cantcrosswall(mode) || biteself()) {
				return;
			}
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score += add;
			count++;
			createfood();
		}
		else {
			nexthead->next = head;
			head = nexthead;
			if (cantcrosswall(mode) || biteself()) {
				return;
			}
			p = head;
			q = head->next;
			while (q->next != NULL) {
				pos(p->x, p->y);
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
			if (cantcrosswall(mode) || biteself()) {
				return;
			}
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score += add;
			count++;
			createfood();
		}
		else {
			nexthead->next = head;
			head = nexthead;
			if (cantcrosswall(mode) || biteself()) {
				return;
			}
			p = head;
			q = head->next;
			while (q->next != NULL) {
				pos(p->x, p->y);
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

}

void pause(void) {
	int i;
	for (i = 26; i <= 34; i++) {
		pos(i, 12);
		printf("-");
		pos(i, 14);
		printf("-");
	}
	pos(25, 13);
	printf("|");
	pos(35, 13);
	printf("|");
	pos(26, 13);
	printf("暂停中...");
	pos(99, 29);
	while (1) {
		Sleep(300);
		if (GetAsyncKeyState(VK_ESCAPE)) {
			endgamestatus = 3;
			break;
		}
		if (GetAsyncKeyState(VK_SPACE)) {
			pos(25,12);
			printf("            ");
			pos(25,13);
			printf("            ");
			pos(25,14);
			printf("            ");
			break;
		}
		if (GetAsyncKeyState(0x4A) && sleeptime >= 50) {
			sleeptime -= 30;
			add += 2;
		}
		if (GetAsyncKeyState(0x4B) && add > 2) {
			sleeptime += 30;
			add -= 2;
		}
		pos(64, 7);
		printf("                       ");
		pos(64, 7);
		printf("每个食物得分：%d", add);
		pos(99, 29);
	}
	pos(food->x, food->y);
	food = food;
	printf("●");

}

void gamecircle(void) {
	while (1) {
		pos(64, 5);
		printf("得分：%d", score);
		pos(76, 5);
		printf("食物个数：%d", count);
		pos(64, 7);
		printf("                       ");
		pos(64, 7);
		printf("每个食物得分：%d", add);
		pos(99, 29);
		if (endgamestatus) {
			break;
		}
		Sleep(sleeptime);
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
		else if (GetAsyncKeyState(0x4A)) {
			if (sleeptime >= 50) {
				sleeptime -= 30;
				add += 2;
			}
		}
		else if (GetAsyncKeyState(0x4B)) {
			if (add > 2) {
				sleeptime += 30;
				add -= 2;
			}
		}
		snakemove();
	}
	
}

void welcometogame(void) {
	pos(30, 5);
	system("title 贪吃蛇");
	printf("欢迎来到贪吃蛇游戏！");
	pos(30, 12);
	printf("标准模式");
	pos(30, 14);
	printf("穿墙模式");
	pos(28, 12);
	printf("→");
	pos(99, 29);
	mode = 0;
	while (!(keyup(VK_RETURN))) {
		if (keyup(VK_DOWN) || keyup(VK_UP)) {       //根据上下选择模式
			mode = (mode + 1) % 2;
			pos(28, 12);
			printf("  ");
			pos(28, 14);
			printf("  ");
			if (mode == 0) {
				pos(28, 12);
				printf("→");
			}
			else if (mode == 1) {
				pos(28, 14);
				printf("→");
			}
		}
		pos(99, 29);

	}
	system("cls");
}

int endgame(void) {
	system("cls");
	pos(24, 12);
	switch (endgamestatus) {
	case 1:printf("对不起，您撞墙了！"); break;
	case 2:printf("对不起，您撞到自己了！"); break;
	case 3:printf("您退出了游戏"); break;
	}
	pos(24, 13);
	printf("您的得分是%d！", score);
	pos(24, 14);
	printf("按回车再玩一局！按ESC回到主菜单");
	pos(99, 29);
	endgamestatus = 0;
	sleeptime = 400;
	score = 0;
	count = 0;
	add = 10;
	while (1) {
		if (keyup(VK_ESCAPE)) {
			gamestatus = 0;
			break;
		}
		if (keyup(VK_RETURN)) {
			gamestatus = 1;
			break;
		}
	}
	system("cls");
}

void gamestart(void) {
	creatMap(mode);
	initsnake();
	createfood();
	
	if (mode == 0) {
		pos(64, 10);
		printf("当前模式：标准模式");
	}
	if (mode == 1) {
		pos(64, 10);
		printf("当前模式：穿墙模式");
	}
	pos(60, 12); printf("______________________________________");
	pos(64, 14);
	printf("用↑.↓.←.→来控制蛇的移动");
	pos(64, 17);printf("<J>：加速"); pos(76, 187); printf("<K>：减速");
	pos(64, 20);printf("<ESC>：退出"); pos(76, 20); printf("<空格键>：暂停");
	pos(90, 25);
	printf("by黄方");
	status = R;
}

int main(void) {
	system("mode con cols=100 lines=30");

	HWND hWnd = GetConsoleWindow(); //获得cmd窗口句柄
	RECT rc;
	GetWindowRect(hWnd, &rc); //获得cmd窗口对应矩形

	//改变cmd窗口风格
	SetWindowLongPtr(hWnd,
		GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	//因为风格涉及到边框改变，必须调用SetWindowPos，否则无效果
	SetWindowPos(hWnd,
		NULL,
		rc.left,
		rc.top,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL);


	while (gamestatus == 0) {
		welcometogame();
		gamestatus++;
		while (gamestatus == 1) {
			gamestart();
			gamecircle();
			endgame();
		}
	}
	return 0;
}