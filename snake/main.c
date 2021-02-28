#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>

#define keydown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)?1:0)
#define keyup(vk_code) ((GetAsyncKeyState(vk_code) & 0x0001)?1:0)

//�ߵ�״̬��U�ϣ�D�£�L��R��
#define U 1
#define D 2
#define L 3
#define R 4


typedef struct SNAKE {     //������ṹ�����һ���ڵ�
	int x;
	int y;
	struct SNAKE *next;
} snake;

//ȫ�ֱ���
int score = 0, count = 0, add = 10; 
int status, sleeptime = 400; //ÿ�����е�ʱ����
int mode = 0,gamestatus = 0;
snake *head, *food; //��ͷָ�룬ʵ��ָ��
snake *q, *p; //�������õ���ָ��
int endgamestatus = 0; //��Ϸ״̬��1��ײǽ��2��ҧ���Լ���3�������˳���Ϸ

//����ȫ������
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

void pos(int x, int y) {   //���ù��λ��,�����������ʵ������Ļ���ض�λ�����
	COORD pos;        //������λ�ýṹʵ��
	HANDLE hOutput;   //������ʵ��
	pos.X = x;        
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ��׼������
	SetConsoleCursorPosition(hOutput, pos); //����x��y�;��ͨ�����ú����趨���λ��
}

void creatMap(int mode) {
	if (mode == 0) {
		int i;
		for (i = 0; i <= 56; i += 2) { //��ӡ���±߿�
			pos(i, 0);
			printf("��");
			pos(i, 26);
			printf("��");
		}
		for (i = 1; i < 26; i++) { //��ӡ���ұ߿�
			pos(0, i);
			printf("��");
			pos(56, i);
			printf("��");
		}
	}
	else if (mode == 1) {
		int i;
		for (i = 0; i <= 56; i += 2) { //��ӡ���±߿�
			pos(i, 0);
			printf("��");
			pos(i, 26);
			printf("��");
		}
		for (i = 1; i < 26; i++) { //��ӡ���ұ߿�
			pos(0, i);
			printf("��");
			pos(56, i);
			printf("��");
		}
	}
	
}

void initsnake(void) { //��ʼ������
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake)); //����β��ʼ��ͷ�巨����x,y�趨��ʼ��λ��
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
		printf("��");
		tail = tail->next;
	}
}


int biteself(void) {    //����self�ڵ��ͷ��ʼ�������壬�ж�ͷ��λ���Ƿ���ͬһ��
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
		food_1->x = (rand() % 52) + 2;    //�������ʳ��λ��
	} while (((food_1->x % 2) != 0));
	food_1->y = (rand() % 24) + 1;
	q = head;
	while (q != NULL) {
		while (q->x == food_1->x && q->y == food_1->y) { //ʳ��������غϾ͸ı�ʳ������
			food_1->x = ((food_1->x * food_1->x) % 52) + 2;
			food_1->y = ((food_1->y * food_1->y) % 24) + 1;
		}
		q = q->next;
	}
	pos(food_1->x, food_1->y);
	food = food_1;
	printf("��");
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
				printf("��");
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
				printf("��");
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
				printf("��");
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
				printf("��");
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
				printf("��");
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
				printf("��");
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
				printf("��");
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
				printf("��");
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
	printf("��ͣ��...");
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
		printf("ÿ��ʳ��÷֣�%d", add);
		pos(99, 29);
	}
	pos(food->x, food->y);
	food = food;
	printf("��");

}

void gamecircle(void) {
	while (1) {
		pos(64, 5);
		printf("�÷֣�%d", score);
		pos(76, 5);
		printf("ʳ�������%d", count);
		pos(64, 7);
		printf("                       ");
		pos(64, 7);
		printf("ÿ��ʳ��÷֣�%d", add);
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
	system("title ̰����");
	printf("��ӭ����̰������Ϸ��");
	pos(30, 12);
	printf("��׼ģʽ");
	pos(30, 14);
	printf("��ǽģʽ");
	pos(28, 12);
	printf("��");
	pos(99, 29);
	mode = 0;
	while (!(keyup(VK_RETURN))) {
		if (keyup(VK_DOWN) || keyup(VK_UP)) {       //��������ѡ��ģʽ
			mode = (mode + 1) % 2;
			pos(28, 12);
			printf("  ");
			pos(28, 14);
			printf("  ");
			if (mode == 0) {
				pos(28, 12);
				printf("��");
			}
			else if (mode == 1) {
				pos(28, 14);
				printf("��");
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
	case 1:printf("�Բ�����ײǽ�ˣ�"); break;
	case 2:printf("�Բ�����ײ���Լ��ˣ�"); break;
	case 3:printf("���˳�����Ϸ"); break;
	}
	pos(24, 13);
	printf("���ĵ÷���%d��", score);
	pos(24, 14);
	printf("���س�����һ�֣���ESC�ص����˵�");
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
		printf("��ǰģʽ����׼ģʽ");
	}
	if (mode == 1) {
		pos(64, 10);
		printf("��ǰģʽ����ǽģʽ");
	}
	pos(60, 12); printf("______________________________________");
	pos(64, 14);
	printf("�á�.��.��.���������ߵ��ƶ�");
	pos(64, 17);printf("<J>������"); pos(76, 187); printf("<K>������");
	pos(64, 20);printf("<ESC>���˳�"); pos(76, 20); printf("<�ո��>����ͣ");
	pos(90, 25);
	printf("by�Ʒ�");
	status = R;
}

int main(void) {
	system("mode con cols=100 lines=30");

	HWND hWnd = GetConsoleWindow(); //���cmd���ھ��
	RECT rc;
	GetWindowRect(hWnd, &rc); //���cmd���ڶ�Ӧ����

	//�ı�cmd���ڷ��
	SetWindowLongPtr(hWnd,
		GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	//��Ϊ����漰���߿�ı䣬�������SetWindowPos��������Ч��
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