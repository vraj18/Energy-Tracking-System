#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
typedef struct t_node
{
    int transactionID;
    int buyerID;
    int sellerID;
    int energyInKWH;
    int pricePerKwh;
    char timeStamp[20];
    struct t_node *next;
} T;
T *transactions = NULL;

typedef struct b_node
{
    int buyerID;
    struct b_node *next;
} B;
B *Buyers = NULL;
typedef struct s_node
{
    int sellerID;
    int pricePerKWHb300;
    int pricePerKWHa300;
    B *frequent_buyers;
    struct s_node *next;
} S;
S *Sellers = NULL;

typedef struct sl_node
{
    int sellerID;
    T *seller_transactions;
    struct sl_node *next;
} SL;
SL *seller_list = NULL;
typedef struct bl_node
{
    int buyerID;
    int energyBought;
    T *buyer_transactions;
    struct bl_node *next;
} BL;
BL *buyer_list = NULL;

typedef struct bs_node
{
    int buyerID;
    int sellerID;
    int n_transactions;
    struct bs_node *next;
} BS;
BS *bs_pair = NULL;

//------------------------------------FILE HANDLING------------------------------------------------------------
void sellers(int Sid, int price_below, int price_above);

void add_transaction(int Tid, int Bid, int Sid, int energy, char *time);
void write_transactions_to_file()
{
    FILE *file = fopen("transactions.txt", "w");

    if (file == NULL)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }
    T *nptr;
    nptr = transactions;
    while (nptr)
    {
        fprintf(file, "%d %d %d %d %d %s\n",
                nptr->transactionID,
                nptr->buyerID,
                nptr->sellerID,
                nptr->energyInKWH,
                nptr->pricePerKwh,
                nptr->timeStamp);
        nptr = nptr->next;
    }
    fprintf(file, "%d %d %d %d %d %s\n", 0, 0, 0, 0, 0, 0);
    fclose(file);
    printf("Transactions written to file successfully.\n");
}
void read_transactions_from_file()
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
    char timeStamp[20];
    while (flag)
    {
        fscanf(file, "%d %d %d %d %d %s",
               &transactionID,
               &buyerID,
               &sellerID,
               &energyInKWH,
               &pricePerKwh,
               &timeStamp);
        if (transactionID == 0)
        {
            flag = 0;
        }
        if (flag)
        {
            add_transaction(transactionID, buyerID, sellerID, energyInKWH, timeStamp);
        }
    }

    fclose(file);
    printf("Transactions read from file successfully.\n");
}

void write_sellers_to_file()
{
    FILE *file = fopen("sellers.txt", "w");

    if (file == NULL)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }
    S *nptr;
    nptr = Sellers;
    while (nptr)
    {
        fprintf(file, "%d %d %d\n",
                nptr->sellerID,
                nptr->pricePerKWHb300,
                nptr->pricePerKWHa300);
        nptr = nptr->next;
    }
    fprintf(file, "%d %d %d", 0, 0, 0);
    fclose(file);
    printf("Sellers written to file successfully.\n");
}

void read_sellers_from_file()
{
    FILE *file = fopen("sellers.txt", "r");

    if (file == NULL)
    {
        printf("Error: Unable to open file for reading.\n");
        return;
    }
    int flag = 1;
    int sellerID;
    int pricebelow;
    int priceabove;
    char timeStamp[20];
    while (flag)
    {
        fscanf(file, "%d %d %d",
               &sellerID,
               &pricebelow,
               &priceabove);
        if (sellerID == 0)
        {
            flag = 0;
        }
        if (flag)
        {
            sellers(sellerID, pricebelow, priceabove);
        }
    }
    fclose(file);
    printf("Sellers read from file successfully.\n");
}

void sellers(int Sid, int price_below, int price_above);
int return_price(int Sid, int energy)
{
    S *nptr = Sellers;
    int ret_val = 0;
    int found = 0;
    while (nptr)
    {
        if (nptr->sellerID == Sid)
        {
            found = 1;
            if (energy > 300)
            {
                ret_val = nptr->pricePerKWHa300;
            }
            else
            {
                ret_val = nptr->pricePerKWHb300;
            }
        }
        nptr = nptr->next;
    }
    if (!found)
    {
        printf("new seller Id added\n");
        int energyb300;
        printf("enter new price for energy below 300 KWH\n");
        scanf("%d", &energyb300);
        int energya300;
        printf("enter new price for energy above 300 KWH\n");
        scanf("%d", &energya300);
        sellers(Sid, energyb300, energya300);
        if (energy > 300)
        {
            ret_val = energya300;
        }
        else
        {
            ret_val = energyb300;
        }
    }
    return ret_val;
}

