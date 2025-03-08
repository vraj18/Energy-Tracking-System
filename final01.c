#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX_TRANSACTIONS 500



// -------------------------------------------------------------------------------------------

struct Transaction
{
    int transactionID;
    int buyerID;
    int sellerID;
    double energyInKWH;
    double pricePerKwh;
    char timeStamp[20];
};
int transactionCount ;  
struct Transaction transactions[MAX_TRANSACTIONS];

struct Buyer
{
    int buyerID;
    double energyInKWH;
    double revenueGenerated;
};
int buyerCount ;
struct Buyer buyers[MAX_TRANSACTIONS];


struct Seller
{
    int sellerID;
    double energyInKWH;
    double revenueGenerated;
};
int sellerCount ;
struct Seller sellers[MAX_TRANSACTIONS];

struct BuyerSellerPair
{
    int buyerID;
    int sellerID;
    double energyInKWH;
    double revenueGenerated;
    int BSCount;
};
int pairCount;
struct BuyerSellerPair pairs[MAX_TRANSACTIONS];


struct SellerList{
    int sellerID;
    int transaction_count;
    struct Transaction seller_transactions[MAX_TRANSACTIONS];
};
struct SellerList sellerList[MAX_TRANSACTIONS];


struct BuyerList{
    int buyerID;
    int transaction_count;
    struct Transaction buyer_transactions[MAX_TRANSACTIONS];
};
struct BuyerList buyerList[MAX_TRANSACTIONS];

//--------------------------------------------file handling-------------------------------------------------
//---------------------------------buyer-seller-pair file handling------------------------------------------
void write_pairs_to_file(struct BuyerSellerPair pairs[], int size) {
    FILE *file = fopen("buyersellerpairs.txt", "w");

    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d %d %.2f %.2f %d\n",
                pairs[i].buyerID,
                pairs[i].sellerID,
                pairs[i].energyInKWH,
                pairs[i].revenueGenerated,
                pairs[i].BSCount);
    }

    fclose(file);
    printf("Buyer-Seller Pairs written to file successfully.\n");
}
void read_pairs_from_file(struct BuyerSellerPair pairs[], int size) {
    FILE *file = fopen("buyersellerpairs.txt", "r");

    if (file == NULL) {
        printf("Error: Unable to open file for reading.\n");
        return;
    }
    int flag = 1;
    for (int i = 0; i < size && flag; i++) {
        fscanf(file, "%d %d %lf %lf %d",
               &pairs[i].buyerID,
               &pairs[i].sellerID,
               &pairs[i].energyInKWH,
               &pairs[i].revenueGenerated,
               &pairs[i].BSCount);
               if(pairs[i].buyerID==0){
                pairCount = i;
                flag = 0 ;
               }
    }

    fclose(file);
    printf("Buyer-Seller Pairs read from file successfully.\n");
}
//==========================================================================================================================================
//-------------------------------transactions file handling-----------------------------------------

void write_transactions_to_file(struct Transaction transactions[], int size) {
    FILE *file = fopen("transactions.txt", "w");

    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d %d %d %.2f %.2f %s\n",
                transactions[i].transactionID,
                transactions[i].buyerID,
                transactions[i].sellerID,
                transactions[i].energyInKWH,
                transactions[i].pricePerKwh,
                transactions[i].timeStamp);
    }

    fclose(file);
    printf("Transactions written to file successfully.\n");
}
void read_transactions_from_file(struct Transaction transactions[], int size) {
    FILE *file = fopen("transactions.txt", "r");

    if (file == NULL) {
        printf("Error: Unable to open file for reading.\n");
        return;
    }
    int flag = 1;
    for (int i = 0; i < size && flag; i++) {
        fscanf(file, "%d %d %d %lf %lf %s",
               &transactions[i].transactionID,
               &transactions[i].buyerID,
               &transactions[i].sellerID,
               &transactions[i].energyInKWH,
               &transactions[i].pricePerKwh,
               transactions[i].timeStamp);
               if(transactions[i].transactionID==0){
                    transactionCount = i;
                    flag = 0 ;
               }
    }

    fclose(file);
    printf("Transactions read from file successfully.\n");
}
//==========================================================================================================================================
//-------------------------------sellers file handling-----------------------------------------

