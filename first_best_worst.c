#include<stdio.h>
#include<stdlib.h>

typedef struct lei_item       //表示空闲分区表中的表箱
{
    int id;                 //假如 id 为-1，表示此分区时一个空闲分区。
    int base;              //指向分区的首地址
    int size;               //表示分区大小
    int status;             //表示此分区是否已经分配     0表示空闲  1表示已经分配
}Item;
typedef Item datatype;

typedef struct lei_list
{
    datatype* node;      //表示一个datatype类型的链表的结点
    struct lei_list* front;
    struct lei_list* next;
}List;

#define Max 640
int memory = Max;       //定义可用内存空间为640



List init(){            //初始化一个链表;
    List list;
    list.node = (datatype *)malloc(sizeof(datatype));
    list.node->base = 0;
    list.node->id = -1;                     //-1表示是空闲分区
    list.node->size = memory;
    list.node->status = 0;                  
    list.front = list.next = NULL;
    return list;
}

datatype* input(){          //初始化打算申请的内存分区节点
    datatype* item = (datatype *)malloc(sizeof(datatype));
    printf("请输入作业号：");
    scanf("%d",&item->id);
    printf("请输入所需要的内存的大小：");
    scanf("%d",&item->size);
    item->status = 0;
    return item;
}
void Momery_state(List *list){
    List* temp = list;
    printf("-----------------------------------\n");
    printf("内存分配状况\n");
        	printf("分区号\t起始\t大小\t状态\t\n"); 
    while (temp)
    {
        if(temp->node->status == 0 && temp->node->id == -1){
        	printf("FREE\t%d\t%d\t空闲\n",temp->node->base,temp->node->size);
        }
        else
        {
        	printf("%d\t%d\t%d\t占用\n",temp->node->id,temp->node->base,temp->node->size);
        }
        temp = temp->next;
    }
    printf("-----------------------------------\n");  
}

int First_fit(List *list){
    datatype* item = input();
    List* temp = list;                      //定义一个临时变量list* ，指向list

    while (temp)
    {

        if(temp->node->status == 0 && temp->node->size > item->size){       //如果此前的分区未分配，，并且分区大小大于 请求分配的大小     那么此时就可以进行分配
            List *front = temp->front;                                          //存储当前未分配分区的 上一个分区地址
            List *next = temp->next;                                          //存储当前未分配分区的  下一个分区地址   
            int base = temp->node->base;                                      //记录未分配当前分区的首地址

            datatype* new_node = (datatype*)malloc(sizeof(datatype));          // 多余出来的部分要新建立一个分区
            new_node->id = -1;                                                 //然后需要对这个新的分区进行一些信息的设置
            new_node->size = temp->node->size - item->size;         //新分区的大小  等于  还未分配的时的分区大小 - 请求分配的结点的大小 

            temp->node = item;                                  //对请求分配的分区结点进行分配
            temp->node->status = 1;

            new_node->status = 0;
            new_node->base = base + temp->node->size;             //新建立分区的首地址是  请求分配的分区的首地址 + 请求分配的分区的大小


            List* temp_next = (List*)malloc(sizeof(List));           //临时节点 （申请一个新的链表节点 表示下一个分区）  并且进行初始化
            temp_next->node = new_node;                             //保存下一个的分区的信息
            temp_next->front = temp_next->next = NULL;                                    

            if(front == NULL && next == NULL){                      //如果 front和next节点都是空，表明它是第一次分配分区
                temp->node->base = 0;                               //初始化首地址
                temp->next = temp_next;                     
                temp_next->front = temp;
            }
             if(front == NULL && next != NULL){                 //在第一个分区中插入新的分区
                 temp->node->base = 0;
                 temp->node->status = 1;
                temp_next->next = temp->next;
                temp->next = temp_next;
             }
            if(front != NULL){                      //表明不是第一次分配节点，此时需要在中间插入下一个节点
                temp->node->base = temp->front->node->base+temp->front->node->size;        //初始化首地址
                temp_next->next = temp->next;                                       //保证新插入的节点会记录原先节点的下一个节点的首地址
                temp_next->front = temp;                               // 首尾都需要保证
                temp->next = temp_next;                             //最后让所申请的分区节点的下一个节点指向  我们刚刚建立的临时节点
            }
            return 1;
        }   
        else if(temp->node->status == 0 && temp->node->size == item->size)
        {
            item->base = temp->front->node->base+temp->front->node->size;               //新插入分区的首地址  等于上一个分区的 首地址+分区的大小
            item->status = 1;                                           //表示已经分配
            temp->node = item;

            return 1;
        }
        else{
            temp = temp->next;
            continue;
        }
        temp = temp->next;
    }
    return 0;
}

