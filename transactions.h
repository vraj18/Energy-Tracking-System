#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <stdbool.h>

typedef struct T_B_tree_node T_Btree_node;

typedef struct Transactions_tag {
    int tid;
    int bid;
    int sid;
    int energy;
    int price;
    int total_price;
    char time_stamp[12];
} Transactions;

typedef struct T_Data_node_tag {
    Transactions val[3];
    int num_data;
    struct T_Data_node_tag *next;
    struct T_Data_node_tag *prev;
    T_Btree_node *parent;
} Transaction_data_node;

typedef struct T_B_tree_node {
    int numkeys;
    int keys[2];
    int flag;
    struct T_B_tree_node *parent;
    union {
        struct T_B_tree_node *child[3];
        Transaction_data_node *data[3];
    };
} T_Btree_node;

int t_compare(const void *a, const void *b);
int t_median(Transactions *ptr);
T_Btree_node* t_base_case_tree(Transactions arr[]);
void t_display_tree(T_Btree_node *root);
void t_display_tree_nodes(T_Btree_node *root);
T_Btree_node* t_insert_btree(T_Btree_node **head, T_Btree_node *node, int value);
T_Btree_node* t_insert_node(T_Btree_node *root, Transaction_data_node *dptr, Transactions trans);
Transaction_data_node* t_search(T_Btree_node *root, Transactions trans);
T_Btree_node* t_insert(T_Btree_node *root, Transactions trans);
void listTransactionsInPeriod(T_Btree_node *root, const char *start, const char *end);
void display_energy_in_range(T_Btree_node *root , int start , int end);
#endif // TRANSACTIONS_H