void sellers(int Sid, int price_below, int price_above)
{
    S *nptr;
    nptr = Sellers;
    if (Sellers)
    {
        while (nptr->next)
        {
            nptr = nptr->next;
        }
        S *temp;
        temp = (S *)malloc(sizeof(S));
        temp->sellerID = Sid;
        temp->pricePerKWHb300 = price_below;
        temp->pricePerKWHa300 = price_above;
        temp->frequent_buyers = NULL;
        nptr->next = temp;
        temp->next = NULL;
    }
    else
    {
        S *temp;
        temp = (S *)malloc(sizeof(S));
        temp->sellerID = Sid;
        temp->pricePerKWHb300 = price_below;
        temp->pricePerKWHa300 = price_above;
        temp->frequent_buyers = NULL;
        Sellers = temp;
        temp->next = NULL;
    }
}

void display_sellers()
{
    S *nptr;
    nptr = Sellers;
    while (nptr)
    {
        printf("SellerID : %d , Price Below 300 : %d , Price above 300 : %d ", nptr->sellerID, nptr->pricePerKWHb300, nptr->pricePerKWHa300);
        B *lptr;
        lptr = nptr->frequent_buyers;
        while (lptr)
        {
            printf(" BuyerID : %d", lptr->buyerID);
            lptr = lptr->next;
        }
        printf("\n");
        nptr = nptr->next;
    }
}
//--------------------------------Q1 . Add transaction-------------------------------------------------------
void add_seller(int Tid, int Bid, int Sid, int energy, int price, char *time);

void add_buyer(int Tid, int Bid, int Sid, int energy, int price, char *time);

