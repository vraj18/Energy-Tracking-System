#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct T_B_tree_node T_Btree_node;

typedef struct Transactions_tag
{
    int tid;
    int bid;
    int sid;
    int energy;
    int price;
    int total_price;
    char time_stamp[11];
} Transactions;

typedef struct T_Data_node_tag
{
    Transactions val[3];
    int num_data;
    struct T_Data_node_tag *next;
    struct T_Data_node_tag *prev;
    T_Btree_node *parent;
} Transaction_data_node;

typedef struct T_B_tree_node
{
    int numkeys;
    int keys[2];
    int flag;
    struct T_B_tree_node *parent;
    union
    {
        struct T_B_tree_node *child[3];
        Transaction_data_node *data[3];
    };
} T_Btree_node;

int t_compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int t_median(Transactions *ptr)
{

    qsort(ptr, 4, sizeof(Transactions), t_compare);
    return ptr[2].tid;
}

T_Btree_node *t_base_case_tree(Transactions arr[])
{
    // making Btree node
    T_Btree_node *root = (T_Btree_node *)malloc(sizeof(T_Btree_node));
    root->flag = 1;
    root->keys[0] = t_median(arr);
    root->parent = NULL;
    root->numkeys = 1;
    // making data nodes
    root->data[0] = (Transaction_data_node *)malloc(sizeof(Transaction_data_node));
    root->data[0]->val[0] = arr[0];
    root->data[0]->num_data = 1;
    root->data[0]->val[1] = arr[1];
    root->data[0]->num_data++;
    root->data[0]->parent = root;

    root->data[1] = (Transaction_data_node *)malloc(sizeof(Transaction_data_node));
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

void sort_buyer_seller_pair() {}

int partition_transactions(Transactions arr[], int low, int high)
{
    int pivot = arr[high].energy; // Pivot is the total_energy of the last element
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j].energy <= pivot)
        {
            i++;
            // Swap arr[i] and arr[j]
            Transactions temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // Swap arr[i + 1] and arr[high] (pivot)
    Transactions temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// Quick sort function
void quick_sort_transactions(Transactions arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition_transactions(arr, low, high); // Partition index

        // Recursively sort elements before and after partition
        quick_sort_transactions(arr, low, pi - 1);
        quick_sort_transactions(arr, pi + 1, high);
    }
}
void display_energy_in_range(T_Btree_node *root, int start, int end)
{
    while (root->flag == 0)
    {
        root = root->child[0];
    }
    Transaction_data_node *dptr = root->data[0];
    Transaction_data_node *temp = dptr;
    int count = 0;
    while (dptr)
    {
        for (int i = 0; i < dptr->num_data; i++)
        {
            if (dptr->val[i].energy >= start && dptr->val[i].energy <= end)
            {
                count++;
            }
        }
        dptr = dptr->next;
    }
    int j = 0;
    Transactions *arr = (Transactions *)malloc(count * sizeof(Transactions));
    while (temp)
    {
        for (int i = 0; i < temp->num_data; i++)
        {
            if (temp->val[i].energy >= start && temp->val[i].energy <= end)
            {
                arr[j++] = temp->val[i];
            }
        }
        temp = temp->next;
    }
    quick_sort_transactions(arr, 0, count - 1);
    printf("Transactions in the range [%d , %d] are :\n", start, end);
    for (int i = 0; i < count; i++)
    {
        printf("Transaction_id :%d  , Buyer_id: %d , Seller_id: %d, Energy: %d, price: %d , total_price: %d  , time_stamp: %s\n", arr[i].tid, arr[i].bid, arr[i].sid, arr[i].energy, arr[i].price, arr[i].total_price, arr[i].time_stamp);
    }
}

// no need ?? what does this function do?

// void t_display_tree(T_Btree_node *root)
// {
//     if (root)
//     {
//         for (int i = 0; i < root->numkeys; i++)
//         {
//             printf("%d ", root->keys[i]);
//         }
//         if (root->flag == 0)
//         {
//             for (int i = 0; i <= root->numkeys; i++)
//             {
//                 t_display_tree(root->child[i]);
//             }
//         }
//     }
// }

void t_display_tree_nodes(T_Btree_node *root)
{
    while (root->flag == 0)
    {
        root = root->child[0];
    }
    Transaction_data_node *dptr = root->data[0];

    while (dptr)
    {
        for (int i = 0; i < dptr->num_data; i++)
        {
            printf("Transaction_id :%d  , Buyer_id: %d , Seller_id: %d, Energy: %d, price: %d , total_price: %d  , time_stamp: %s\n", dptr->val[i].tid, dptr->val[i].bid, dptr->val[i].sid, dptr->val[i].energy, dptr->val[i].price, dptr->val[i].total_price, dptr->val[i].time_stamp);
        }
        dptr = dptr->next;
    }
}

