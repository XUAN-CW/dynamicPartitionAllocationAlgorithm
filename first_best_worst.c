#include<stdio.h>
#include<stdlib.h>

typedef struct lei_item       //��ʾ���з������еı���
{
    int id;                 //���� id Ϊ-1����ʾ�˷���ʱһ�����з�����
    int base;              //ָ��������׵�ַ
    int size;               //��ʾ������С
    int status;             //��ʾ�˷����Ƿ��Ѿ�����     0��ʾ����  1��ʾ�Ѿ�����
}Item;
typedef Item datatype;

typedef struct lei_list
{
    datatype* node;      //��ʾһ��datatype���͵�����Ľ��
    struct lei_list* front;
    struct lei_list* next;
}List;

#define Max 640
int memory = Max;       //��������ڴ�ռ�Ϊ640



List init(){            //��ʼ��һ������;
    List list;
    list.node = (datatype *)malloc(sizeof(datatype));
    list.node->base = 0;
    list.node->id = -1;                     //-1��ʾ�ǿ��з���
    list.node->size = memory;
    list.node->status = 0;                  
    list.front = list.next = NULL;
    return list;
}

datatype* input(){          //��ʼ������������ڴ�����ڵ�
    datatype* item = (datatype *)malloc(sizeof(datatype));
    printf("��������ҵ�ţ�");
    scanf("%d",&item->id);
    printf("����������Ҫ���ڴ�Ĵ�С��");
    scanf("%d",&item->size);
    item->status = 0;
    return item;
}
void Momery_state(List *list){
    List* temp = list;
    printf("-----------------------------------\n");
    printf("�ڴ����״��\n");
    printf("-----------------------------------\n");
    while (temp)
    {
        if(temp->node->status == 0 && temp->node->id == -1){
            printf("�����ţ�FREE\n");
            printf("��ʼ��ַ��%d\n",temp->node->base);
            printf("�ڴ��С��%d\n",temp->node->size);
            printf("����״̬������\n");
        }
        else
        {
            printf("�����ţ�%d\t��ʼ��ַ��%d\n",temp->node->id,temp->node->base);
            printf("�ڴ��С��%d\n",temp->node->size);
            printf("����״̬���ѷ���\n");
        }
        printf("-----------------------------------\n");
        temp = temp->next;
    }

}