int Momory_recycle(List *list){
    List* temp = list;                      //申请一个链表节点 指向list 的头节点
    int number;                         //用于存放要释放的节点的分区号
    printf("请输入需要回收的ID号：");
    scanf("%d",&number);
    while (temp)
    {   
        if(temp->node->id == number)            //首先找到 节点id = number 的节点，然后分四种情况讨论 
        {   
            // 一、 要回收的是第一个结点
            if(temp->front == NULL){
                temp->node->status = 0;
                temp->node->id = -1;
                if(temp->next == NULL){
                    temp->node->size = temp->node->size + temp->next->node->size;
                    temp->next = temp->next;
                    return 1;
                }
                
            if(temp->next->node->id == -1 && temp->next->node->status == 0){
                List* next = temp->next;
                // 此时来判断 temp->next 是否是系统的最后一个结点
                // 此时只将当前节点 和下一个结点合并就可以了
                //即 首地址不变，   分区状态 和 分区id进行变化  
                temp->node->size = temp->node->size + next->node->size;
                temp->node->status = 0;
                temp->node->id = -1;
                 temp->next = next->next;
                if(next->next == NULL){
                    free(next);
                    return 1;
                }
                //如果不是最后一个结点的话就会多一个步骤
                // 让 next->next->front 指向上一个结点
                else
                {
                    next->next->front = temp;
                    free(next);    
                    return 1;
                }       
            }
                
                return 1;
            }
              //二、 前后都没有空闲的分区
            //最简单，   直接改变 分区的 id 和 分区的状态就可以了。
            // 如果回收第一个分区的话 必须要先进行处理，如果不先进行处理 ，判断 temp->front->node->id != -1 会报一个段错误。因为temp-》front 此时指向的是null  
            if(temp->front->node->id != -1 && temp->front->node->status != 0 && temp->next->node->id != -1 && temp->next->node->status != 0){
                temp->node->status = 0;
                temp->node->id = -1;
                return 1;
            }
            //三、要回收的节点    前面和后面都是空闲的
            // 将三个空闲区合并到一起，起始地址为前面的分区的起始地址， 大小为三个空闲区大小之和
            //还需要做一个判断，如果
            if(temp->front->node->id == -1 && temp->front->node->status == 0 && temp->next->node->id == -1 && temp->next->node->status == 0){
                List* front = temp->front;
                List* next = temp->next;
                front->node->size = front->node->size + temp->node->size + next->node->size;    
                front->next = next->next;
                 if(next->next == NULL){
                    free(temp);
                    return 1;
                }
                //如果不是最后一个结点的话就会多一个步骤
                // 让 next->next->front 指向上一个结点
                else
                {
                    
                    next->next->front = front;
                    free(temp);  
                    return 1;
                }       
                return 1;
            }
            // 四、 要回收的节点  前面的节点是空闲的
            //合并后的分区起始地址为前一个结点， 分区大小为前一个节点 与 当前节点之和。
            if(temp->front->node->id == -1 && temp->front->node->status == 0){
                List* front = temp->front;
                front->next = temp->next;
                temp->next->front = front;
                front->node->size += temp->node->size;
                free(temp);
                return 1;
            }
            //五、 要回收的节点    后面的额节点是空闲的
            //合并后的分区首地址为当前节点 ，  分区大小为当前节点 与 当前节点的下一个结点大小之和。
            // 这个需要多一个步骤， 改变分区的 id 和  分区的状态。
            // 还要注意一点：  当要回收的空间是和  系统最后的空闲区相邻时 ， temp->next->next 指向的是null；

            if(temp->next->node->id == -1 && temp->next->node->status == 0){
                List* next = temp->next;
                // 此时来判断 temp->next 是否是系统的最后一个结点
                // 此时只将当前节点 和下一个结点合并就可以了
                //即 首地址不变，   分区状态 和 分区id进行变化  
                temp->node->size = temp->node->size + next->node->size;
                temp->node->status = 0;
                temp->node->id = -1;
                 temp->next = next->next;
                if(next->next == NULL){
                    free(next);
                    return 1;
                }
                //如果不是最后一个结点的话就会多一个步骤
                // 让 next->next->front 指向上一个结点
                else
                {
                    next->next->front = temp;
                    free(next);    
                    return 1;
                }       
            }
        }
        temp = temp->next;
    }
    return 0;
 }


