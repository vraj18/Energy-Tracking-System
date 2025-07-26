#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "transactions.h"

typedef struct B_tree_node Btree_node;


typedef struct Buyer_subtree_tag{
    int numkeys;
    Transactions keys[2];
    struct Buyer_subtree_tag* parent;
    struct Buyer_subtree_tag* child[3];   
}Buyer_subtree;

typedef struct Buyers_tag{
    int bid;
    int total_energy;
    Buyer_subtree *transaction_subtree;
}Buyers;

typedef struct B_Data_node_tag{
    Buyers val[3];
    int num_data;
    struct B_Data_node_tag *next;
    struct B_Data_node_tag *prev;
    Btree_node *parent;
}Buyers_data_node;

typedef struct B_tree_node{
    int numkeys;
    int keys[2];
    int flag;
    struct B_tree_node* parent;
    union{
        struct B_tree_node* child[3];   
        Buyers_data_node* data[3];
    };
}Btree_node;





int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int median(Buyers *ptr){

    qsort(ptr, 4, sizeof(Buyers), compare);
    return ptr[2].bid;
}

Btree_node* Buyers_base_case_tree(Buyers arr[]){
    //making Btree node
    Btree_node *root = (Btree_node *)malloc(sizeof(Btree_node));
    root->flag = 1;
    root->keys[0] = median(arr);
    root->parent = NULL;
    root->numkeys = 1;
    //making data nodes
    root->data[0] = (Buyers_data_node*)malloc(sizeof(Buyers_data_node));
    root->data[0]->val[0] = arr[0];
    root->data[0]->num_data = 1;
    root->data[0]->val[1] = arr[1];
    root->data[0]->num_data++;
    root->data[0]->parent = root;

    root->data[1] = (Buyers_data_node*)malloc(sizeof(Buyers_data_node));
    root->data[1]->val[0] = arr[2];
    root->data[1]->num_data = 1;
    root->data[1]->val[1] =  arr[3];
    root->data[1]->num_data++;
    root->data[1]->parent = root;
    //maintaining doubly linked list
    root->data[0]->next = root->data[1];
    root->data[0]->prev = NULL;
    root->data[1]->prev = root->data[0];
    root->data[1]->next = NULL;
    return root;
}

void display_subtree(Buyer_subtree* root){
    if(root){
        for(int i = 0 ; i<root->numkeys ; i++){
            printf("Transaction_id :%d  , Buyer_id: %d , Seller_id: %d, Energy: %d, price: %d , total_price: %d  , time_stamp: %s\n" , root->keys[i].tid , root->keys[i].bid , root->keys[i].sid , root->keys[i].energy , root->keys[i].price , root->keys[i].total_price , root->keys[i].time_stamp );
        }
        if(root->child[0]){
            for(int i = 0 ; i<=root->numkeys ; i++){
                display_subtree(root->child[i]);
            }
        }
    }

}

// Quick sort partition function
int partition(Buyers arr[], int low, int high) {
    int pivot = arr[high].total_energy; // Pivot is the total_energy of the last element
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].total_energy <= pivot) {
            i++;
            // Swap arr[i] and arr[j]
            Buyers temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // Swap arr[i + 1] and arr[high] (pivot)
    Buyers temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// Quick sort function
void quick_sort(Buyers arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); // Partition index

        // Recursively sort elements before and after partition
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}
void sort_buyers_list(Btree_node* root){
    while(root->flag==0){
        root = root->child[0];
    }
    Buyers_data_node *dptr = root->data[0];
    Buyers_data_node *temp = dptr;
    int length = 0;
    while(dptr){
        length+= dptr->num_data;
        dptr = dptr->next;
    }
    Buyers *arr = (Buyers*)malloc(length*sizeof(Buyers));
    int i = 0;  
    while(temp){
        for(int j = 0 ; j<temp->num_data ; j++){
            arr[i] = temp->val[j];
            i++;
        }
        temp = temp->next;
    }
    quick_sort(arr, 0 , length-1);
    for(int i = 0 ; i<length ; i++){
        printf("BuyerId : %d, Energy_bought: %d \n", arr[i].bid, arr[i].total_energy);
        display_subtree(arr[i].transaction_subtree);
    }

}


