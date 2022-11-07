// Collaborators:
// Hardik Aggawal (2021CSB1173)
// Harshit Kumar Ravi (2021CSB1093)
// Gyanendra Mani (2021CSB1090)

// Teaching Assistant: Vinay Kumar Sajja

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

/* Structure definition of the fibonacci heap node */

typedef struct node
{
    int key;
    struct node *parent;
    struct node *child;
    struct node *left;
    struct node *right;
    int degree;
    bool mark;
    bool found;
} node;



/* Structure definition of the fibonacci heap */

typedef struct heap
{
    node *min;
    int n;
} heap;




/*This function makes a fibonacci heap pointer dynamically 
  and assign values to its variables */

heap *MAKE_HEAP()
{
    heap* temp=(heap*)malloc(sizeof(heap));
    temp->min=NULL;
    temp->n=0;
    return temp;
}



/*This function allocate memory of the node dynamically 
  and assign values to its variables*/

node *allocateMemoryNode()
{
    node *temp = (node *)malloc(sizeof(node));
    temp->degree = 0;
    temp->parent = NULL;
    temp->child = NULL;
    temp->mark = false;
    temp->found=false;
    return temp;
}



/*This function insert the node in the root list 
  which is represented as doubly linked list*/

void insert(heap* fibbHeap,node* newNode)
{
    if (fibbHeap->min == NULL)
        fibbHeap->min = newNode;
    else
    {
        if (fibbHeap->min->right == fibbHeap->min)
        {
            fibbHeap->min->left=newNode;
            fibbHeap->min->right = newNode;
            newNode->left=fibbHeap->min;
            newNode->right=fibbHeap->min;
        }
        else
        {
            newNode->right=fibbHeap->min->right;
            newNode->left=fibbHeap->min;
            fibbHeap->min->right->left=newNode;
            fibbHeap->min->right=newNode;
        }
        if(newNode->key<fibbHeap->min->key)
            fibbHeap->min=newNode;
    }
}



/*This function insert a node in the fibonacci heap */

void FIB_HEAP_INSERT(heap* fibHeap,int key)
{
    node *newNode = allocateMemoryNode();
    newNode->key = key;
    newNode->right=newNode;
    newNode->left=newNode;
    insert(fibHeap,newNode);
    fibHeap->n=fibHeap->n+1;
}



/*This function make a fibonacci heap which is union of two fibonacci heaps (h1 and h2)
  It also destroys h1 and h2 in the process*/

heap* FIB_HEAP_UNION(heap*h1,heap*h2)
{
    heap*temp=MAKE_HEAP();
    temp->min=h1->min;
    temp->n=h1->n+h2->n;
    if(h1->min != NULL && h2->min != NULL){
        node* temp1=h1->min->right;
        node* temp2=h2->min->left;
        h1->min->right=h2->min;
        h2->min->left=h1->min;
        temp1->left=temp2;
        temp2->right=temp1;}
    if (h1->min == NULL || (h2->min != NULL && (h2->min->key < h1->min->key)))
        temp->min=h2->min;
    return temp;
}



/*This function is used to cut the node from its parent and insert it in the root list*/

void CUT(heap* h,node* child,node* parent){
    if(parent->child==child){
        if(child->right==child)
            parent->child=NULL;
        else
            parent->child=child->right;
    }
    if(parent->child!=NULL){
        child->right->left=child->left;
        child->left->right=child->right;
    }
    insert(h,child);
    child->parent=NULL;
    child->mark=false;
    parent->degree--;
}



/*This function recusively cut the child from parent 
  until the child is an unmarked node or it has no parent*/

void CASCADING_CUT(heap* fibHeap,node* y)
{
    node* parent=y->parent;
    if(parent!=NULL)
    {
        if(y->mark==false)
            y->mark=true;
        else
        {
            CUT(fibHeap,y,parent);
            CASCADING_CUT(fibHeap,parent);
        }
        
    }
}



/*This function is used to decrease the key value of a node to a given value*/

void FIB_HEAP_DECREASE_KEY(heap* fibHeap, node* oldNode, int newKey)
{
    if (fibHeap->min==NULL)
    {
        printf("\nHeap empty.");
    }
    else if(oldNode->key < newKey)
    {
        printf("\nThe new value is greater than the current value.");
    }
    else
    {
        oldNode->key=newKey;
        node* temp=oldNode->parent;
        if( temp!=NULL && ((oldNode->key)<(temp->key)) ) 
        {
            CUT(fibHeap,oldNode,temp);
            CASCADING_CUT(fibHeap,temp);
        }
        if( oldNode->key < fibHeap->min->key )
        {
            fibHeap->min=oldNode;
        }
    }
}