void add_bs(int Bid, int Sid)
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
                S *lptr = Sellers;
                int flag = 1;
                while (lptr && flag)
                {
                    if (lptr->sellerID == Sid)
                    {
                        flag = 0;
                        B *ptr = lptr->frequent_buyers;
                        B *bprev;
                        bprev = NULL;
                        int invalid = 0;
                        while (ptr)
                        {
                            if(ptr->buyerID==Bid){
                                invalid = 1;
                            }
                            bprev = ptr;
                            ptr = ptr->next;
                        }
                        ptr = bprev;
                        if(!invalid){
                            B *temp;
                            temp = (B *)malloc(sizeof(B));
                            temp->buyerID = Bid;
                            temp->next = NULL;
                            if (ptr == NULL)
                            {
                                lptr->frequent_buyers = temp;
                            }
                            else
                            {
                                ptr->next = temp;
                            }
                        }
                    }
                    lptr = lptr->next;
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

void add_transaction(int Tid, int Bid, int Sid, int energy, char *time)
{
    int price = return_price(Sid, energy);
    if (transactions == NULL)
    {
        transactions = (T *)malloc(sizeof(T));
        transactions->transactionID = Tid;
        transactions->buyerID = Bid;
        transactions->sellerID = Sid;
        transactions->energyInKWH = energy;
        transactions->pricePerKwh = price;
        transactions->next = NULL;
        strcpy(transactions->timeStamp, time);
        add_seller(Tid, Bid, Sid, energy, price, time);
        add_buyer(Tid, Bid, Sid, energy, price, time);
        add_bs(Bid, Sid);
    }
    else
    {
        T *nptr, *t, *prev;
        nptr = transactions;
        int invalid = 0;
        while (nptr != NULL && !invalid)
        {
            if (nptr->transactionID == Tid)
            {
                invalid = 1;
            }
            prev = nptr;
            nptr = nptr->next;
        }
        if (!invalid)
        {
            t = (T *)malloc(sizeof(T));
            t->transactionID = Tid;
            t->buyerID = Bid;
            t->sellerID = Sid;
            t->energyInKWH = energy;
            t->pricePerKwh = price;
            strcpy(t->timeStamp, time);
            prev->next = t;
            t->next = NULL;
            add_seller(Tid, Bid, Sid, energy, price, time);
            add_buyer(Tid, Bid, Sid, energy, price, time);
            add_bs(Bid, Sid);
        }
        else
        {
            printf("enter a unique transaction ID");
        }
    }
}

//--------------------------------Q2 . Display all transactions-------------------------------------------------------

void display_transactions()
{
    T *nptr;
    nptr = transactions;
    while (nptr)
    {
        printf("transaction ID : %d , BuyerId : %d , SellerId : %d , EnergyInKwh: %d , Price : %d , TimeStamp : %s \n", nptr->transactionID, nptr->buyerID, nptr->sellerID, nptr->energyInKWH, nptr->pricePerKwh, nptr->timeStamp);
        nptr = nptr->next;
    }
}

//----------------------------------Q.3------------------------------------------------------------------------

void display_sellers_list()
{
    SL *slptr;
    slptr = seller_list;
    while (slptr)
    {
        printf("SellerID : %d\n", slptr->sellerID);
        T *nptr;
        nptr = slptr->seller_transactions;
        while (nptr)
        {
            printf("transaction ID : %d , BuyerId : %d , SellerId : %d , EnergyInKwh: %d , Price : %d , TimeStamp : %s \n", nptr->transactionID, nptr->buyerID, nptr->sellerID, nptr->energyInKWH, nptr->pricePerKwh, nptr->timeStamp);
            nptr = nptr->next;
        }
        slptr = slptr->next;
    }
}

void add_seller(int Tid, int Bid, int Sid, int energy, int price, char *time)
{

    if (seller_list == NULL)
    {
        seller_list = (SL *)malloc(sizeof(SL));
        seller_list->sellerID = Sid;
        T *temp1 = (T *)malloc(sizeof(T));
        temp1->buyerID = Bid;
        temp1->sellerID = Sid;
        temp1->energyInKWH = energy;
        temp1->pricePerKwh = price;
        temp1->transactionID = Tid;
        strcpy(temp1->timeStamp, time);
        temp1->next = NULL;
        seller_list->seller_transactions = temp1;
        seller_list->seller_transactions->next = NULL;
        seller_list->next = NULL;
    }
    else
    {
        SL *slptr, *temp, *slprev;
        slptr = seller_list;
        int invalid = 0;

        while (slptr != NULL && !invalid)
        {
            if (slptr->sellerID == Sid)
            {
                T *nptr, *t;
                nptr = slptr->seller_transactions;
                while (nptr->next != NULL)
                {
                    nptr = nptr->next;
                }
                t = (T *)malloc(sizeof(T));
                t->transactionID = Tid;
                t->buyerID = Bid;
                t->sellerID = Sid;
                t->energyInKWH = energy;
                t->pricePerKwh = price;
                strcpy(t->timeStamp, time);
                nptr->next = t;
                t->next = NULL;
                invalid = 1;
            }
            slprev = slptr;
            slptr = slptr->next;
        }
        if (!invalid)
        {
            T *temp1 = (T *)malloc(sizeof(T));
            temp1->buyerID = Bid;
            temp1->sellerID = Sid;
            temp1->energyInKWH = energy;
            temp1->pricePerKwh = price;
            temp1->transactionID = Tid;
            strcpy(temp1->timeStamp, time);
            temp1->next = NULL;
            temp = (SL *)malloc(sizeof(SL));
            temp->sellerID = Sid;
            temp->seller_transactions = temp1;
            temp->next = NULL;
            slprev->next = temp;
        }
    }
}

//----------------------------------Q.4------------------------------------------------------------------------

void display_buyers_list()
{
    BL *slptr;
    slptr = buyer_list;
    while (slptr)
    {
        printf("BuyerID : %d , EnergyBought : %d \n", slptr->buyerID, slptr->energyBought);
        T *nptr;
        nptr = slptr->buyer_transactions;
        while (nptr)
        {
            printf("transaction ID : %d , BuyerId : %d , SellerId : %d , EnergyInKwh: %d , Price : %d , TimeStamp : %s \n", nptr->transactionID, nptr->buyerID, nptr->sellerID, nptr->energyInKWH, nptr->pricePerKwh, nptr->timeStamp);
            nptr = nptr->next;
        }
        slptr = slptr->next;
    }
}

void add_buyer(int Tid, int Bid, int Sid, int energy, int price, char *time)
{

    if (buyer_list == NULL)
    {
        buyer_list = (BL *)malloc(sizeof(BL));
        buyer_list->buyerID = Bid;
        buyer_list->energyBought = energy;
        T *temp1 = (T *)malloc(sizeof(T));
        temp1->buyerID = Bid;
        temp1->sellerID = Sid;
        temp1->energyInKWH = energy;
        temp1->pricePerKwh = price;
        temp1->transactionID = Tid;
        strcpy(temp1->timeStamp, time);
        temp1->next = NULL;
        buyer_list->buyer_transactions = temp1;
        buyer_list->buyer_transactions->next = NULL;
        buyer_list->next = NULL;
    }
    else
    {
        BL *slptr, *temp, *slprev;
        slptr = buyer_list;
        int invalid = 0;

        while (slptr != NULL && !invalid)
        {
            if (slptr->buyerID == Bid)
            {
                slptr->energyBought += energy;
                T *nptr, *t;
                nptr = slptr->buyer_transactions;
                while (nptr->next != NULL)
                {
                    nptr = nptr->next;
                }
                t = (T *)malloc(sizeof(T));
                t->transactionID = Tid;
                t->buyerID = Bid;
                t->sellerID = Sid;
                t->energyInKWH = energy;
                t->pricePerKwh = price;
                strcpy(t->timeStamp, time);
                nptr->next = t;
                t->next = NULL;
                invalid = 1;
            }
            slprev = slptr;
            slptr = slptr->next;
        }
        if (!invalid)
        {
            T *temp1 = (T *)malloc(sizeof(T));
            temp1->buyerID = Bid;
            temp1->sellerID = Sid;
            temp1->energyInKWH = energy;
            temp1->pricePerKwh = price;
            temp1->transactionID = Tid;
            strcpy(temp1->timeStamp, time);
            temp1->next = NULL;
            temp = (BL *)malloc(sizeof(BL));
            temp->buyerID = Bid;
            temp->energyBought = energy;
            temp->buyer_transactions = temp1;
            temp->next = NULL;
            slprev->next = temp;
        }
    }
}

//----------------------------------Q.5------------------------------------------------------------------------
int isWithinPeriod(const char *timestamp, const char *start, const char *end)
{
    return strcmp(timestamp, start) >= 0 && strcmp(timestamp, end) <= 0;
}

void listTransactionsInPeriod(const char *start, const char *end)
{
    int found = 0;
    printf("Transactions between %s and %s:\n", start, end);
    T *nptr;
    nptr = transactions;
    while (nptr)
    {
        if (isWithinPeriod(nptr->timeStamp, start, end))
        {
            found = 1;
            printf("Transaction ID: %d, Buyer ID: %d, Seller ID: %d, Energy: %d kWh, Price: %d /kWh, Timestamp: %s\n",
                   nptr->transactionID, nptr->buyerID,
                   nptr->sellerID, nptr->energyInKWH,
                   nptr->pricePerKwh, nptr->timeStamp);
        }
        nptr = nptr->next;
    }
    if (!found)
    {
        printf("No transactions found in the specified period.\n");
    }
}

//------------------------------------------Q.6-----------------------------------------------------------
void totalRevenueBySeller(int sellerID)
{
    int totalRevenue = 0;

    printf("Transactions for Seller %d:\n", sellerID);
    bool hasTransactions = false;
    SL *slptr;
    slptr = seller_list;
    while (slptr && !hasTransactions)
    {
        if (slptr->sellerID == sellerID)
        {
            T *nptr;
            nptr = slptr->seller_transactions;
            while (nptr)
            {
                int transactionRevenue = nptr->energyInKWH * nptr->pricePerKwh;
                totalRevenue += transactionRevenue;
                printf("  TransactionID: %d, BuyerID: %d, Energy: %d kWh, Price: %d\n",
                       nptr->transactionID,
                       nptr->buyerID,
                       nptr->energyInKWH,
                       nptr->pricePerKwh);
                nptr = nptr->next;
            }
            hasTransactions = true;
        }
        slptr = slptr->next;
    }

    if (hasTransactions)
    {
        printf("Total Revenue for Seller %d: $%d\n", sellerID, totalRevenue);
    }
    else
    {
        printf("No transactions found for Seller %d.\n", sellerID);
    }
}

//------------------------------------------------------Q.7--------------------------------------------------------
T *splitT(T *head1)
{
    T *fast = head1;
    T *slow = head1;

    while (fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;
        if (fast != NULL)
        {
            slow = slow->next;
        }
    }
    T *temp = slow->next;
    slow->next = NULL;
    return temp;
}

T *merge_T(T *first, T *second)
{

    if (first == NULL)
        return second;
    if (second == NULL)
        return first;

    if (first->energyInKWH * first->pricePerKwh < second->energyInKWH * second->pricePerKwh)
    {
        first->next = merge_T(first->next, second);
        return first;
    }
    else
    {
        second->next = merge_T(first, second->next);
        return second;
    }
}

T *MergeT(T *head)
{
    if (head == NULL || head->next == NULL)
        return head;

    T *second = splitT(head);
    head = MergeT(head);
    second = MergeT(second);

    return merge_T(head, second);
}
//---------------------------------------------------Q.8---------------------------------------------------------------------------
BL *splitBL(BL *head)
{
    BL *fast = head;
    BL *slow = head;

    while (fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;
        if (fast != NULL)
        {
            slow = slow->next;
        }
    }

    BL *temp = slow->next;
    slow->next = NULL;
    return temp;
}

BL *merge_BL(BL *first, BL *second)
{

    if (first == NULL)
        return second;
    if (second == NULL)
        return first;

    if (first->energyBought < second->energyBought)
    {

        first->next = merge_BL(first->next, second);
        return first;
    }
    else
    {
        second->next = merge_BL(first, second->next);
        return second;
    }
}

BL *MergeBL(BL *head)
{

    if (head == NULL || head->next == NULL)
        return head;

    // Split the list into two halves
    BL *second = splitBL(head);

    // Recursively sort each half
    head = MergeBL(head);
    second = MergeBL(second);

    // Merge the two sorted halves
    return merge_BL(head, second);
}

//----------------------------------------------Q.9----------------------------------------------------------------

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

//-------------------------------------------Q.10---------------------------------------------------------------
void updateTransaction(int Tid, int newPrice, int newEnergy)
{
    T *t = transactions;
    BL *buyer;
    SL *seller;
    int found = 0;

    while (t != NULL && !found)
    {
        if (t->transactionID == Tid)
        {
            found = 1;
            t->energyInKWH = newEnergy;
            t->pricePerKwh = newPrice;
            seller = seller_list;
            buyer = buyer_list;
            while (buyer->buyerID != t->buyerID)
            {
                buyer = buyer->next;
            }
            T *nptr;
            nptr = buyer->buyer_transactions;
            while (nptr->transactionID != Tid)
            {
                nptr = nptr->next;
            }
            nptr->energyInKWH = newEnergy;
            nptr->pricePerKwh = newPrice;

            while (seller->sellerID != t->sellerID)
            {
                seller = seller->next;
            }
            T *lptr;
            lptr = seller->seller_transactions;
            while (lptr->transactionID != Tid)
            {
                lptr = lptr->next;
            }
            lptr->energyInKWH = newEnergy;
            lptr->pricePerKwh = newPrice;
        }
        t = t->next;
    }
}

int main()
{
    int choice;
    int flag = 1;
    read_sellers_from_file();
    read_transactions_from_file();
    while (flag)
    {
        printf("\n----------------------------------------------------------------------\n");
        printf("\nEnergy Trading Record Management System\n");
        printf("1. Add New Transaction\n");
        printf("2. Display All Transactions\n");
        printf("3. List Transactions by Seller\n");
        printf("4. List Transactions by Buyer\n");
        printf("5. List Transactions in a Given Time Period\n");
        printf("7. Calculate Total Revenue by Seller\n");
        printf("8. Sort the list of Buyers based on their energy bought.\n");
        printf("9. Find and Display the Transaction with the Highest Energy Amount\n");
        printf("10. Update the details of an existing transaction.\n");
        printf("11. Display the list of all sellers.\n");
        printf("12. Sort all seller/buyer pairs in decreasing order of the number of transactions between them\n");
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
        T *head1;
        switch (choice)
        {
        case 1:
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
            add_transaction(tid, bid, sid, energy, timestamp);
            break;

        case 2:
            display_transactions();
            break;

        case 3:
            display_sellers_list();
            break;

        case 4:

            display_buyers_list();
            break;

        case 5:
            printf("Enter Start Date (YYYY-MM-DD): ");
            scanf("%s", start);
            printf("Enter End Date (YYYY-MM-DD): ");
            scanf("%s", end);
            listTransactionsInPeriod(start, end);

            break;

        case 7:
            printf("Enter Seller ID: ");
            scanf("%d", &sid);
            totalRevenueBySeller(sid);
            break;
        case 8:
            printf("buyers list before sorting\n");
            display_buyers_list();
            printf("Sorted list of buyers based on based on energy bought:\n");
            buyer_list = MergeBL(buyer_list);
            display_buyers_list();
            break;
        case 9:
            head1 = transactions;
            transactions = MergeT(head1);
            display_transactions();
            break;
        case 10:

            printf("Enter the transactionID: ");
            scanf("%d", &Tid);
            printf("Enter the new Price: ");
            scanf("%d", &newPrice);
            printf("Enter the new Energy: ");
            scanf("%d", &newEnergy);

            updateTransaction(Tid, newPrice, newEnergy);
            break;
        case 11:
            display_sellers();
            break;
        case 12:
            printf("before sorting buyer seller pairs : \n");
            display_BS_pair();
            BS *head = bs_pair;
            bs_pair = MergeBS(head);
            printf("\nAFter sorting buyer seller pairs : \n");
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
    write_transactions_to_file();
    write_sellers_to_file();
    return 0;
}