// Function to display the B+ tree in a tabular format
void Buyers_display_tree_tabular(Btree_node *root) {
    if (!root) {
        printf("The tree is empty.\n");
        return;
    }

    int level = 0;
    Btree_node *current = root;
    while (current->flag == 0) {
        current = current->child[0];
        level++;
    }

    // Display data nodes in tabular format
    Buyers_data_node *data_node = current->data[0];
    while (data_node) {
        for (int i = 0; i < data_node->num_data; i++) {
            printf("BuyerId : %d, Energy_bought: %d) \n", data_node->val[i].bid, data_node->val[i].total_energy);
            display_subtree(data_node->val[i].transaction_subtree);
        }
        printf("\n");
        data_node = data_node->next;
    }

}

//insertion to buyers subtree
Buyer_subtree* buyers_subtree_insert(Buyer_subtree** head, Buyer_subtree* node , Transactions value){
    // if(value==12)printf("%d" , node->numkeys);
    if(node->numkeys <2){
        // printf("here3 ");
        if(value.tid >node->keys[0].tid ){
            node->keys[1] = value;
        }else{
            node->keys[1] = node->keys[0];
            node->keys[0] = value;
            node->child[2] = node->child[1];
        }
        node->numkeys = 2;
        return NULL;
    }else{
        // printf("here3 ");
        Buyer_subtree *new = (Buyer_subtree*)malloc(sizeof(Buyer_subtree));
        Transactions prr[3];
        int k = 0;
        int flag = 1;
        for(int i = 0 ; i<3 ; i++){
            if( (k>1 || value.tid < node->keys[k].tid ) && flag ){
                prr[i] = value;
                flag = 0;
            }else{
                prr[i] = node->keys[k];
                k++;
            } 
        }
        // if(value==10)
        //     printf("%d %d %d " , prr[0] , prr[1] , prr[2]);
        node->numkeys = 1;
        node->keys[0] = prr[0];
        new->keys[0] = prr[2];
        new->numkeys = 1;
        new->child[0] = NULL;
        new->child[1] = NULL;           
        new->child[2] = NULL;
        if(node->parent==NULL){
            Buyer_subtree *root = (Buyer_subtree*)malloc(sizeof(Buyer_subtree));
            root->keys[0] = prr[1];
            root->numkeys = 1;
            root->child[0] = node;
            root->child[1] = new;
            root->parent = NULL;
            node->parent = new->parent = root;
            *head = root;
            // if(value==10)printf("%d" , new->keys[0]);
        }else{
            Buyer_subtree* parent = node->parent;
            // if(value==12)printf("%d %d" , node->parent->keys[0] , node->parent->numkeys);
            Buyer_subtree *t_n = buyers_subtree_insert(head , node->parent , prr[1]);

            if(t_n == NULL){
                if(new->keys[0].tid>parent->keys[1].tid){
                    parent->child[2] = new;
                    new->parent = parent;
                    
                }else{
                    parent->child[2] = parent->child[1];
                    parent->child[1] = new;
                    new->parent = parent;
                }
                parent->numkeys = 2;
            }else{
                Buyer_subtree* arr[4];
                int k = 0;
                int i = 0;
                int flag = 1;
                while(i<3 && k<4){
                    if((parent->child[i]->keys[0].tid>new->keys[0].tid) && flag){
                        arr[k] = new;
                        // if(value==12)printf("%d " , arr[k]->keys[0]);
                        k++;
                        flag = 0;
                    }else{
                        arr[k] = parent->child[i];
                        // if(value==12)printf("%d " , arr[k]->keys[0]);
                        k++;
                        i++;
                    }
                }
                if(flag){
                    arr[3] = new;
                }
                parent->child[0] = arr[0];
                arr[0]->parent = parent;
                parent->child[1] = arr[1];
                arr[1]->parent = parent;
                parent->numkeys = 1;
                t_n->child[0] = arr[2];
                arr[2]->parent = t_n;
                t_n->child[1] = arr[3];
                arr[3]->parent = t_n;
                t_n->numkeys = 1;
            }
        }
        return new;
    }
    return NULL;
}