void write_sellers_to_file(struct Seller sellers[], int size) {
    FILE *file = fopen("sellers.txt", "w");

    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d %.2f %.2f\n", 
                sellers[i].sellerID, 
                sellers[i].energyInKWH, 
                sellers[i].revenueGenerated);
    }

    fclose(file);
    printf("Sellers written to file successfully.\n");
}
void read_sellers_from_file(struct Seller sellers[], int size) {
    FILE *file = fopen("sellers.txt", "r");

    if (file == NULL) {
        printf("Error: Unable to open file for reading.\n");
        return;
    }
    int flag = 1;
    for (int i = 0; i < size && flag; i++) {
        fscanf(file, "%d %lf %lf", 
               &sellers[i].sellerID, 
               &sellers[i].energyInKWH, 
               &sellers[i].revenueGenerated);
               if(sellers[i].sellerID==0){
                sellerCount = i;
                flag = 0;
               }
    }

    fclose(file);
    printf("Sellers read from file successfully.\n");
}

//==========================================================================================================================================
//-------------------------------buyers file handling-----------------------------------------

void write_buyers_to_file(struct Buyer buyers[], int size) {
    FILE *file = fopen("buyers.txt", "w");

    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d %.2f %.2f\n", 
                buyers[i].buyerID, 
                buyers[i].energyInKWH, 
                buyers[i].revenueGenerated);
    }

    fclose(file);
    printf("Buyers written to file successfully.\n");
}
void read_buyers_from_file(struct Buyer buyers[], int size) {
    FILE *file = fopen("buyers.txt", "r");

    if (file == NULL) {
        printf("Error: Unable to open file for reading.\n");
        return;
    }
    int flag = 1;
    for (int i = 0; i < size && flag ; i++) {
        fscanf(file, "%d %lf %lf", 
               &buyers[i].buyerID, 
               &buyers[i].energyInKWH, 
               &buyers[i].revenueGenerated);
               if(buyers[i].buyerID==0){
                buyerCount = i;
                flag = 0 ;
               }
    }

    fclose(file);
    printf("Buyers read from file successfully.\n");
}

//==========================================================================================================================================

// -------------------------------------------------------------------------------------------

// 1.Add New Transactions: Users should be able to input new energy trading transactions with all the specified attributes
//  (Transaction ID, Buyer ID, Seller ID, Amount of Energy, Price per kWh, Timestamp). The transactions should be stored in
//  the system.


int isTransactionIDUnique(int transactionID) {
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].transactionID == transactionID) {
            return 0;
        }
    }
    return 1;
}


void inertUpdateBuyerSeller(int buyerID, int sellerID,double energyInKWH,double price){
    int found = 0;
    for(int i=0;i<pairCount;i++){
        if((sellerID == pairs[i].buyerID) && (sellerID == pairs[i].sellerID)){
            found = 1;
            pairs[i].sellerID = sellerID;
            pairs[i].buyerID = buyerID;
            pairs[i].energyInKWH += energyInKWH;
            pairs[i].revenueGenerated += energyInKWH*price;
            pairs[i].BSCount += 1;
        }
    }
    if(!found){
        pairs[pairCount].sellerID = sellerID;
        pairs[pairCount].buyerID = buyerID;
        pairs[pairCount].energyInKWH = energyInKWH;
        pairs[pairCount].revenueGenerated = energyInKWH*price;
        pairs[pairCount].BSCount = 1;
        pairCount++;
    }
}
void inertUpdateBuyers(int buyerID,double energyInKWH,double price){
    int found = 0;
    for(int i=0;i<buyerCount;i++){
        if(buyerID == buyers[i].buyerID){
            found = 1;
            buyers[i].energyInKWH += energyInKWH;
            buyers[i].revenueGenerated += energyInKWH*price;
        }
    }
    if(!found){
        buyers[buyerCount].buyerID = buyerID;
        buyers[buyerCount].energyInKWH = energyInKWH;
        buyers[buyerCount].revenueGenerated = energyInKWH*price;
        buyerCount++;
    }
}


