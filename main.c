#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "buyers.c"
#include "Sellers.c"
#include "transactions.h"

typedef struct B_tree_node Btree_node;

typedef struct bs_node
{
    int buyerID;
    int sellerID;
    int n_transactions;
    struct bs_node *next;
} BS;
BS *bs_pair = NULL;

// ------------------------------------------------------------------------------------------
void add_bs(int Bid, int Sid, S_Btree_node *sellers_root)
{
    BS *nptr, *prev;
    nptr = bs_pair;
    int found = 0;
    prev = NULL;
    while (nptr && !found)
    {
        if (nptr->buyerID == Bid && nptr->sellerID == Sid)
        {
            nptr->n_transactions++;
            found = 1;
            if (nptr->n_transactions >= 5)
            {
                Sellers_data_node *dptr = sellers_search(sellers_root, Sid);
                for (int i = 0; i < dptr->num_data; i++)
                {
                    if (dptr->val[i].sid == Sid)
                    {
                        int flag = 1;
                        B *ptr = dptr->val[i].frequent_buyers;
                        B *bprev;
                        bprev = NULL;
                        int invalid = 0;
                        while (ptr && flag)
                        {
                            if (ptr->buyerID == Bid)
                            {
                                invalid = 1;
                            }
                            bprev = ptr;
                            ptr = ptr->next;
                        }
                        ptr = bprev;
                        if (!invalid)
                        {
                            B *temp;
                            temp = (B *)malloc(sizeof(B));
                            temp->buyerID = Bid;
                            temp->next = NULL;
                            if (ptr == NULL)
                            {
                                dptr->val[i].frequent_buyers = temp;
                            }
                            else
                            {
                                ptr->next = temp;
                            }
                        }
                    }
                }
            }
        }
        prev = nptr;
        nptr = nptr->next;
    }
    if (!found)
    {
        BS *t;
        t = (BS *)malloc(sizeof(BS));
        t->buyerID = Bid;
        t->sellerID = Sid;
        t->n_transactions = 1;
        t->next = NULL;
        if (prev)
        {
            prev->next = t;
        }
        else
        {
            bs_pair = t;
        }
    }
}

BS *splitBS(BS *head)
{
    BS *fast = head;
    BS *slow = head;

    while (fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;
        if (fast != NULL)
        {
            slow = slow->next;
        }
    }

    BS *temp = slow->next;
    slow->next = NULL;
    return temp;
}

BS *merge_BS(BS *first, BS *second)
{

    if (first == NULL)
        return second;
    if (second == NULL)
        return first;

    if (first->n_transactions > second->n_transactions)
    {

        first->next = merge_BS(first->next, second);
        return first;
    }
    else
    {
        second->next = merge_BS(first, second->next);
        return second;
    }
}

BS *MergeBS(BS *head)
{

    if (head == NULL || head->next == NULL)
        return head;

    // Split the list into two halves
    BS *second = splitBS(head);

    // Recursively sort each half
    head = MergeBS(head);
    second = MergeBS(second);

    // Merge the two sorted halves
    return merge_BS(head, second);
}

void display_BS_pair()
{
    BS *nptr = bs_pair;
    while (nptr != NULL)
    {
        printf("BuyerID : %d , SellerID : %d , NumberOfTransactions : %d \n", nptr->buyerID, nptr->sellerID, nptr->n_transactions);
        nptr = nptr->next;
    }
}

// ------------------------------------------------------------------------------------------