/* This assists the function CONSOLIDATE in merging same degree root list nodes*/

void FIB_LINK(heap* fibHeap,node* p2, node* p1)
{
    p2->left->right=p2->right;
    p2->right->left=p2->left;
    if(p1->right==p1)
    {
        fibHeap->min=p1;
    }
    p2->left=p2;
    p2->right=p2;
    p2->parent=p1;
    if(p1->child==NULL)
    {
        p1->child=p2;
    }
    else
    {
        p2->right=p1->child;
        p2->left=p1->child->left;
        p1->child->left->right=p2;
        p1->child->left=p2;
    }
    if(p2->key<p1->child->key)
    {
        p1->child = p2;
    }
    p1->degree++;
    p2->mark=false;
}



/*This function finds the maximum number of nodes in the fibonacci heap*/

int degree(heap*h){
    return (int)(log2(h->n))+1;
}



/*This function is used to combine the root list nodes which have same degree */

void CONSOLIDATE(heap* H){
    int k=degree(H);
    node** x;
    x=(node**)malloc((k+1)*sizeof(node*));
    for(int i=0;i<=k;i++){
        x[i]=NULL;
    }
    node*temp2=H->min;
    node* temp1=temp2;
    do{
        int d=temp2->degree;
        while(x[d]!=NULL){
            node*temp3=x[d];
            if(temp2->key>temp3->key){
                node* temp4=temp3;
                temp3=temp2;
                temp2=temp4;
            }
            FIB_LINK(H,temp3,temp2);
            if(temp1==temp3)
                temp1=temp2;
            x[d]=NULL;
            d++;
        }
        x[d]=temp2;
        temp2=temp2->right;
    }while(temp2!=temp1);
    H->min=NULL;
    for(int i=0;i<=k;i++){
        if(x[i]!=NULL){
            if(H->min==NULL){
                H->min=x[i];
                x[i]->left=x[i];
                x[i]->right=x[i];
            }
            else{
                insert(H,x[i]);
                if(x[i]->key<H->min->key)
                    H->min=x[i];
            }
        }
    }
}



/*This function is used to extract and return the node having minimum key value*/

int FIB_HEAP_EXTRACT_MIN(heap* fibHeap)
{
    if(fibHeap->min!=NULL)
    {
        node *temp=fibHeap->min;
        node *ptr=temp;
        node *x=NULL;
        if(temp->child!=NULL) 
        {
            x=temp->child;
            do
            {
                ptr=x->right;
                (fibHeap->min->left)->right=x;
                x->right=fibHeap->min;
                x->left=fibHeap->min->left;
                fibHeap->min->left=x;
                if( (x->key) < (fibHeap->min->key) )
                {
                    fibHeap->min=x;
                }
                x->parent=NULL;
                x=ptr;
            }while(ptr!=temp->child);
        }
        temp->left->right=temp->right;
        temp->right->left=temp->left;
        fibHeap->min=temp->right;
        if(temp==temp->right && temp->child==NULL)
            fibHeap->min=NULL;
        else 
        {
            fibHeap->min=temp->right;
            CONSOLIDATE(fibHeap);
        }
        fibHeap->n--;
        return temp->key;
    }
}



/*This function search for a node having the given key value in the fibonacci heap*/

void search(heap* fibHeap,node* fibHeapNode,int value,int newKey,int *done)
{
    node *temp=fibHeapNode;
    temp->found=true;
    if(temp->key==value)
    {
        *done=1;
        temp->found=false;
        FIB_HEAP_DECREASE_KEY(fibHeap,temp,newKey);
        return;
    }
    else
    {
        if(temp->child!=NULL)
            search(fibHeap,temp->child,value,newKey,done);
        if(!temp->right->found)
            search(fibHeap,temp->right,value,newKey,done);
    }
    temp->found=false;
    
}



/* This function is used to delete a node from the fibonacci heap */

void FIB_HEAP_DELETE(heap* h,int key,int *done){
    search(h,h->min,key,INT_MIN,done);
    int temp=FIB_HEAP_EXTRACT_MIN(h);
}