int isWithinPeriod(const char *timestamp, const char *start, const char *end)
{
    return strcmp(timestamp, start) >= 0 && strcmp(timestamp, end) <= 0;
}
void listTransactionsInPeriod(T_Btree_node *root, const char *start, const char *end)
{
    while (root->flag == 0)
    {
        root = root->child[0];
    }
    Transaction_data_node *dptr = root->data[0];

    while (dptr)
    {
        for (int i = 0; i < dptr->num_data; i++)
        {
            if (isWithinPeriod(dptr->val[i].time_stamp, start, end))
            {
                printf("Transaction_id :%d  , Buyer_id: %d , Seller_id: %d, Energy: %d, price: %d , total_price: %d  , time_stamp: %s\n", dptr->val[i].tid, dptr->val[i].bid, dptr->val[i].sid, dptr->val[i].energy, dptr->val[i].price, dptr->val[i].total_price, dptr->val[i].time_stamp);
            }
        }
        dptr = dptr->next;
    }
}

T_Btree_node *t_insert_btree(T_Btree_node **head, T_Btree_node *node, int value)
{
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
        T_Btree_node *new = (T_Btree_node *)malloc(sizeof(T_Btree_node));
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
        node->numkeys = 1;
        node->keys[0] = prr[0];
        new->keys[0] = prr[2];
        new->numkeys = 1;
        if (node->parent == NULL)
        {
            T_Btree_node *root = (T_Btree_node *)malloc(sizeof(T_Btree_node));
            root->keys[0] = prr[1];
            root->numkeys = 1;
            root->child[0] = node;
            root->child[1] = new;
            root->flag = 0;
            root->parent = NULL;
            node->parent = new->parent = root;
            *head = root;
        }
        else
        {
            T_Btree_node *parent = node->parent;
            T_Btree_node *t_n = t_insert_btree(head, node->parent, prr[1]);

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
                T_Btree_node *arr[4];
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

T_Btree_node *t_insert_node(T_Btree_node *root, Transaction_data_node *dptr, Transactions trans)
{
    if (dptr->num_data < 3)
    {
        int t;
        for (int i = 0; i < dptr->num_data; i++)
        {
            if (dptr->val[i].tid > trans.tid)
            {
                t = dptr->val[i].tid;
                dptr->val[i] = trans;
                trans.tid = t;
            }
        }
        dptr->val[dptr->num_data] = trans;
        dptr->num_data++;
    }
    else
    {
        Transactions arr[4];
        for (int i = 0; i < 3; i++)
        {
            arr[i] = dptr->val[i];
        }
        arr[3] = trans;
        int parent_value = t_median(arr);
        T_Btree_node *parent = dptr->parent;
        Transaction_data_node *t = (Transaction_data_node *)malloc(sizeof(Transaction_data_node));
        for (int i = 0; i < 2; i++)
        {
            dptr->val[i] = arr[i];
        }
        // dptr->val[2].tid = 0;
        dptr->num_data = 2;
        for (int i = 0; i < 2; i++)
        {
            t->val[i] = arr[i + 2];
        }
        // t->val[2].tid = 0;
        t->num_data = 2;
        t->parent = parent;
        t->next = dptr->next;
        t->prev = dptr;
        if (dptr->next)
            dptr->next->prev = t;
        dptr->next = t;
        if (dptr->parent->numkeys < 2)
        {
            if (parent->keys[0] < arr[2].tid)
            {
                parent->keys[1] = arr[2].tid;
                parent->data[2] = t;
                parent->numkeys = 2;
            }
            else
            {
                parent->keys[1] = parent->keys[0];
                parent->keys[0] = arr[2].tid;
                parent->data[2] = parent->data[1];
                parent->data[1] = t;
                parent->numkeys = 2;
            }
        }
        else
        {

            T_Btree_node *new = t_insert_btree(&root, parent, arr[2].tid);
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

Transaction_data_node *t_search(T_Btree_node *root, Transactions trans)
{
    T_Btree_node *nptr = root;
    while (nptr->flag == 0)
    {
        int k = 0;
        int flag = 0;
        for (int i = 0; i < nptr->numkeys; i++)
        {
            // if(e==33){
            //     printf("%d " , nptr->keys[i]);
            // }
            if (trans.tid >= nptr->keys[i])
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
    Transaction_data_node *dptr;
    int k = 0;
    int flag = 0;
    for (int i = 0; i < nptr->numkeys; i++)
    {

        if (trans.tid >= nptr->keys[i])
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
    return dptr;
}

T_Btree_node *t_insert(T_Btree_node *root, Transactions trans)
{

    if (root == NULL)
    {
        static Transactions arr[4];
        static int i = 0;
        arr[i] = trans;
        i++;
        if (i == 4)
        {
            root = t_base_case_tree(arr);
        }
    }
    else
    {
        Transaction_data_node *dptr = t_search(root, trans);
        root = t_insert_node(root, dptr, trans);
    }

    return root;
}