void add_transaction(S_Btree_node **sellers_root, T_Btree_node **transactions_root, Btree_node **buyers_root)
{
    int tid, bid, sid;
    int energy, price;
    char timestamp[20], start[20], end[20];
    int newPrice;
    printf("Enter Transaction ID: ");
    scanf("%d", &tid);
    printf("Enter Buyer ID: ");
    scanf("%d", &bid);
    printf("Enter Seller ID: ");
    scanf("%d", &sid);
    printf("Enter Energy Amount (kWh): ");
    scanf("%d", &energy);
    printf("Enter Timestamp (YYYY-MM-DD): ");
    scanf("%s", timestamp);

    price = get_price(*sellers_root, sid, energy);
    // printf("%d" , price);
    if (price == -1)
    {
        int b_300, a_300;
        printf("new seller found\n");
        printf("enter new price below 300\n");
        scanf("%d", &b_300);
        printf("enter new price above 300\n");
        scanf("%d", &a_300);
        if (energy > 300)
        {
            newPrice = a_300;
        }
        else
        {
            newPrice = b_300;
        }
        Transactions t;
        t.tid = tid;
        t.bid = bid;
        t.sid = sid;
        t.energy = energy;
        t.price = newPrice;
        t.total_price = energy * newPrice;
        strcpy(t.time_stamp, timestamp);
        *transactions_root = t_insert(*transactions_root, t);
        Sellers s = {sid, b_300, a_300, 0, NULL, NULL};
        *sellers_root = sellers_insert(*sellers_root, s);
        // printf("here");
        Buyer_subtree **Bsubtree_root = Buyers_search2(*buyers_root, t);
        if (Bsubtree_root == NULL)
        {
            Buyers b = {bid, energy, NULL};
            *buyers_root = Buyers_insert(*buyers_root, b);
            Bsubtree_root = Buyers_search2(*buyers_root, t);
        }
        // printf("here");
        buyers_subtree_search(Bsubtree_root, t);
        Seller_subtree **Ssubtree_root = Sellers_search2(*sellers_root, t);
        sellers_subtree_search(Ssubtree_root, t);
    }
    else
    {
        Transactions t;
        t.tid = tid;
        t.bid = bid;
        t.sid = sid;
        t.energy = energy;
        t.price = price;
        t.total_price = energy * price;
        strcpy(t.time_stamp, timestamp);
        *transactions_root = t_insert(*transactions_root, t);
        Buyer_subtree **Bsubtree_root = Buyers_search2(*buyers_root, t);
        // printf("here");
        if (Bsubtree_root == NULL)
        {
            Buyers b = {bid, energy, NULL};
            *buyers_root = Buyers_insert(*buyers_root, b);
            Bsubtree_root = Buyers_search2(*buyers_root, t);
        }
        printf("%d", (*Bsubtree_root)->keys[0].bid);
        buyers_subtree_search(Bsubtree_root, t);
        // printf("here");
        Seller_subtree **Ssubtree_root = Sellers_search2(*sellers_root, t);
        sellers_subtree_search(Ssubtree_root, t);
    }
    add_bs(bid, sid, *sellers_root);
}

void write_transactions_to_file(T_Btree_node *root)
{
    FILE *file = fopen("transactions.txt", "w");

    if (file == NULL)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }
    while (root->flag == 0)
    {
        root = root->child[0];
    }
    Transaction_data_node *dptr = root->data[0];

    while (dptr)
    {
        for (int i = 0; i < dptr->num_data; i++)
        {
            dptr->val[i].time_stamp[sizeof(dptr->val[i].time_stamp) - 1] = '\0';
            fprintf(file, "%d %d %d %d %d %d %s \n",
                    dptr->val[i].tid,
                    dptr->val[i].bid,
                    dptr->val[i].sid,
                    dptr->val[i].energy,
                    dptr->val[i].price,
                    dptr->val[i].total_price,
                    dptr->val[i].time_stamp);
        }
        dptr = dptr->next;
    }
    fprintf(file, "%d %d %d %d %d %d %s\n", 0, 0, 0, 0, 0, 0, 0);
    fclose(file);
    printf("Transactions written to file successfully.\n");
}