Buyer_subtree* buyers_subtree_search(Buyer_subtree** head , Transactions value){
    Buyer_subtree *prev=NULL;
    Buyer_subtree* nptr = *head;
    // printf("here3");

    while(nptr){
        // printf("here3");
        int k = 0;
        int flag = 0;
        for(int i = 0 ; i<nptr->numkeys ; i++){
            // if(value.tid ==18 )printf("here ");
            if(value.tid>=nptr->keys[i].tid){
                k = i;
                flag = 1;
            }
        }
        if(flag){
            prev = nptr;
            nptr = nptr->child[k+1];
        }else{
            prev = nptr;
            nptr = nptr->child[0];
        }
    }
    // printf("here3");

    if(prev==NULL){
        
        if(*head == NULL){
            
            Buyer_subtree *root = (Buyer_subtree*)malloc(sizeof(Buyer_subtree));
            root->numkeys = 1;
            root->parent = NULL;
            root->keys[0] = value;
            root->child[0] = NULL;
            root->child[1] = NULL; 
            root->child[2] = NULL; 
            *head = root; 
        }else{
            // printf("here4");

            buyers_subtree_insert(head , prev , value);
        }   
        
    }else{
        // printf("here5");

        // printf("here3");
        buyers_subtree_insert(head , prev , value);

    }
    // if(value.tid==7)printf("%d " , (*head)->keys[0].tid);
    return *head;
}


//searching of transaction subtree
Buyer_subtree** Buyers_search2(Btree_node *root , Transactions t){
    Btree_node *nptr = root;
    while(nptr->flag == 0){
        int k = 0;
        int flag = 0;
        for(int i = 0 ; i<nptr->numkeys ; i++){
            // if(e==33){
            //     printf("%d " , nptr->keys[i]);
            // }
            if(t.bid>=nptr->keys[i]){
                k = i;
                flag = 1;
            }
        }
        if(flag){
            nptr = nptr->child[k+1];
        }else{
            nptr = nptr->child[0];
        }
    }
    Buyers_data_node* dptr;
    int k = 0;
    int flag = 0;   
    for(int i = 0 ; i<nptr->numkeys ; i++){
        
        if(t.bid>=nptr->keys[i]){
            k = i;
            flag = 1;

        }
    }
    if(flag){
        dptr = nptr->data[k+1];
    }else{
        dptr = nptr->data[0];
    }
    for(int i = 0 ; i<dptr->num_data ; i++){
        if(dptr->val[i].bid==t.bid){
            dptr->val[i].total_energy+= t.energy;
            return &dptr->val[i].transaction_subtree;
        }
    }
    return NULL;
}


//insertion in the B+ tree
Btree_node* Buyers_insert_btree(Btree_node** head, Btree_node * node , int value){
    // if(value==12)printf("%d" , node->numkeys);
    if(node->numkeys <2){
        if(value >node->keys[0] ){
            node->keys[1] = value;
            node->flag = 0;
        }else{
            node->keys[1] = node->keys[0];
            node->keys[0] = value;
            node->child[2] = node->child[1];
            node->flag;
        }
        node->numkeys = 2;
        return NULL;
    }else{
        Btree_node *new = (Btree_node*)malloc(sizeof(Btree_node));
        int prr[3];
        int k = 0;
        int flag = 1;
        for(int i = 0 ; i<3 ; i++){
            if( (k>1 || value < node->keys[k] ) && flag ){
                prr[i] = value;
                flag = 0;
            }else{
                prr[i] = node->keys[k];
                k++;
            } 
        }
        // if(value==10)
        //     printf("%d %d %d " , prr[0] , prr[1] , prr[2]);
        node->numkeys = 1;
        node->keys[0] = prr[0];
        new->keys[0] = prr[2];
        new->numkeys = 1;
        if(node->parent==NULL){
            Btree_node *root = (Btree_node*)malloc(sizeof(Btree_node));
            root->keys[0] = prr[1];
            root->numkeys = 1;
            root->child[0] = node;
            root->child[1] = new;
            root->flag = 0;
            root->parent = NULL;
            node->parent = new->parent = root;
            *head = root;
            // if(value==10)printf("%d" , new->keys[0]);
        }else{
            Btree_node* parent = node->parent;
            // if(value==12)printf("%d %d" , node->parent->keys[0] , node->parent->numkeys);
            Btree_node *t_n = Buyers_insert_btree(head , node->parent , prr[1]);

            if(t_n == NULL){
                if(new->keys[0]>parent->keys[1]){
                    parent->child[2] = new;
                    new->parent = parent;
                    
                }else{
                    parent->child[2] = parent->child[1];
                    parent->child[1] = new;
                    new->parent = parent;
                }
                parent->numkeys = 2;
            }else{
                Btree_node* arr[4];
                int k = 0;
                int i = 0;
                int flag = 1;
                while(i<3 && k<4){
                    if((parent->child[i]->keys[0]>new->keys[0]) && flag){
                        arr[k] = new;
                        // if(value==12)printf("%d " , arr[k]->keys[0]);
                        k++;
                        flag = 0;
                    }else{
                        arr[k] = parent->child[i];
                        // if(value==12)printf("%d " , arr[k]->keys[0]);
                        k++;
                        i++;
                    }
                }
                if(flag){
                    arr[3] = new;
                }
                parent->child[0] = arr[0];
                arr[0]->parent = parent;
                parent->child[1] = arr[1];
                arr[1]->parent = parent;
                parent->numkeys = 1;
                parent->flag = 0;
                t_n->child[0] = arr[2];
                arr[2]->parent = t_n;
                t_n->child[1] = arr[3];
                arr[3]->parent = t_n;
                t_n->numkeys = 1;
                t_n->flag = 0;
            }
        }
        return new;
    }
    return NULL;
}

