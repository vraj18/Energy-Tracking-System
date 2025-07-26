#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "transactions.h"
typedef struct S_B_tree_node S_Btree_node;

typedef struct Seller_subtree_tag
{
    int numkeys;
    Transactions keys[2];
    struct Seller_subtree_tag *parent;
    struct Seller_subtree_tag *child[3];
} Seller_subtree;

typedef struct b_node
{
    int buyerID;
    struct b_node *next;
} B;

typedef struct Sellers_tag
{
    int sid;
    int energy_price_below_300;
    int energy_price_above_300;
    int total_revenue;
    B *frequent_buyers;
    Seller_subtree *transaction_subtree;
} Sellers;

typedef struct S_Data_node_tag
{
    Sellers val[3];
    int num_data;
    struct S_Data_node_tag *next;
    struct S_Data_node_tag *prev;
    S_Btree_node *parent;
} Sellers_data_node;

typedef struct S_B_tree_node
{
    int numkeys;
    int keys[2];
    int flag;
    struct S_B_tree_node *parent;
    union
    {
        struct S_B_tree_node *child[3];
        Sellers_data_node *data[3];
    };
} S_Btree_node;



int Seller_compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int Seller_median(Sellers *ptr)
{

    qsort(ptr, 4, sizeof(Sellers), Seller_compare);
    return ptr[2].sid;
}

S_Btree_node *sellers_base_case_tree(Sellers arr[])
{
    // making Btree node
    S_Btree_node *root = (S_Btree_node *)malloc(sizeof(S_Btree_node));
    root->flag = 1;
    root->keys[0] = Seller_median(arr);
    root->parent = NULL;
    root->numkeys = 1;
    // making data nodes
    root->data[0] = (Sellers_data_node *)malloc(sizeof(Sellers_data_node));
    root->data[0]->val[0] = arr[0];
    root->data[0]->num_data = 1;
    root->data[0]->val[1] = arr[1];
    root->data[0]->num_data++;
    root->data[0]->parent = root;

    root->data[1] = (Sellers_data_node *)malloc(sizeof(Sellers_data_node));
    root->data[1]->val[0] = arr[2];
    root->data[1]->num_data = 1;
    root->data[1]->val[1] = arr[3];
    root->data[1]->num_data++;
    root->data[1]->parent = root;
    // maintaining doubly linked list
    root->data[0]->next = root->data[1];
    root->data[0]->prev = NULL;
    root->data[1]->prev = root->data[0];
    root->data[1]->next = NULL;
    return root;
}

void sellers_display_tree(S_Btree_node *root)
{
    if (root)
    {
        for (int i = 0; i < root->numkeys; i++)
        {
            printf("%d ", root->keys[i]);
        }
        if (root->flag == 0)
        {
            for (int i = 0; i <= root->numkeys; i++)
            {
                sellers_display_tree(root->child[i]);
            }
        }
    }
}

void sellers_display_tree_nodes(S_Btree_node *root)
{
    while (root->flag == 0)
    {
        root = root->child[0];
    }
    Sellers_data_node *dptr = root->data[0];

    while (dptr)
    {
        for (int i = 0; i < dptr->num_data; i++)
        {
            printf("Seller_id: %d, price_below_300: %d ,price_above_300: %d ", dptr->val[i].sid, dptr->val[i].energy_price_below_300, dptr->val[i].energy_price_above_300);
        }
        printf("\n");
        dptr = dptr->next;
    }
}

void display_subtree_seller(Seller_subtree *root)
{
    if (root)
    {
        for (int i = 0; i < root->numkeys; i++)
        {
            printf("Transaction_id :%d  , Buyer_id: %d , Seller_id: %d, Energy: %d, price: %d , total_price: %d  , time_stamp: %s\n", root->keys[i].tid, root->keys[i].bid, root->keys[i].sid, root->keys[i].energy, root->keys[i].price, root->keys[i].total_price, root->keys[i].time_stamp);
        }
        if (root->child[0])
        {
            for (int i = 0; i <= root->numkeys; i++)
            {
                display_subtree_seller(root->child[i]);
            }
        }
    }
}