void read_transactions_from_file(T_Btree_node **transactions_root, Btree_node **buyers_root, S_Btree_node **sellers_root)
{
    FILE *file = fopen("transactions.txt", "r");

    if (file == NULL)
    {
        printf("Error: Unable to open file for reading.\n");
        return;
    }
    int flag = 1;
    int transactionID;
    int buyerID;
    int sellerID;
    int energyInKWH;
    int pricePerKwh;
    int total_price;
    char timeStamp[20];
    while (flag)
    {
        fscanf(file, "%d %d %d %d %d %d %s",
               &transactionID,
               &buyerID,
               &sellerID,
               &energyInKWH,
               &pricePerKwh,
               &total_price,
               &timeStamp);
        if (transactionID == 0)
        {
            flag = 0;
        }
        if (flag)
        {
            Transactions t;
            t.tid = transactionID;
            t.bid = buyerID;
            t.sid = sellerID;
            t.energy = energyInKWH;
            t.price = pricePerKwh;
            t.total_price = total_price;
            timeStamp[sizeof(t.time_stamp) - 1] = '\0';
            strcpy(t.time_stamp, timeStamp);
            *transactions_root = t_insert(*transactions_root, t);
            Buyer_subtree **Bsubtree_root = Buyers_search2(*buyers_root, t);
            buyers_subtree_search(Bsubtree_root, t);
            Seller_subtree **Ssubtree_root = Sellers_search2(*sellers_root, t);
            sellers_subtree_search(Ssubtree_root, t);
            add_bs(buyerID, sellerID, *sellers_root);
        }
    }

    fclose(file);
    printf("Transactions read from file successfully.\n");
}

void write_buyers_to_file(Btree_node *root)
{
    FILE *file = fopen("buyers.txt", "w");

    if (file == NULL)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }
    while (root->flag == 0)
    {
        root = root->child[0];
    }
    Buyers_data_node *dptr = root->data[0];

    while (dptr)
    {
        for (int i = 0; i < dptr->num_data; i++)
        {
            fprintf(file, "%d %d %s\n",
                    dptr->val[i].bid,
                    0,
                    NULL);
        }
        dptr = dptr->next;
    }
    fprintf(file, "%d %d %d %d %d %d %s\n", 0, 0, 0, 0, 0, 0, 0);
    fclose(file);
    printf("Buyers written to file successfully.\n");
}

void read_buyers_from_file(Btree_node **buyers_root)
{
    FILE *file = fopen("buyers.txt", "r");

    if (file == NULL)
    {
        printf("Error: Unable to open file for reading.\n");
        return;
    }
    int flag = 1;
    int buyerID;
    int energy;
    int *t;
    while (flag)
    {
        fscanf(file, "%d %d %s",
               &buyerID,
               &energy,
               &t);
        if (buyerID == 0)
        {
            flag = 0;
        }
        if (flag)
        {
            Buyers B = {buyerID, 0, NULL};
            *buyers_root = Buyers_insert(*buyers_root, B);
        }
    }

    fclose(file);
    printf("Buyers read from file successfully.\n");
}

void write_sellers_to_file(S_Btree_node *root)
{
    FILE *file = fopen("sellers.txt", "w");

    if (file == NULL)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }
    while (root->flag == 0)
    {
        root = root->child[0];
    }
    Sellers_data_node *dptr = root->data[0];

    while (dptr)
    {
        for (int i = 0; i < dptr->num_data; i++)
        {
            fprintf(file, "%d %d %d %d %s %s\n",
                    dptr->val[i].sid,
                    dptr->val[i].energy_price_below_300,
                    dptr->val[i].energy_price_above_300,
                    0,
                    NULL,
                    NULL);
        }
        dptr = dptr->next;
    }
    fprintf(file, "%d %d %d %d %d %d %s\n", 0, 0, 0, 0, 0, 0, 0);
    fclose(file);
    printf("Sellers written to file successfully.\n");
}