void inertUpdateSellers(int sellerID,double energyInKWH,double price){
    int found = 0;
    for(int i=0;i<sellerCount;i++){
        if(sellerID == sellers[i].sellerID){
            found = 1;
            sellers[i].energyInKWH += energyInKWH;
            sellers[i].revenueGenerated += energyInKWH*price;
        }
    }
    if(!found){
        sellers[sellerCount].sellerID = sellerID;
        sellers[sellerCount].energyInKWH = energyInKWH;
        sellers[sellerCount].revenueGenerated = energyInKWH*price;
        sellerCount++;
    }
}

void addTransaction(int transactionID, int buyerID, int sellerID, double energy, double price, const char* timeStamp) {
    if (transactionCount < MAX_TRANSACTIONS) {
        if (isTransactionIDUnique(transactionID)) {
            transactions[transactionCount].transactionID = transactionID;
            transactions[transactionCount].buyerID = buyerID;
            transactions[transactionCount].sellerID = sellerID;
            transactions[transactionCount].energyInKWH = energy;
            transactions[transactionCount].pricePerKwh = price;
            strcpy(transactions[transactionCount].timeStamp, timeStamp);
            inertUpdateBuyers(buyerID,energy,price);
            inertUpdateSellers(sellerID,energy,price);
            inertUpdateBuyerSeller(buyerID,sellerID,energy,price);
            transactionCount++;
            printf("Transaction was added\n");
        } else {
            printf("Transaction ID %d is not unique. Transaction was not added.\n", transactionID);
        }

    } else {
        printf("Transaction limit reached. Transaction was not added.\n");
    }
}
// -------------------------------------------------------------------------------------------

//  2. Display All Transactions: Users should be able to view all the transactions stored in the system.
// Each transaction should be clearly displayed, including all its attributes.


void displayAllTransactions(){
    if(transactionCount==0){
        printf("Transaction Data Empty");
    }
    else{
        printf("Displaying all the transactions: \n");
        for(int i=0;i<transactionCount;i++){
            printf("Transaction ID: %d, Buyer ID: %d, Seller ID: %d, Energy: %.2f kWh, Price: $%.2f/kWh, Timestamp: %s\n",
               transactions[i].transactionID, transactions[i].buyerID, transactions[i].sellerID,
               transactions[i].energyInKWH, transactions[i].pricePerKwh, transactions[i].timeStamp);
        }
    }
}

// -------------------------------------------------------------------------------------------
// 3. Create a list of transactions for every seller.

void listOfSellers(struct Transaction transactions[]){
    for(int j = 0 ; j< sellerCount ; j++){
        int seller_transaction_count = 0;
        sellerList[j].transaction_count = 0 ;
        for(int i=0;i<transactionCount;i++){
            if(sellers[j].sellerID == transactions[i].sellerID){
                sellerList[j].sellerID = sellers[j].sellerID;
                sellerList[j].transaction_count ++;
                sellerList[j].seller_transactions[seller_transaction_count].sellerID = transactions[i].sellerID;
                sellerList[j].seller_transactions[seller_transaction_count].transactionID = transactions[i].transactionID;
                sellerList[j].seller_transactions[seller_transaction_count].buyerID = transactions[i].buyerID;
                sellerList[j].seller_transactions[seller_transaction_count].energyInKWH = transactions[i].energyInKWH;
                sellerList[j].seller_transactions[seller_transaction_count].pricePerKwh = transactions[i].pricePerKwh;
                seller_transaction_count++;
            }
        }
    }
    for(int i = 0 ; i<sellerCount ; i++){
        printf("sellerID : %d \n", sellerList[i].sellerID);
        int j = 0;
        while(j< sellerList[i].transaction_count){
            printf("Transaction ID: %d, Buyer ID: %d,  Energy: %.2f kWh, Price: $%.2f/kWh, \n",
               sellerList[i].seller_transactions[j].transactionID, sellerList[i].seller_transactions[j].buyerID, 
               sellerList[i].seller_transactions[j].energyInKWH, sellerList[i].seller_transactions[j].pricePerKwh);
               j++;
        }
    }
}