void Sellers_display_tree_tabular(S_Btree_node *root)
{
    if (!root)
    {
        printf("The tree is empty.\n");
        return;
    }
    int level = 0;
    S_Btree_node *current = root;

    // Traverse to the leftmost leaf node to start with data nodes
    while (current->flag == 0)
    {
        current = current->child[0];
        level++;
    }

    // Display data nodes in tabular format
    Sellers_data_node *data_node = current->data[0];
    while (data_node)
    {
        for (int i = 0; i < data_node->num_data; i++)
        {
            printf("Seller_id : %d, total_revenue : %d Regular_buyers: ", data_node->val[i].sid, data_node->val[i].total_revenue);
            while (data_node->val[i].frequent_buyers)
            {
                printf("%d ", data_node->val[i].frequent_buyers->buyerID);
                data_node->val[i].frequent_buyers = data_node->val[i].frequent_buyers->next;
            }
            printf("\n");
            display_subtree_seller(data_node->val[i].transaction_subtree);
        }
        printf("\n");
        data_node = data_node->next;
    }
}

Seller_subtree *Sellers_subtree_insert(Seller_subtree **head, Seller_subtree *node, Transactions value)
{
    // if(value==12)printf("%d" , node->numkeys);
    if (node->numkeys < 2)
    {
        printf("here3 ");
        if (value.tid > node->keys[0].tid)
        {
            node->keys[1] = value;
        }
        else
        {
            node->keys[1] = node->keys[0];
            node->keys[0] = value;
            node->child[2] = node->child[1];
        }
        node->numkeys = 2;
        return NULL;
    }
    else
    {
        // printf("here3 ");
        Seller_subtree *new = (Seller_subtree *)malloc(sizeof(Seller_subtree));
        Transactions prr[3];
        int k = 0;
        int flag = 1;
        for (int i = 0; i < 3; i++)
        {
            if ((k > 1 || value.tid < node->keys[k].tid) && flag)
            {
                prr[i] = value;
                flag = 0;
            }
            else
            {
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

        if (node->parent == NULL)
        {
            Seller_subtree *root = (Seller_subtree *)malloc(sizeof(Seller_subtree));
            root->keys[0] = prr[1];
            root->numkeys = 1;
            root->child[0] = node;
            root->child[1] = new;
            root->parent = NULL;
            node->parent = new->parent = root;
            *head = root;
            // if(value==10)printf("%d" , new->keys[0]);
        }
        else
        {
            Seller_subtree *parent = node->parent;
            // if(value==12)printf("%d %d" , node->parent->keys[0] , node->parent->numkeys);
            Seller_subtree *t_n = Sellers_subtree_insert(head, node->parent, prr[1]);

            if (t_n == NULL)
            {
                if (new->keys[0].tid > parent->keys[1].tid)
                {
                    parent->child[2] = new;
                    
                }
                else
                {
                    parent->child[2] = parent->child[1];
                    parent->child[1] = new;
                }
                new->parent = parent;
                parent->numkeys = 2;
            }
            else
            {
                Seller_subtree *arr[4];
                int k = 0;
                int i = 0;
                int flag = 1;
                while (i < 3 && k < 4)
                {
                    if ((parent->child[i]->keys[0].tid > new->keys[0].tid) && flag)
                    {
                        arr[k] = new;
                        // if(value==12)printf("%d " , arr[k]->keys[0]);
                        k++;
                        flag = 0;
                    }
                    else
                    {
                        arr[k] = parent->child[i];
                        // if(value==12)printf("%d " , arr[k]->keys[0]);
                        k++;
                        i++;
                    }
                }
                if (flag)
                {
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

Seller_subtree *sellers_subtree_search(Seller_subtree **head, Transactions value)
{
    Seller_subtree *prev = NULL;
    Seller_subtree *nptr = *head;

    while (nptr)
    {
        // printf("here3");
        int k = 0;
        int flag = 0;
        for (int i = 0; i < nptr->numkeys; i++)
        {
            if (value.tid >= nptr->keys[i].tid)
            {
                k = i;
                flag = 1;
            }
        }
        if (flag)
        {
            prev = nptr;
            nptr = nptr->child[k + 1];
        }
        else
        {
            prev = nptr;
            nptr = nptr->child[0];
        }
    }
    // printf("here3");

    if (prev == NULL)
    {

        if (*head == NULL)
        {
            // printf("here2 ");
            Seller_subtree *root = (Seller_subtree *)malloc(sizeof(Seller_subtree));
            root->numkeys = 1;
            root->parent = NULL;
            root->keys[0] = value;
            root->child[0] = NULL;
            root->child[1] = NULL;
            root->child[2] = NULL;
            *head = root;
        }
        else
        {

            Sellers_subtree_insert(head, prev, value);
        }
    }
    else
    {
        // printf("here3");
        Sellers_subtree_insert(head, prev, value);
    }
    // if(value.tid==7)printf("%d " , (*head)->keys[0].tid);
    return *head;
}

S_Btree_node *sellers_insert_btree(S_Btree_node **head, S_Btree_node *node, int value)
{
    // if(value==12)printf("%d" , node->numkeys);
    if (node->numkeys < 2)
    {
        if (value > node->keys[0])
        {
            node->keys[1] = value;
            node->flag = 0;
        }
        else
        {
            node->keys[1] = node->keys[0];
            node->keys[0] = value;
            node->child[2] = node->child[1];
            node->flag;
        }
        node->numkeys = 2;
        return NULL;
    }
    else
    {
        S_Btree_node *new = (S_Btree_node *)malloc(sizeof(S_Btree_node));
        int prr[3];
        int k = 0;
        int flag = 1;
        for (int i = 0; i < 3; i++)
        {
            if ((k > 1 || value < node->keys[k]) && flag)
            {
                prr[i] = value;
                flag = 0;
            }
            else
            {
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
        if (node->parent == NULL)
        {
            S_Btree_node *root = (S_Btree_node *)malloc(sizeof(S_Btree_node));
            root->keys[0] = prr[1];
            root->numkeys = 1;
            root->child[0] = node;
            root->child[1] = new;
            root->flag = 0;
            root->parent = NULL;
            node->parent = new->parent = root;
            *head = root;
            // if(value==10)printf("%d" , new->keys[0]);
        }
        else
        {
            S_Btree_node *parent = node->parent;
            // if(value==12)printf("%d %d" , node->parent->keys[0] , node->parent->numkeys);
            S_Btree_node *t_n = sellers_insert_btree(head, node->parent, prr[1]);

            if (t_n == NULL)
            {
                if (new->keys[0] > parent->keys[1])
                {
                    parent->child[2] = new;
                    new->parent = parent;
                }
                else
                {
                    parent->child[2] = parent->child[1];
                    parent->child[1] = new;
                    new->parent = parent;
                }
                parent->numkeys = 2;
            }
            else
            {
                S_Btree_node *arr[4];
                int k = 0;
                int i = 0;
                int flag = 1;
                while (i < 3 && k < 4)
                {
                    if ((parent->child[i]->keys[0] > new->keys[0]) && flag)
                    {
                        arr[k] = new;
                        // if(value==12)printf("%d " , arr[k]->keys[0]);
                        k++;
                        flag = 0;
                    }
                    else
                    {
                        arr[k] = parent->child[i];
                        // if(value==12)printf("%d " , arr[k]->keys[0]);
                        k++;
                        i++;
                    }
                }
                if (flag)
                {
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

S_Btree_node *sellers_insert_node(S_Btree_node *root, Sellers_data_node *dptr, Sellers S)
{
    if (dptr->num_data < 3)
    {
        int t;
        for (int i = 0; i < dptr->num_data; i++)
        {
            if (dptr->val[i].sid > S.sid)
            {
                t = dptr->val[i].sid;
                dptr->val[i] = S;
                S.sid = t;
            }
        }
        dptr->val[dptr->num_data] = S;
        dptr->num_data++;
    }
    else
    {
        Sellers arr[4];
        for (int i = 0; i < 3; i++)
        {
            arr[i] = dptr->val[i];
        }
        arr[3] = S;
        int parent_value = Seller_median(arr);
        S_Btree_node *parent = dptr->parent;
        Sellers_data_node *t = (Sellers_data_node *)malloc(sizeof(Sellers_data_node));
        for (int i = 0; i < 2; i++)
        {
            dptr->val[i] = arr[i];
        }
        dptr->val[2].sid = 0;
        dptr->num_data = 2;
        for (int i = 0; i < 2; i++)
        {
            t->val[i] = arr[i + 2];
        }
        t->val[2].sid = 0;
        t->num_data = 2;
        t->parent = parent;
        t->next = dptr->next;
        t->prev = dptr;
        if (dptr->next)
            dptr->next->prev = t;
        dptr->next = t;
        if (dptr->parent->numkeys < 2)
        {
            if (parent->keys[0] < arr[2].sid)
            {
                parent->keys[1] = arr[2].sid;
                parent->data[2] = t;
                parent->numkeys = 2;
            }
            else
            {
                parent->keys[1] = parent->keys[0];
                parent->keys[0] = arr[2].sid;
                parent->data[2] = parent->data[1];
                parent->data[1] = t;
                parent->numkeys = 2;
            }
        }
        else
        {

            S_Btree_node *new = sellers_insert_btree(&root, parent, arr[2].sid);
            while (t->next && t->next->parent == parent)
            {
                t = t->next;
            }
            for (int i = 1; i >= 0; i--)
            {
                new->data[i] = t;
                t->parent = new;
                t = t->prev;
            }
            new->flag = 1;
            for (int i = 1; i >= 0; i--)
            {
                parent->data[i] = t;
                t->parent = parent;
                t = t->prev;
            }
        }
    }
    return root;
}

// int find_key_index(int *keys, int numkeys, int sid)
// {
//     int low = 0, high = numkeys - 1;
//     int result = -1; // Will point to first key <= sid

//     while (low <= high)
//     {
//         int mid = low + (high - low) / 2;
//         if (keys[mid] <= sid)
//         {
//             result = mid;
//             low = mid + 1;
//         }
//         else
//         {
//             high = mid - 1;
//         }
//     }

//     return result + 1; // +1 because child/data pointers are one more than keys
// }

// Sellers_data_node *sellers_search(S_Btree_node *root, int sid)
// {
//     if (root == NULL)
//         return NULL;

//     S_Btree_node *current = root;

//     while (current->flag == 0)
//     {
//         int child_index = find_key_index(current->keys, current->numkeys, sid);
//         current = current->child[child_index];
//         if (current == NULL)
//             return NULL;
//     }

//     int data_index = find_key_index(current->keys, current->numkeys, sid);
//     return current->data[data_index];
// }

Sellers_data_node* sellers_search(S_Btree_node *root , int sid){
    S_Btree_node *nptr = root;
    while(nptr->flag == 0){
        int k = 0;
        int flag = 0;
        for(int i = 0 ; i<nptr->numkeys ; i++){
            if(sid>=nptr->keys[i]){
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
    Sellers_data_node* dptr;
    int k = 0;
    int flag = 0;
    for(int i = 0 ; i<nptr->numkeys ; i++){

        if(sid>=nptr->keys[i]){
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

Seller_subtree **Sellers_search2(S_Btree_node *root, Transactions t)
{
    S_Btree_node *nptr = root;
    while (nptr->flag == 0)
    {
        int k = 0;
        int flag = 0;
        for (int i = 0; i < nptr->numkeys; i++)
        {
            // if(e==33){
            //     printf("%d " , nptr->keys[i]);
            // }
            if (t.sid >= nptr->keys[i])
            {
                k = i;
                flag = 1;
            }
        }
        if (flag)
        {
            nptr = nptr->child[k + 1];
        }
        else
        {
            nptr = nptr->child[0];
        }
    }
    Sellers_data_node *dptr;
    int k = 0;
    int flag = 0;
    for (int i = 0; i < nptr->numkeys; i++)
    {

        if (t.sid >= nptr->keys[i])
        {
            k = i;
            flag = 1;
        }
    }
    if (flag)
    {
        dptr = nptr->data[k + 1];
    }
    else
    {
        dptr = nptr->data[0];
    }
    for (int i = 0; i < dptr->num_data; i++)
    {
        if (dptr->val[i].sid == t.sid)
        {
            dptr->val[i].total_revenue += t.total_price;
            return &dptr->val[i].transaction_subtree;
        }
    }
    return NULL;
}
int get_price(S_Btree_node *root, int sid, int energy)
{
    S_Btree_node *nptr = root;
    while (nptr->flag == 0)
    {
        int k = 0;
        int flag = 0;
        for (int i = 0; i < nptr->numkeys; i++)
        {
            // if(e==33){
            //     printf("%d " , nptr->keys[i]);
            // }
            if (sid >= nptr->keys[i])
            {
                k = i;
                flag = 1;
            }
        }
        if (flag)
        {
            nptr = nptr->child[k + 1];
        }
        else
        {
            nptr = nptr->child[0];
        }
    }
    Sellers_data_node *dptr;
    int k = 0;
    int flag = 0;
    for (int i = 0; i < nptr->numkeys; i++)
    {

        if (sid >= nptr->keys[i])
        {
            k = i;
            flag = 1;
        }
    }
    if (flag)
    {
        dptr = nptr->data[k + 1];
    }
    else
    {
        dptr = nptr->data[0];
    }

    for (int i = 0; i < dptr->num_data; i++)
    {
        if (dptr->val[i].sid == sid)
        {
            if (energy < 300)
            {
                return dptr->val[i].energy_price_below_300;
            }
            else
            {
                return dptr->val[i].energy_price_above_300;
            }
        }
    }

    return -1;
}

S_Btree_node *sellers_insert(S_Btree_node *root, Sellers S)
{
    if (root == NULL)
    {
        static Sellers arr[4];
        static int i = 0;
        arr[i] = S;
        i++;
        if (i == 4)
        {
            root = sellers_base_case_tree(arr);
        }
    }
    else
    {
        Sellers_data_node *dptr = sellers_search(root, S.sid);
        root = sellers_insert_node(root, dptr, S);
    }

    return root;
}
int get_price2(S_Btree_node *root, int sid, int energy)
{
    S_Btree_node *nptr = root;
    while (nptr->flag == 0)
    {
        int k = 0;
        int flag = 0;
        for (int i = 0; i < nptr->numkeys; i++)
        {
            // if(e==33){
            //     printf("%d " , nptr->keys[i]);
            // }
            if (sid >= nptr->keys[i])
            {
                k = i;
                flag = 1;
            }
        }
        if (flag)
        {
            nptr = nptr->child[k + 1];
        }
        else
        {
            nptr = nptr->child[0];
        }
    }
    Sellers_data_node *dptr;
    int k = 0;
    int flag = 0;
    for (int i = 0; i < nptr->numkeys; i++)
    {

        if (sid >= nptr->keys[i])
        {
            k = i;
            flag = 1;
        }
    }
    if (flag)
    {
        dptr = nptr->data[k + 1];
    }
    else
    {
        dptr = nptr->data[0];
    }

    for (int i = 0; i < dptr->num_data; i++)
    {
        if (dptr->val[i].sid == sid)
        {
            return dptr->val[i].total_revenue;
        }
    }

    return -1;
}

void totalRevenueBySeller(S_Btree_node *sellers_root, int sid)
{
    int price = get_price2(sellers_root, sid, 0);
    if (price == -1)
    {
        printf("Seller with id %d not found.\n", sid);
        return;
    }
    printf("Total revenue of seller %d is %d \n", sid, price);
}