int First_fit(List *list){
    datatype* item = input();
    List* temp = list;                      //����һ����ʱ����list* ��ָ��list

    while (temp)
    {

        if(temp->node->status == 0 && temp->node->size > item->size){       //�����ǰ�ķ���δ���䣬�����ҷ�����С���� �������Ĵ�С     ��ô��ʱ�Ϳ��Խ��з���
            List *front = temp->front;                                          //�洢��ǰδ��������� ��һ��������ַ
            List *next = temp->next;                                          //�洢��ǰδ���������  ��һ��������ַ   
            int base = temp->node->base;                                      //��¼δ���䵱ǰ�������׵�ַ

            datatype* new_node = (datatype*)malloc(sizeof(datatype));          // ��������Ĳ���Ҫ�½���һ������
            new_node->id = -1;                                                 //Ȼ����Ҫ������µķ�������һЩ��Ϣ������
            new_node->size = temp->node->size - item->size;         //�·����Ĵ�С  ����  ��δ�����ʱ�ķ�����С - �������Ľ��Ĵ�С 

            temp->node = item;                                  //���������ķ��������з���
            temp->node->status = 1;

            new_node->status = 0;
            new_node->base = base + temp->node->size;             //�½����������׵�ַ��  �������ķ������׵�ַ + �������ķ����Ĵ�С


            List* temp_next = (List*)malloc(sizeof(List));           //��ʱ�ڵ� ������һ���µ�����ڵ� ��ʾ��һ��������  ���ҽ��г�ʼ��
            temp_next->node = new_node;                             //������һ���ķ�������Ϣ
            temp_next->front = temp_next->next = NULL;                                    

            if(front == NULL && next == NULL){                      //��� front��next�ڵ㶼�ǿգ��������ǵ�һ�η������
                temp->node->base = 0;                               //��ʼ���׵�ַ
                temp->next = temp_next;                     
                temp_next->front = temp;
            }
             if(front == NULL && next != NULL){                 //�ڵ�һ�������в����µķ���
                 temp->node->base = 0;
                 temp->node->status = 1;
                temp_next->next = temp->next;
                temp->next = temp_next;
             }
            if(front != NULL){                      //�������ǵ�һ�η���ڵ㣬��ʱ��Ҫ���м������һ���ڵ�
                temp->node->base = temp->front->node->base+temp->front->node->size;        //��ʼ���׵�ַ
                temp_next->next = temp->next;                                       //��֤�²���Ľڵ���¼ԭ�Ƚڵ����һ���ڵ���׵�ַ
                temp_next->front = temp;                               // ��β����Ҫ��֤
                temp->next = temp_next;                             //�����������ķ����ڵ����һ���ڵ�ָ��  ���Ǹոս�������ʱ�ڵ�
            }
            return 1;
        }   
        else if(temp->node->status == 0 && temp->node->size == item->size)
        {
            item->base = temp->front->node->base+temp->front->node->size;               //�²���������׵�ַ  ������һ�������� �׵�ַ+�����Ĵ�С
            item->status = 1;                                           //��ʾ�Ѿ�����
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
    List* temp = list;                      //����һ������ڵ� ָ��list ��ͷ�ڵ�
    int number;                         //���ڴ��Ҫ�ͷŵĽڵ�ķ�����
    printf("��������Ҫ���յ�ID�ţ�");
    scanf("%d",&number);
    while (temp)
    {   
        if(temp->node->id == number)            //�����ҵ� �ڵ�id = number �Ľڵ㣬Ȼ�������������� 
        {   
            // һ�� Ҫ���յ��ǵ�һ�����
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
                // ��ʱ���ж� temp->next �Ƿ���ϵͳ�����һ�����
                // ��ʱֻ����ǰ�ڵ� ����һ�����ϲ��Ϳ�����
                //�� �׵�ַ���䣬   ����״̬ �� ����id���б仯  
                temp->node->size = temp->node->size + next->node->size;
                temp->node->status = 0;
                temp->node->id = -1;
                 temp->next = next->next;
                if(next->next == NULL){
                    free(next);
                    return 1;
                }
                //����������һ�����Ļ��ͻ��һ������
                // �� next->next->front ָ����һ�����
                else
                {
                    next->next->front = temp;
                    free(next);    
                    return 1;
                }       
            }
                
                return 1;
            }
              //���� ǰ��û�п��еķ���
            //��򵥣�   ֱ�Ӹı� ������ id �� ������״̬�Ϳ����ˡ�
            // ������յ�һ�������Ļ� ����Ҫ�Ƚ��д���������Ƚ��д��� ���ж� temp->front->node->id != -1 �ᱨһ���δ�����Ϊtemp-��front ��ʱָ�����null  
            if(temp->front->node->id != -1 && temp->front->node->status != 0 && temp->next->node->id != -1 && temp->next->node->status != 0){
                temp->node->status = 0;
                temp->node->id = -1;
                return 1;
            }
            //����Ҫ���յĽڵ�    ǰ��ͺ��涼�ǿ��е�
            // �������������ϲ���һ����ʼ��ַΪǰ��ķ�������ʼ��ַ�� ��СΪ������������С֮��
            //����Ҫ��һ���жϣ����
            if(temp->front->node->id == -1 && temp->front->node->status == 0 && temp->next->node->id == -1 && temp->next->node->status == 0){
                List* front = temp->front;
                List* next = temp->next;
                front->node->size = front->node->size + temp->node->size + next->node->size;    
                front->next = next->next;
                 if(next->next == NULL){
                    free(temp);
                    return 1;
                }
                //����������һ�����Ļ��ͻ��һ������
                // �� next->next->front ָ����һ�����
                else
                {
                    
                    next->next->front = front;
                    free(temp);  
                    return 1;
                }       
                return 1;
            }
            // �ġ� Ҫ���յĽڵ�  ǰ��Ľڵ��ǿ��е�
            //�ϲ���ķ�����ʼ��ַΪǰһ����㣬 ������СΪǰһ���ڵ� �� ��ǰ�ڵ�֮�͡�
            if(temp->front->node->id == -1 && temp->front->node->status == 0){
                List* front = temp->front;
                front->next = temp->next;
                temp->next->front = front;
                front->node->size += temp->node->size;
                free(temp);
                return 1;
            }
            //�塢 Ҫ���յĽڵ�    ����Ķ�ڵ��ǿ��е�
            //�ϲ���ķ����׵�ַΪ��ǰ�ڵ� ��  ������СΪ��ǰ�ڵ� �� ��ǰ�ڵ����һ������С֮�͡�
            // �����Ҫ��һ�����裬 �ı������ id ��  ������״̬��
            // ��Ҫע��һ�㣺  ��Ҫ���յĿռ��Ǻ�  ϵͳ���Ŀ���������ʱ �� temp->next->next ָ�����null��

            if(temp->next->node->id == -1 && temp->next->node->status == 0){
                List* next = temp->next;
                // ��ʱ���ж� temp->next �Ƿ���ϵͳ�����һ�����
                // ��ʱֻ����ǰ�ڵ� ����һ�����ϲ��Ϳ�����
                //�� �׵�ַ���䣬   ����״̬ �� ����id���б仯  
                temp->node->size = temp->node->size + next->node->size;
                temp->node->status = 0;
                temp->node->id = -1;
                 temp->next = next->next;
                if(next->next == NULL){
                    free(next);
                    return 1;
                }
                //����������һ�����Ļ��ͻ��һ������
                // �� next->next->front ָ����һ�����
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
     int min = 0;        //��¼ ��С�����Ľ��Ĵ�С
     int base_min = 0;      //��¼ ��С�ڵ�Ľ�����ʼ��ַ
     List* temp = list; 
     datatype* item = input();              // Ҫ�� item �� ��ʼ��ַ  �� ����״̬���г�ʼ��
    
     while (temp)
     {
         //�������δ����   ��Ҫ����  �Ƚϲ����� ���Ҽ�¼��ֵ �� ������id��
         if(temp->node->status == 0 && temp->node->id == -1&& temp->node->size > item->size){
             if(min == 0){          //����minΪ0 ��ʾ��δ�ҵ�һ�����Է���ķ���
                 min = temp->node->size;
                 base_min = temp->node->base;
             }
             else
             {
                 if(temp->node->size < min){      // �ҵ�һ��֮����Ҫ�ҳ���С�ķ���  Ҳ��������  size��С��
                     min = temp->node->size;
                     base_min = temp->node->base;
                 }
             }
             
         }
         if(temp->node->status == 0 && temp->node->id == -1 && temp->node->size == item->size){
             int base = temp->node->base;
             temp->node = item;
             temp->node->status = 1;
             temp->node->base = base;
             return 1;
         }
        temp = temp->next;
     }

     //��Ϊ����û���κ�һ���ռ��������Ҫ����Ҫ��һ���жϴ���   
     temp = list;
     while (temp)
     {
         if(temp->node->base == base_min){

            datatype* temp_node = (datatype*)malloc(sizeof(datatype));      //���ж���Ŀռ�����  ������Ҫ�ڽ���һ�������뵽������
            temp_node->id = -1;
            temp_node->status = 0;
            temp_node->base = base_min + item->size;
            temp_node->size = temp->node->size - item->size;

            temp->node = item;                          //��item���������ĳ�ʼ��
            temp->node->base = base_min;
            temp->node->status = 1;
            
            List* temp_list_node = (List*)malloc(sizeof(List));         //������һ�� ����Ľ�� ���ҳ�ʼ��
            temp_list_node->node = temp_node;
            temp_list_node->front = temp;
            temp_list_node->next = temp->next;
            if(temp->next != NULL){
                temp->next->front = temp_list_node;
            }
            temp->next = temp_list_node;
            return 1;
         }
         temp = temp->next;
     }
       
 }


int main(){
    printf("����ģ��\n");
    List list = init();
    int select;
    int insert_state,recycle_state;
    int insert_state_best;
    do
    {
        printf("������Ҫ���еĲ���\n");
        printf("1-�״���Ӧ�㷨�� 2-�����Ӧ�㷨�� 3-�ڴ���գ� 4-��ʾ�ڴ�״���� 5-�˳�\n");
        scanf("%d",&select);
        switch (select)
        {
        case 1:              // 1. �״���Ӧ�㷨
            insert_state = First_fit(&list);
            if(insert_state == 0){
                printf("����ʧ��\n");
            }
            else {
                printf("����ɹ���\n");
            }     
            break;
        case 2:             // 2. �����Ӧ�㷨
            insert_state_best = Best_fit(&list);
            if(insert_state_best == 1){
                printf("����ɹ�\n");
            }    
            else  {
                printf("����ʧ��\n");
            }        
            break;
        case 3:             //3.�ڴ����
            recycle_state = Momory_recycle(&list);
            if(recycle_state == 1){
                printf("���ճɹ���\n");
            }
            else{
                printf("����ʧ��\n");
            }
            break;
        case 4:             //4.��ʾ�ڴ�״��
            Momery_state(&list);
            break;          
        }
    } while (select != 5);

    system("pause");
}