// -------------------------------------------------------------------------------------------
// 4. Create a list of transactions for every buyer.

void listOfBuyers(struct Transaction transactions[]){
    for(int j = 0 ; j< buyerCount ; j++){
        int buyer_transaction_count = 0;
        buyerList[j].transaction_count = 0 ;
        for(int i=0;i<transactionCount;i++){
            if(buyers[j].buyerID == transactions[i].buyerID){
                buyerList[j].buyerID = buyers[j].buyerID;
                buyerList[j].transaction_count ++;
                buyerList[j].buyer_transactions[buyer_transaction_count].buyerID = transactions[i].buyerID;
                buyerList[j].buyer_transactions[buyer_transaction_count].transactionID = transactions[i].transactionID;
                buyerList[j].buyer_transactions[buyer_transaction_count].sellerID = transactions[i].sellerID;
                buyerList[j].buyer_transactions[buyer_transaction_count].energyInKWH = transactions[i].energyInKWH;
                buyerList[j].buyer_transactions[buyer_transaction_count].pricePerKwh = transactions[i].pricePerKwh;
                buyer_transaction_count++;
            }
        }
    }
    for(int i = 0 ; i<buyerCount ; i++){
        printf("buyerID : %d \n", buyerList[i].buyerID);
        int j = 0;
        while(j< buyerList[i].transaction_count){
            printf("Transaction ID: %d, Seller ID: %d,  Energy: %.2f kWh, Price: $%.2f/kWh, \n",
               buyerList[i].buyer_transactions[j].transactionID, buyerList[i].buyer_transactions[j].sellerID, 
               buyerList[i].buyer_transactions[j].energyInKWH, buyerList[i].buyer_transactions[j].pricePerKwh);
               j++;
        }
    }
}

// -------------------------------------------------------------------------------------------
// 5. Given a time period, list all transactions that took place in that period.

int isWithinPeriod(const char* timestamp, const char* start, const char* end) {
    return strcmp(timestamp, start) >= 0 && strcmp(timestamp, end) <= 0;
}

void listTransactionsInPeriod(const char* start, const char* end) {
    int found = 0;
    printf("Transactions between %s and %s:\n", start, end);
    for (int i = 0; i < transactionCount; i++) {
        if (isWithinPeriod(transactions[i].timeStamp, start, end)) {
            found = 1;
            printf("Transaction ID: %d, Buyer ID: %d, Seller ID: %d, Energy: %.2f kWh, Price: $%.2f/kWh, Timestamp: %s\n",
                   transactions[i].transactionID, transactions[i].buyerID,
                   transactions[i].sellerID, transactions[i].energyInKWH,
                   transactions[i].pricePerKwh, transactions[i].timeStamp);
        }
    }
    if (!found) {
        printf("No transactions found in the specified period.\n");
    }
}

// -------------------------------------------------------------------------------------------
//  6. Find the month in which maximum transactions took place.

