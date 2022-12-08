#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// construct linked list node
struct list_node
{
    // the data in the node of linked list
    int data;
    // the pointer of the next node 
    struct list_node * next;
};

typedef struct list_node list_single;

list_single *list_node_creat(int data)
{
    // first define the header pointer
    list_single * node = NULL;
    // allocate space 
    node = (list_single *)malloc(sizeof(list_single));
    if (node == NULL)
        printf("malloc failed\n");
    // set zero for node
    memset(node,0,sizeof(list_single));
    node->data = data;
    node->next = NULL;
    return node;
}

void node_append(list_single *ph_LList,list_single *new)
{
    list_single *p = ph_LList;
    // find the end pointer of linked list
    while (p->next != NULL)
    {
        p = p->next;
    }
    // append the new node to linked list
    p->next = new;
}


void node_head_append(list_single *ph_LList,list_single *new)
{
    list_single *p = ph_LList;
    new->next = p->next;
    p->next = new;
}

void print_allnode(list_single *ph_LList)
{
    list_single *p =ph_LList;
    
}
void node_insert(list_single *ph_LList,list_single * new)
{
    // get the pointer of insert position
    list_single * p = ph_LList;
    // 
    
    


}

int main(){
    
    printf("%d\n",node->data);
    free(node);
    return 0;
}