Btree_node* Buyers_insert_node(Btree_node *root , Buyers_data_node *dptr , Buyers trans){
    if(dptr->num_data<3){
        int t;
        for(int i = 0 ; i<dptr->num_data ; i++){
            if(dptr->val[i].bid > trans.bid){
                t = dptr->val[i].bid;
                dptr->val[i] = trans;
                trans.bid = t;
            }
        }
        dptr->val[dptr->num_data] = trans;
        dptr->num_data++;
    }else{
        Buyers arr[4];
        for(int i = 0 ; i<3 ; i++){
            arr[i] = dptr->val[i];
        }
        arr[3] = trans;
        int parent_value = median(arr );
        Btree_node* parent = dptr->parent;
        Buyers_data_node* t = (Buyers_data_node*)malloc(sizeof(Buyers_data_node));
        for(int i = 0 ; i<2 ; i++){
            dptr->val[i] = arr[i];
        }
        dptr->val[2].bid = 0;
        dptr->num_data = 2;
        for(int i = 0 ; i<2 ; i++){
            t->val[i] = arr[i+2];
        }
        t->val[2].bid = 0;
        t->num_data = 2;
        t->parent = parent;
        t->next = dptr->next;
        t->prev = dptr;
        if(dptr->next)dptr->next->prev = t;
        dptr->next = t;
        if(dptr->parent->numkeys<2){
            if(parent->keys[0]<arr[2].bid){
                parent->keys[1] = arr[2].bid;
                parent->data[2] = t;
                parent->numkeys = 2;
            }else{
                parent->keys[1] = parent->keys[0];
                parent->keys[0] = arr[2].bid;
                parent->data[2] = parent->data[1];
                parent->data[1] = t;
                parent->numkeys = 2;
            }
        }else{
            
            Btree_node* new = Buyers_insert_btree(&root , parent , arr[2].bid);
            while(t->next && t->next->parent == parent){
                t = t->next;
            }
            for(int i = 1 ; i>=0 ; i--){
                new->data[i] = t;
                t->parent = new;
                t = t->prev;
            }
            new->flag = 1;
            for(int i = 1 ; i>=0 ; i--){
                parent->data[i] = t;
                t->parent = parent;
                t = t->prev;
            }
        }
        
    } 
    return root;
}


Buyers_data_node* Buyers_search(Btree_node *root , Buyers trans){
    Btree_node *nptr = root;
    while(nptr->flag == 0){
        int k = 0;
        int flag = 0;
        for(int i = 0 ; i<nptr->numkeys ; i++){
            // if(e==33){
            //     printf("%d " , nptr->keys[i]);
            // }
            if(trans.bid>=nptr->keys[i]){
                k = i;
                flag = 1;
            }
        }
        if(flag){
            nptr = nptr->child[k+1];
        }else{
            nptr = nptr->child[0];
        }
    }
    Buyers_data_node* dptr;
    int k = 0;
    int flag = 0;   
    for(int i = 0 ; i<nptr->numkeys ; i++){
        
        if(trans.bid>=nptr->keys[i]){
            k = i;
            flag = 1;

        }
    }
    if(flag){
        dptr = nptr->data[k+1];
    }else{
        dptr = nptr->data[0];
    }
    return dptr;
}

Btree_node* Buyers_insert(Btree_node *root , Buyers buy){
    if(root==NULL){
        static Buyers arr[4];
        static int i = 0;
        arr[i] = buy;
        i++;
        if(i==4){
            root = Buyers_base_case_tree(arr); 
        }
    }else{
        Buyers_data_node* dptr = Buyers_search(root , buy);
        root = Buyers_insert_node(root , dptr , buy);
    }    
    
    return root;
}