void monthWithMaxTransactions() {
    int monthCount[12] = {0};
    int maxTransactions = 0, maxMonth = -1;

    
    for (int i = 0; i < transactionCount; i++) {

        int year = 0, month = 0;

        for (int j = 0; j < 4; j++) {
            year = year * 10 + (transactions[i].timeStamp[j] - '0');
        }

        for (int j = 5; j < 7; j++) {
            month = month * 10 + (transactions[i].timeStamp[j] - '0');
        }

        monthCount[month - 1]++;
        if (monthCount[month - 1] > maxTransactions) {
            maxTransactions = monthCount[month - 1];
            maxMonth = month;
        }
    }

    if (maxMonth != -1) {
        printf("Month with maximum transactions: %02d (Total transactions: %d)\n", maxMonth, maxTransactions);

        printf("Transactions for month %02d:\n", maxMonth);
        for (int i = 0; i < transactionCount; i++) {
            int year = 0, month = 0;

            for (int j = 0; j < 4; j++) {
                year = year * 10 + (transactions[i].timeStamp[j] - '0');
            }

            for (int j = 5; j < 7; j++) {
                month = month * 10 + (transactions[i].timeStamp[j] - '0');
            }

            if (month == maxMonth) {
                printf("Transaction ID: %d, Timestamp: %s, Energy(KWH): %.2f , Price per KWH: %.2f\n", 
                       transactions[i].transactionID, 
                       transactions[i].timeStamp, 
                       transactions[i].energyInKWH,
                       transactions[i].pricePerKwh);
            }
        }
    } else {
        printf("No transactions available.\n");
    }
}



// -------------------------------------------------------------------------------------------
// 7. Calculate Total Revenue by Seller: The program should allow users to query the total revenue
//  generated by a specific seller based on their Seller ID. Revenue should be calculated as the sum of
//  the products of energy amounts and prices per kWh for all transactions involving that seller.


void totalRevenueBySeller(int sellerID) {
    float totalRevenue = 0.0;

    printf("Transactions for Seller %d:\n", sellerID);
    bool hasTransactions = false;

    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].sellerID == sellerID) {
            float transactionRevenue = transactions[i].energyInKWH * transactions[i].pricePerKwh;
            totalRevenue += transactionRevenue;

            printf("  TransactionID: %d, BuyerID: %d, Energy: %.2f kWh, Price: %.2f\n",
                   transactions[i].transactionID,
                   transactions[i].buyerID,
                   transactions[i].energyInKWH,
                   transactions[i].pricePerKwh);
            hasTransactions = true; 
        }
    }

    if (hasTransactions) {
        printf("Total Revenue for Seller %d: $%.2f\n", sellerID, totalRevenue);
    } else {
        printf("No transactions found for Seller %d.\n", sellerID);
    }
}



// -------------------------------------------------------------------------------------------

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapDouble(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}
// -------------------------------------------------------------------------------------------

int partition(struct Buyer buyers[], int low, int high) {
    double pivot = buyers[high].energyInKWH;  
    int i = (low - 1);  

    for (int j = low; j < high; j++) {

        if (buyers[j].energyInKWH <= pivot) {
            i++;
            swap(&buyers[i].buyerID, &buyers[j].buyerID);
            swapDouble(&buyers[i].energyInKWH, &buyers[j].energyInKWH);
            swapDouble(&buyers[i].revenueGenerated, &buyers[j].revenueGenerated);
        }
    }
    swap(&buyers[i + 1].buyerID, &buyers[high].buyerID);
    swapDouble(&buyers[i + 1].energyInKWH, &buyers[high].energyInKWH);
    swapDouble(&buyers[i + 1].revenueGenerated, &buyers[high].revenueGenerated);
    return (i + 1);
}

