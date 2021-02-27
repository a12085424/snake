#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>

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
int score = 0, add = 10; 
int status, sleeptime = 400; //ÿ�����е�ʱ����
snake *head, *food; //��ͷָ�룬ʵ��ָ��
snake *q, *p; //�������õ���ָ��
int endgamestatus = 0; //��Ϸ״̬��1��ײǽ��2��ҧ���Լ���3�������˳���Ϸ

//����ȫ������
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

void pos(int x, int y) {   //���ù��λ��,�����������ʵ������Ļ���ض�λ�����
	COORD pos;        //������λ�ýṹʵ��
	HANDLE hOutput;   //������ʵ��
	pos.X = x;        
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ��׼������
	SetConsoleCursorPosition(hOutput, pos); //����x��y�;��ͨ�����ú����趨���λ��
}

void creatMap(void) {
	int i;
	for (i = 0; i < 58; i += 2) { //��ӡ���±߿�
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
	while ((food_1->x % 2) != 0) {         //�������ʳ��λ��
		food_1->x = rand() % 52 + 2;
	}
	food_1->y = rand() % 24 + 1;
	q = head;
	while (q != NULL) {
		if (q->x == food_1->x && q->y == food_1->y) { //ʳ��������غϾ�������һ��
			free(food_1);
			food_1 = createfood();
			break;
		}
		q = q->next;
	}
	pos(food_1->x, food_1->y);
	food = food_1;
	printf("��");
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
				printf("��");
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
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("��");
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
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("��");
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
			q = head;
			while (q != NULL) {
				pos(q->x, q->y);
				printf("��");
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
	printf("���ܴ�ǽ������ҧ���Լ�\n");
	pos(64, 16);
	printf("�á�.��.��.���������ߵ��ƶ�");
	pos(64, 17);
	printf("F1Ϊ���٣�F2Ϊ����\n");
	pos(64, 18);
	printf("ESC���˳���Ϸ  �ո������ͣ��Ϸ");
	pos(64, 20);
	printf("�����������о�����");
	status = R;
	while (1) {
		pos(64, 10);
		printf("�÷֣�%d", score);
		pos(64, 11);
		printf("ÿ��ʳ��÷֣�%d", add);
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
	system("title ̰����");
	printf("��ӭ����̰������Ϸ��\n");
	pos(40, 25);
	system("pause");
	system("cls");
	pos(25, 12);
	printf("�á�.��.��.���������ߵ��ƶ�\n");
	pos(25, 13);
	printf("F1Ϊ���٣�F2Ϊ���٣������ܻ�ø��ߵķ���\n");
	pos(25, 14);
	system("pause");
	system("cls");
}

void endgame(void) {
	system("cls");
	pos(24, 12);
	switch (endgamestatus) {
	case 1:printf("�Բ�����ײǽ�ˣ�"); break;
	case 2:printf("�Բ�����ײ���Լ��ˣ�"); break;
	case 3:printf("���˳�����Ϸ"); break;
	}
	pos(24, 13);
	printf("���ĵ÷���%d��", score);
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