void read_sellers_from_file(S_Btree_node **sellers_root)
{
    FILE *file = fopen("sellers.txt", "r");

    if (file == NULL)
    {
        printf("Error: Unable to open file for reading.\n");
        return;
    }
    int flag = 1;
    int sid;
    int price_below;
    int price_above;
    int total_revenue;
    int *t;
    int *t1;
    while (flag)
    {
        fscanf(file, "%d %d %d %d %s %s",
               &sid,
               &price_below,
               &price_above,
               &total_revenue,
               &t,
               &t1);

        if (sid == 0)
        {
            flag = 0;
        }
        if (flag)
        {
            Sellers S = {sid, price_below, price_above, 0, NULL, NULL};
            *sellers_root = sellers_insert(*sellers_root, S);
        }
    }

    fclose(file);
    printf("Sellers read from file successfully.\n");
}

int main()
{

    T_Btree_node *transactions_root = NULL;
    Btree_node *buyers_root = NULL;
    S_Btree_node *sellers_root = NULL;
    read_buyers_from_file(&buyers_root);
    read_sellers_from_file(&sellers_root);
    read_transactions_from_file(&transactions_root, &buyers_root, &sellers_root);

    int choice;
    int flag = 1;

    while (flag)
    {
        printf("\n----------------------------------------------------------------------\n");
        printf("\nEnergy Trading Record Management System\n");
        printf("1. Add New Transaction\n");
        printf("2. Display All Transactions\n");
        printf("3. List Transactions by Seller\n");
        printf("4. List Transactions by Buyer\n");
        printf("5. List Transactions in a Given Time Period\n");
        printf("6. Calculate Total Revenue by Seller\n");
        printf("7. Find and Display (in ascending order) the transactions with the Energy Amounts between certain range.\n");
        printf("8. Sort the list of Buyers based on their energy bought.\n");
        printf("9. Sort all seller/buyer pairs in decreasing order of the number of transactions between them\n");
        printf("14. Exit\n");
        printf("\n----------------------------------------------------------------------\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        if (choice == 14)
        {
            flag = 0;
        }

        int tid, bid, sid;
        int energy, price;
        char timestamp[20], start[20], end[20];
        int Tid;
        int newPrice;
        int newEnergy;
        int energy_range_start;
        int energy_range_end;
        switch (choice)
        {
        case 1:
            add_transaction(&sellers_root, &transactions_root, &buyers_root);
            break;

        case 2:
            t_display_tree_nodes(transactions_root);
            printf("\n");
            break;
        case 3:
            Sellers_display_tree_tabular(sellers_root);
            break;
        case 4:
            Buyers_display_tree_tabular(buyers_root);
            break;

        case 5:
            printf("Enter Start Date (YYYY-MM-DD): ");
            scanf("%s", &start);
            printf("Enter End Date (YYYY-MM-DD): ");
            scanf("%s", &end);
            listTransactionsInPeriod(transactions_root, start, end);

            break;

        case 6:
            printf("Enter Seller ID: ");
            scanf("%d", &sid);
            totalRevenueBySeller(sellers_root, sid);
            break;
        case 7:

            printf("Enter starting of energy range: ");
            scanf("%d", &energy_range_start);
            printf("Enter ending of energy range: ");
            scanf("%d", &energy_range_end);
            display_energy_in_range(transactions_root, energy_range_start, energy_range_end);
            break;
        case 8:
            printf("buyers list before sorting\n");
            Buyers_display_tree_tabular(buyers_root);

            printf("Sorted list of buyers based on based on energy bought:\n");
            sort_buyers_list(buyers_root);
            break;
        case 9:

            bs_pair = MergeBS(bs_pair);
            printf("Sorted list of buyers and sellers based on number of transactions:\n");
            display_BS_pair();
            break;
        case 14:
            printf("Thankyou!");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
            break;
        }
    }
    write_transactions_to_file(transactions_root);
    write_sellers_to_file(sellers_root);
    write_buyers_to_file(buyers_root);
}
