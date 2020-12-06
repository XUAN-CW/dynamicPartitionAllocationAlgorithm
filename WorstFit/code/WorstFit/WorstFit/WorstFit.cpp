#include <stdio.h>
#include <stdlib.h>
using namespace std;

struct  freelink {  //空闲区结构
	int len, address;
	// len 为分区长度  
	//address为分区起始地址 
};

struct  busylink { //占用区结构
	char name;   // 作业或进程名 name=’S’ 表示 OS 占用 
	int len, address;
};

struct QueueNode { //链表结构
	struct busylink Node; //占用参数和空闲参数相近，用占用结构作为链表结点
	struct QueueNode* next;
};

typedef struct {  //队列结构
	QueueNode* front; //队头
	QueueNode* rear;  //队尾
}LinkQueue;

//全局
LinkQueue* q_free; //自由链头结点
LinkQueue* q_busy; //占用链头结点

void requireMemo(char  name, int  require); /* 模拟内存分配 */
void freeMemo(char name); /* 模拟内存回收 */
int past(int time);   /* 模拟系统过了 time 时间 */
void printlink(LinkQueue* tempQ);
bool IsEmptyQueue(LinkQueue* q); //队列判空
void InitQueue(LinkQueue* q);    //分配内存
void InsertQueueNode(LinkQueue* q, struct busylink tempNode); //入队

void start()   /* 设置系统初始状态*/
{
	InitQueue(q_free);
	InitQueue(q_busy);

	//初始化操作系统占用64k后空闲链表状态
	struct QueueNode* p = (QueueNode*)malloc(sizeof(QueueNode));
	p->Node.address = 64; //操作系统占用0~63K空间，所以目前空闲区始址为64
	p->Node.len = 640 - 64;  //空闲区大小为640-64
	InsertQueueNode(q_free, p->Node);

	//初始化操作系统占用64k后占用链表状态
	struct QueueNode* q = (QueueNode*)malloc(sizeof(QueueNode));
	q->Node.name = 'S';
	q->Node.address = 0; //起初占用表始址为0
	q->Node.len = 64;   //操作系统占用64k
	InsertQueueNode(q_busy, q->Node);
}

int main()
{
	q_free = (LinkQueue*)malloc(sizeof(LinkQueue));  /* 创建自由链头结点  */
	q_busy = (LinkQueue*)malloc(sizeof(LinkQueue)); /* 创建占用链头结点  */

	start();
	past(5);
	printf("t1时刻后：\n");
	requireMemo('A', 8);
	requireMemo('B', 16);
	requireMemo('C', 64);
	requireMemo('D', 124);
	printlink(q_free);

	past(5);
	printf("t2时刻后：\n");
	freeMemo('C');
	printlink(q_free);

	past(5);
	printf("t3时刻后：\n");
	//requireMemo('E', 50);
	requireMemo('E', 364);
	printlink(q_free);

	past(5);
	printf("t4时刻后：\n");
	freeMemo('D');
	printlink(q_free);

	system("pause");
	return 0;
}

void requireMemo(char name, int require) /* 模拟内存分配 */
{
	struct QueueNode* w = q_free->front->next;
	if (w->Node.len<require) //若申请内存空间大于空闲区最大空间
	{
		printf("Can’t allocate"); //内存不足，无法分配
		return; //直接退出
	}

	//申请空间小于或等于空闲区最大空间，则允许申请内存
	struct QueueNode* p = (struct QueueNode*)malloc(sizeof(struct QueueNode));
	p->Node.name = name;
	p->Node.address = w->Node.address; //最大空间始址作为申请空间始址
	p->Node.len = require;
	p->next = NULL;
	InsertQueueNode(q_busy, p->Node);

	//无论申请空间小于或等于空闲区最大空间，都要删除最大空间
	q_free->front->next = w->next;

	//若申请空间等于最大空间
	if (w->Node.len == require)
	{
		//释放临时结点w
		free(w);
		return;
	}

	//若申请空间小于最大空间,分配后,需要计算剩余空闲区大小和起始地址
	w->Node.address += require; //始址后移
	w->Node.len -= require;

	//排序操作
	//如果最大空闲区分配后剩余依然最大,可以直接放在第一个位置,否则和后面的空闲区大小比较放在合适位置
	struct QueueNode* u = q_free->front;
	struct QueueNode* v = u->next;
	while (v && v->Node.len > w->Node.len)
	{
		u = v;
		v = v->next;
	}
	u->next = w;
	w->next = v;
}


void freeMemo(char name)
{
	struct QueueNode* q = q_busy->front;
	struct QueueNode* p = q_busy->front->next;

	while (p && p->Node.name != name)
	{ //查找名字相同的结点
		q = p;
		p = q->next;
	}

	if (!p) //如果为空，说明没有对应结点，直接退出
	{
		printf("%c is not exist", name);
		return;
	}

	//如果找到了,释放申请的内存
	if (p == q_busy->rear)  q_busy->rear = q;
	int len = p->Node.len; //释放前需要获取该结点的空间大小和始址
	int address = p->Node.address;
	q->next = p->next; //删除结点
	free(p); //释放内存 

	//把上面获取是空间和始址作为空闲内存块插入空闲链表
	struct QueueNode* w = (struct QueueNode*)malloc(sizeof(struct QueueNode));
	w->Node.len = len;
	w->Node.address = address;

	//回收的结点插入到空闲链表合适位置
	struct QueueNode* u = q_free->front;
	struct QueueNode* v = q_free->front->next;
	while (v && v->Node.len > w->Node.len)
	{
		u = v;
		v = v->next;
	}
	u->next = w;
	w->next = v;
}

int past(int time)
{
	return time;
}


void printlink(LinkQueue* q) //打印
{
	struct QueueNode* p = q->front->next;
	while (p) {
		printf("空闲大小%d 起始地址%d\n", p->Node.len, p->Node.address);
		p = p->next;
	}
	printf("\n");
}

void InitQueue(LinkQueue* q) { //初始化头结点
	q->front = q->rear = (QueueNode*)malloc(sizeof(QueueNode));
	q->front->next = NULL;
}

bool IsEmptyQueue(LinkQueue* q) { //判空
	if (q->front == q->rear)
		return true;
	return false;
}

void InsertQueueNode(LinkQueue* q, struct busylink tempNode) { //插入结点
	QueueNode* P = (QueueNode*)malloc(sizeof(QueueNode));
	P->Node = tempNode;
	P->next = NULL;
	q->rear->next = P;
	q->rear = P;
}