/* This function print the fibonacci heap with its nodes and their depth */

void PRINT_HEAP(node* n,char* s,int level){
    node*temp=n;
    s[level]='\\';
    s[++level]='\0';
    do{
        printf("%s\t\t%d\n",s,temp->key);
        if(temp->child!=NULL)
            PRINT_HEAP(temp->child,s,level);
        temp=temp->right;
    }while(temp!=n);
}



int main(){
    heap** fibheap;
    int n;
    do{
        printf("Enter the number of Fibonacci heaps to create:\n==> ");
        scanf("%d",&n);
    }
    while(n<1);
    fibheap=(heap**)malloc((n+1)*sizeof(heap*));
    for(int i=1;i<=n;i++)
        fibheap[i]=MAKE_HEAP();
    if(n==1) printf("Heap is made and referenced to 1\n");
    else printf("Heaps are made and referenced from 1 to %d \n",n);
    int ref=-1;
    do{
        printf("\nPlease enter heap reference number to continue operations\nEnter -1 to exit\n");
        printf("Enter heap reference number:\n==> ");
        scanf("%d",&ref);
        int choice = 8;
        if(ref>0 && ref<=n){
            do{
                printf("\nChoose from the following operations:\n");
                printf("\n1: Insert\n2: Union\n3: Extract Minimum\n4: Decrease Key\n5: Delete Node\n6: Print Heap\n7: Minimum\n8: Exit\n\nEnter your choice:\n==> ");
                scanf("%d",&choice);
                if(choice==1){
                    int value;
                    printf("Enter number to insert: \n==> ");
                    scanf("%d",&value);
                    FIB_HEAP_INSERT(fibheap[ref],value);
                }
                else if(choice==2){
                    int value;
                    printf("Enter second reference value to merge with current heap having reference number %d:\n==> ",ref);
                    scanf("%d",&value);
                    fibheap[ref]=FIB_HEAP_UNION(fibheap[ref],fibheap[value]);
                    fibheap[value]->min=NULL;
                    fibheap[value]->n=0;
                    printf("Merged %d and %d into %d and destroyed %d\n",ref,value,ref,value);
                }
                else if(choice==3 && fibheap[ref]->min!=NULL){
                    int value=FIB_HEAP_EXTRACT_MIN(fibheap[ref]);
                    printf("Minimum value extracted is: %d \n",value);
                }
                else if(choice==3 && fibheap[ref]->min==NULL){
                    printf("Heap is empty!\n");
                }
                else if(choice==4){
                    int done=0;
                    int key,newKey;
                    printf("Enter current value of key:\n==> ");
                    scanf("%d",&key);
                    printf("Enter new value of key:\n==> ");
                    scanf("%d",&newKey);

                    if(fibheap[ref]->min!=NULL)
                        search(fibheap[ref],fibheap[ref]->min,key,newKey,&done);
                    if(fibheap[ref]->min!=NULL && done==1 && newKey<=key)
                        printf("%d was decreased to %d\n",key,newKey);
                    else if(newKey<=key)
                        printf("Node with given key value is not present!\n");
                }
                else if(choice==5){
                    int done=0;
                    printf("Enter key value to delete node:\n==> ");
                    int key;
                    scanf("%d",&key);

                    if(fibheap[ref]->min!=NULL)
                        FIB_HEAP_DELETE(fibheap[ref],key,&done);
                    if(fibheap[ref]->min!=NULL && done==1)
                        printf("%d has been deleted\n",key);
                    else
                        printf("Node with given key value is not present!\n");
                }
                else if(choice==6){
                    if(fibheap[ref]->min!=NULL){
                        char s[100]="";
                        PRINT_HEAP(fibheap[ref]->min,s,0);
                    }
                    else{
                        printf("Heap is empty!\n");
                    }
                }
                else if(choice==7){
                    if(fibheap[ref]->n!=0)
                        printf("Minimum element is %d\n",fibheap[ref]->min->key);
                    else
                        printf("Heap is empty!");
                }
                else if(choice==8){
                    printf("Exiting...\n");
                }
                else{
                    printf("Please Enter a valid choice\n");
                }
            }while(choice!=8);
        }
    }while(ref!=-1);
    printf("\n---Program completed successfully---\n\n");
    return 0;
}