int Best_fit(List *list){
	datatype* item = input();              // 要对 item 的 起始地址  和 分配状态进行初始化

	List* MaximumUnallocatedPartition = NULL;
	List* temp = list;
	for (; temp; temp = temp->next){
		//如果分区未分配且可以满足需要的空间，就要进行比较操作
		if (temp->node->status == 0 && temp->node->id == -1){
			//第一次找到未分配分区
			if (NULL == MaximumUnallocatedPartition){
				MaximumUnallocatedPartition = temp;
			}
			else{
				//不是第一次找到可用分区 MaximumUnallocatedPartition 已指向某个节点
				//将指向的节点与 MaximumUnallocatedPartition 比较，MaximumUnallocatedPartition 指向 size 大的节点
				MaximumUnallocatedPartition = temp->node->size > MaximumUnallocatedPartition->node->size ? temp : MaximumUnallocatedPartition;
			}
		}
	}

	if (NULL == MaximumUnallocatedPartition)//空间分配完毕，一个未分配分区都找不到
	{
		return 0;
	}
	if (MaximumUnallocatedPartition->node->size < item->size)//// 最大为分配分区小于需要的空间
	{
		return 0;
	}
	else if(MaximumUnallocatedPartition->node->size == item->size){// 最大为分配分区恰好等于需要的空间
		MaximumUnallocatedPartition->node->id = item->id;
		MaximumUnallocatedPartition->node->status = 1;
		return 1;
	}
	else{//最大为分配分区大于需要的空间
		//处理剩余空间，生成剩余空间节点
		List* residualSpaceNode = (List*)malloc(sizeof(List));         //新申请一个链表的结点并初始化
		residualSpaceNode->node = (datatype*)malloc(sizeof(datatype));
		residualSpaceNode->node->id = -1;
		residualSpaceNode->node->status = 0;
		residualSpaceNode->node->base = MaximumUnallocatedPartition->node->base + item->size;
		residualSpaceNode->node->size = MaximumUnallocatedPartition->node->size - item->size;
		//分配空间
		MaximumUnallocatedPartition->node->status = 1;
		MaximumUnallocatedPartition->node->id = item->id;
		MaximumUnallocatedPartition->node->size = item->size;
		//插入剩余空间节点
		residualSpaceNode->front = MaximumUnallocatedPartition;
		residualSpaceNode->next = MaximumUnallocatedPartition->next;
		if (MaximumUnallocatedPartition->next != NULL){
			MaximumUnallocatedPartition->next->front = residualSpaceNode;
		}
		MaximumUnallocatedPartition->next = residualSpaceNode;
		return 1;
	}
 }


int main(){
    List list = init();
    int select;
    int insert_state,recycle_state;
    int insert_state_best;
    do
    {
    	Momery_state(&list);
        printf("请输入要进行的操作\n");
        printf("1-首次适应算法， 2-最佳适应算法， 3-内存回收， 5-退出\n");
        scanf("%d",&select);
        
        
        switch (select)
        {
        case 1:              // 1. 首次适应算法
            insert_state = First_fit(&list);
			system("cls"); 
            if(insert_state == 0){
                printf("分配失败\n");
            }
            else {
                printf("分配成功！\n");
        
            }     
            break;
        case 2:             // 2. 最佳适应算法
            insert_state_best = Best_fit(&list);
			system("cls"); 
            if(insert_state_best == 1){
                printf("分配成功\n");
                
            }    
            else  {
                printf("分配失败\n");
            }        
            break;
        case 3:             //3.内存回收
            recycle_state = Momory_recycle(&list);
			system("cls"); 
            if(recycle_state == 1){
                printf("回收成功！\n");
                
            }
            else{
                printf("回收失败\n");
            }
            break;
        }
    } while (select != 5);

    system("pause");
}
