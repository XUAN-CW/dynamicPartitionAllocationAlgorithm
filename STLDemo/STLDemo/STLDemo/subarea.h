typedef struct process     //表示进程
{
	int PID;                 //进程 ID
	int startAddress;       //指向分区的首地址
	int size;               //表示分区大小
	int status;             //表示此分区是否已经分配     0表示空闲  1表示已经分配
}process;

#define MAX 640   //从 0 开始，内存大小为 640 KB，最大地址为 640-1