void quickSort(struct Buyer arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
// -------------------------------------------------------------------------------------------

// 8. Sorting Sellers by revenue

int partition_seller(struct Seller sellers[], int low, int high) {
    double pivot = sellers[high].revenueGenerated;
    int i = (low - 1); 

    for (int j = low; j < high; j++) {
        if (sellers[j].revenueGenerated <= pivot) {
            i++; 
            swap(&sellers[i].sellerID, &sellers[j].sellerID);
            swapDouble(&sellers[i].energyInKWH, &sellers[j].energyInKWH);
            swapDouble(&sellers[i].revenueGenerated, &sellers[j].revenueGenerated);
        }
    }
    swap(&sellers[i + 1].sellerID, &sellers[high].sellerID);
    swapDouble(&sellers[i + 1].energyInKWH, &sellers[high].energyInKWH);
    swapDouble(&sellers[i + 1].revenueGenerated, &sellers[high].revenueGenerated);
    return (i + 1);
}

void quickSort_seller(struct Seller arr[], int low, int high) {
    if (low < high) {
        // call partition function to find Partition Index
        int pi = partition_seller(arr, low, high);
        quickSort_seller(arr, low, pi - 1);
        quickSort_seller(arr, pi + 1, high);
    }
}

void SortSellerByRevenue(struct Seller sellers[]) {
    printf("\nBefore Sorting sellers by Energy:\n");
    for (int i = 0; i < sellerCount; i++) {
        printf("sellerID: %d, Energy: %.2f kWh, Price: %.2f\n", sellers[i].sellerID, sellers[i].energyInKWH, sellers[i].revenueGenerated);
    }
    quickSort_seller(sellers, 0, sellerCount - 1);
    printf("\nAfter Sorting sellers by Energy (Ascending):\n");
    for (int i = 0; i < sellerCount; i++) {
        printf("sellerID: %d, Energy: %.2f kWh, Price: %.2f\n", sellers[i].sellerID, sellers[i].energyInKWH, sellers[i].revenueGenerated);
    }
}

// -------------------------------------------------------------------------------------------
// 9. Find and Display the Transaction with the Highest Energy Amount: The program should allow
// users to search for and display the transaction with the highest amount of energy traded (kWh). This
// will help identify the largest trading events. Sort the transactions based on amount of energy traded


void SortTransactionsByEnergy(struct Transaction transactions[]) {
    for (int i = 0; i < transactionCount - 1; i++) {
        for (int j = 0; j < transactionCount - i - 1; j++) {
            if (transactions[j].energyInKWH < transactions[j + 1].energyInKWH) {
                swap(&transactions[j].sellerID, &transactions[j + 1].sellerID);
                swap(&transactions[j].buyerID, &transactions[j + 1].buyerID);
                swap(&transactions[j].transactionID, &transactions[j + 1].transactionID);
                swapDouble(&transactions[j].energyInKWH, &transactions[j + 1].energyInKWH);
                swapDouble(&transactions[j].pricePerKwh, &transactions[j + 1].pricePerKwh);
            }
        }
    }
    printf("sorted data \n");
    for(int i = 0 ; i<transactionCount ; i++){
        printf("SellerID: %d, Energy: %.2f kWh, Price: %.2f\n", transactions[i].sellerID, transactions[i].energyInKWH, transactions[i].pricePerKwh);
    }

    printf("\nTransaction with the Highest Energy Amount:\n");
    printf("transactionID: %d , SellerID: %d,BuyerID: %d, Energy: %.2f kWh, Price: %.2f\n",transactions[0].transactionID, transactions[0].sellerID,transactions[0].buyerID, transactions[0].energyInKWH, transactions[0].pricePerKwh);
}


// -------------------------------------------------------------------------------------------
// 10. Sort the list of buyers based on the energy bought.
void SortBuyerByEnergy(struct Buyer buyers[]) {
    printf("\nBefore Sorting Buyers by Energy:\n");
    for (int i = 0; i < buyerCount; i++) {
        printf("BuyerID: %d, Energy: %.2f kWh, Price: %.2f\n", buyers[i].buyerID, buyers[i].energyInKWH, buyers[i].revenueGenerated);
    }

    quickSort(buyers, 0, buyerCount - 1);

    printf("\nAfter Sorting Buyers by Energy (Ascending):\n");
    for (int i = 0; i < buyerCount; i++) {
        printf("BuyerID: %d, Energy: %.2f kWh, Price: %.2f\n", buyers[i].buyerID, buyers[i].energyInKWH, buyers[i].revenueGenerated);
    }
}

// -------------------------------------------------------------------------------------------
// 11. Find out a pair of seller/buyer involved in maximum number of transactions.
void findMaxBsCount(struct BuyerSellerPair pairs[]){
    int max_pair = pairs[0].BSCount; 
    int bs_index;
    for(int i = 0 ; i<pairCount ; i++){
        if(pairs[i].BSCount>max_pair){
            max_pair = pairs[i].BSCount;
            bs_index = i;
        }
    }
    printf("\nBuyer seller pair with maximum transactions :\n");
    printf("BuyerID: %d, SellerID: %d ,  Energy: %.2f kWh, Revenue: %.2f\n , No of transactions: %d \n", pairs[bs_index].buyerID,pairs[bs_index].sellerID, pairs[bs_index].energyInKWH, pairs[bs_index].revenueGenerated , pairs[bs_index].BSCount );
    
}

// -------------------------------------------------------------------------------------------
// 12. Sort all seller/buyer pairs in decreasing order of the number of transactions between them.
// modified bubble sort used
void SortPairByCount(struct BuyerSellerPair pairs[]) {
    int swapped;int sorted=0;int i=0;
    while(i<pairCount && sorted == 0){
        swapped = 0;
        for (int j = 0; j < pairCount - i - 1; j++) {
            if (pairs[j].BSCount < pairs[j + 1].BSCount) {
                swap(&pairs[j].sellerID, &pairs[j + 1].sellerID);
                swap(&pairs[j].buyerID, &pairs[j + 1].buyerID);
                swapDouble(&pairs[j].energyInKWH, &pairs[j + 1].energyInKWH);
                swap(&pairs[j].BSCount, &pairs[j + 1].BSCount);
                swapDouble(&pairs[j].revenueGenerated, &pairs[j + 1].revenueGenerated);
                swapped = 1;
            }
        }
        if(swapped==0) sorted = 1;
    }

    printf("sorted data \n");
    for(int i = 0 ; i<pairCount ; i++){
        printf("BuyerID: %d, SellerID: %d ,  Energy: %.2f kWh, Revenue: %.2f , No of transactions: %d \n", pairs[i].buyerID,pairs[i].sellerID, pairs[i].energyInKWH, pairs[i].revenueGenerated , pairs[i].BSCount );
    }
    
}

// -------------------------------------------------------------------------------------------
// 13.  Sort all seller/buyer pairs in decreasing order of total revenue exchanged between them.
// modified bubble sort used
void SortPairByRevenue(struct BuyerSellerPair pairs[]) {
    int swapped;int sorted=0;int i=0;
    while(i<pairCount && sorted==0){
        swapped = 0;
        for (int j = 0; j < pairCount - i - 1; j++) {
            if (pairs[j].revenueGenerated < pairs[j + 1].revenueGenerated) {
                swap(&pairs[j].sellerID, &pairs[j + 1].sellerID);
                swap(&pairs[j].buyerID, &pairs[j + 1].buyerID);
                swapDouble(&pairs[j].energyInKWH, &pairs[j + 1].energyInKWH);
                swap(&pairs[j].BSCount, &pairs[j + 1].BSCount);
                swapDouble(&pairs[j].revenueGenerated, &pairs[j + 1].revenueGenerated);
                swapped = 1;
                i++;
            }
        }
        if (swapped == 0) {
            sorted = 1;
        }
    }
    printf("sorted data \n");
    for(int i = 0 ; i<pairCount ; i++){
        printf("BuyerID: %d, SellerID: %d ,  Energy: %.2f kWh, Revenue: %.2f , No of transactions: %d \n", pairs[i].buyerID,pairs[i].sellerID, pairs[i].energyInKWH, pairs[i].revenueGenerated , pairs[i].BSCount );
    }
    
}

// -------------------------------------------------------------------------------------------


int main(){
    


    int choice;
    int flag = 1;
    while(flag){
        read_pairs_from_file(pairs, MAX_TRANSACTIONS);
        read_transactions_from_file(transactions, MAX_TRANSACTIONS);
        read_buyers_from_file(buyers, MAX_TRANSACTIONS);
        read_sellers_from_file(sellers, MAX_TRANSACTIONS);
        printf("\n----------------------------------------------------------------------\n");
        printf("\nEnergy Trading Record Management System\n");
        printf("1. Add New Transaction\n");
        printf("2. Display All Transactions\n");
        printf("3. List Transactions by Seller\n");
        printf("4. List Transactions by Buyer\n");
        printf("5. List Transactions in a Given Time Period\n");
        printf("6. Find the month with maximum transactions\n");
        printf("7. Calculate Total Revenue by Seller\n");
        printf("8. Sort the list of sellers based on their revenue generated.\n");
        printf("9. Find and Display the Transaction with the Highest Energy Amount\n");
        printf("10. Sort the list of buyers based on the energy bought\n");
        printf("11. Find out a pair of seller/buyer involved in maximum number of transactions\n");
        printf("12. Sort all seller/buyer pairs in decreasing order of the number of transactions between them\n");
        printf("13.  Sort all seller/buyer pairs in decreasing order of total revenue exchanged between them\n");
        printf("14. Exit\n");
        printf("\n----------------------------------------------------------------------\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);

        if (choice == 14) {
            flag=0;
        }

        int tid, bid, sid;
        double energy, price;
        char timestamp[20],start[20],end[20];

        switch (choice) {
            case 1:
                printf("Enter Transaction ID: ");
                scanf("%d", &tid);
                printf("Enter Buyer ID: ");
                scanf("%d", &bid);
                printf("Enter Seller ID: ");
                scanf("%d", &sid);
                printf("Enter Energy Amount (kWh): ");
                scanf("%lf", &energy);
                printf("Enter Price per kWh: ");
                scanf("%lf", &price);
                printf("Enter Timestamp (YYYY-MM-DD HH:MM): ");
                scanf("%s", timestamp);
                addTransaction(tid, bid, sid, energy, price, timestamp);
                break;

            case 2:
                displayAllTransactions();
                break;

            case 3:
                listOfSellers(transactions);
                break;

            case 4:

                listOfBuyers(transactions);
                break;

            case 5:
                printf("Enter Start Date (YYYY-MM-DD): ");
                scanf("%s", start);
                printf("Enter End Date (YYYY-MM-DD): ");
                scanf("%s", end);
                listTransactionsInPeriod(start, end);
                
                break;

            case 6:
                monthWithMaxTransactions();
                break;

            case 7:
                printf("Enter Seller ID: ");
                scanf("%d", &sid);
                totalRevenueBySeller(sid);
                break;
            case 8:
                printf("Sorted list of sellers based on based on revenue generated:\n");
                SortSellerByRevenue(sellers);

                break;
            case 9:
                
                SortTransactionsByEnergy(transactions);
                break;
            case 10:
                SortBuyerByEnergy(buyers);
                break;
            case 11:
                findMaxBsCount(pairs);
                break;
            case 12:
                
                SortPairByCount(pairs);
                break;
            case 13:
                SortPairByRevenue(pairs);
                break;
            case 14:
                printf("Thankyou!");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }
        write_pairs_to_file(pairs, pairCount);
        write_transactions_to_file(transactions, transactionCount);
        write_buyers_to_file(buyers , buyerCount);
        write_sellers_to_file(sellers, sellerCount);
    }
    
    return 0;
}