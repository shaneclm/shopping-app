/******************************************************************************************************************************************
    This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts learned.
    I have constructed the functions and their respective algorithms and corresponding code by myself. The program was run, tested,
    and debugged by my own efforts. I further certify that I have not copied in part or whole or otherwise plagiarized the work of 
    other students and/or persons.
                                                                                         Shania Francine T. Cloma, DLSU ID #12274623
******************************************************************************************************************************************/

                                   /* To Compile: gcc -Wall "Shopping App.c" -o MachineProj.exe */

/***************************************************************************************************************************************** 
    shopping program that facilitates buying and selling of items
    keep track of the set of 100 users, the set of at most 20 items per seller, and the set of transactions

    USERS can be buyers and/or sellers. Each user has:
          - string (at most 20 characters) for the name    
          - numeric userID
          - string (at most 10 characters) for the password
          - string (at most 30 characters) for the address
          - a numeric contact number          


    ITEMS each item has:
          - numeric productID
          - string (at most 20 characters) for the item name
          - string (at most 15 characters) for the category
          - string (at most 30 characters) for the item description
          - numeric quantity available
          - numeric unit price
          - numeric seller's userID (which corresponds to the seller's userID)

 
    TRANSACTIONS each transaction has:
                 - date (that further contains the numeric month, day, and year)
                 - the set (at most 5) items
                 - the buyer's user ID
                 - the seller's user ID
                 - the total amount of the transaction
    NOTE: though there may be at most 5 items, these may be of different quantity
          per item and all of these items in the transaction should be from the 
          same seller. The buyer should also not be the seller.

*****************************************************************************************************************************************/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_USERS 100           // Maximum number of users in the app
#define MAX_ITEMS 20            // Maximum number of items a seller can sell
#define MAX_ITEMS_ALL 2000      // Maximum number of items in the system
#define MAX_TRANSACTIONS 2000

typedef struct user
{
    char            name[21];
    int             userID;
    char            password[11];
    char            address[31];
    long long int   contactNum;;
}userType;

typedef struct Date
{
    char day[3];
    char month[3];
    char year[5];
} dateType;

typedef struct Item 
{
    int    productID;
    char   itemName[21];
    char   category[16];
    char   description[31];
    int    quantityAvailable;
    float  unitPrice;
    int    sellerID;
} itemType;

typedef struct CartItem
{
    int     productID;
    char    itemName[21];
    char    category[16];
    float   unitPrice;
    int     quantity;
    int     sellerID;
} CartItemType;

typedef struct Transaction 
{
    dateType    date;
    itemType    itemsTransac[5];
    int         buyerID;
    int         sellerID;
    float       totalAmount;
    int         numItems;           // new variable to count the number of unique items in the transaction
} transactionType;

typedef struct 
{
    int buyerID;
    char buyerName[21];
    float totalAmount;
} BuyerTotal;


void    loadData(userType Users[], int *NumUsers, itemType Items[], int *NumItems);
int     printMainMenu();
int     checkUserID(int userID, userType Users[], int numUsers);            
void    registerUser(userType Users[], int *numUsers);
int     login(userType Users[], int numUsers);
void    adminMenu(itemType Items[], int NumItems, userType Users[], int NumUsers);
void    printUsersTable(userType Users[], int NumUsers);
void    showAllSellers(itemType Items[], int NumItems, userType Users[], int NumUsers);
void    showTotalSalesInGivenDuration();
float   calculateTotalSales(const char* start_date, const char* end_date);
void    showSellersSales(itemType Items[], int NumItems, userType Users[], int NumUsers);
void    showShopaholics(itemType Items[], int NumItems, userType Users[], int NumUsers);
void    selectUserMenu(itemType Items[], int *NumItems, int currentUserID, CartItemType cartItems[], transactionType Transacs[], userType Users[], int NumUsers);
void    loadCartData(int userID, CartItemType cartItems[], int *NumItemsInCart);
void    countTransactions(int *NumTransacs);
void    selectSellMenu(itemType Items[], int *NumItems, int sellerID);
void    addNewItem(itemType Items[], int *NumItems, int sellerID);
void    selectEditStock(itemType Items[], int *NumItems, int sellerID);
void    showMyProducts(itemType Items[], int *NumItems, int sellerID);
void    showLowStockProducts(itemType Items[], int *NumItems, int sellerID);
void    selectBuyMenu(itemType Items[], int *NumItemsInCart, int *NumItems, int userID, CartItemType cartItems[], transactionType Transacs[], userType Users[], int NumUsers);
void    viewAllProducts(itemType Items[], int NumItems);
void    showProductsBySeller(itemType Items[], int NumItems);
void    searchProductsByCategory(itemType Items[], int NumItems);
void    searchProductsByName(itemType Items[], int NumItems);
void    addToCart(itemType Items[], int *NumItemsInCart, int *NumItems, int userID, CartItemType cartItems[]);
void    editCart(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems);
void    removeItemsFromSeller(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems);
void    removeSpecificItem(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems);
void    editQuantity(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems);
void    checkOutMenu(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems, transactionType Transacs[], int userID, userType Users[], int NumUsers);
void    checkOutAll(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems, transactionType Transacs[], char day[], char month[], char year[], int userID, userType Users[], int NumUsers, int *NumTransacs);
char*   getSellerName(int sellerID, itemType items[], int numItems, userType users[], int numUsers);
char*   getBuyerName(int buyerID, userType Users[], int NumUsers);
void    checkOutSpecificSeller(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems, transactionType Transacs[], char day[], char month[], char year[], int userID, userType Users[], int NumUsers, int *NumTransacs);
void    checkOutSpecificItem(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems, transactionType Transacs[], char day[], char month[], char year[], int userID, userType Users[], int NumUsers, int *NumTransacs);
void    saveCartData(int userID, CartItemType cartItems[], int NumItemsInCart);
void    saveData(userType* Users, int NumUsers, itemType* Items, int NumItems);


int main(){
    userType    Users[MAX_USERS];                  // Array to store user data
    int         NumUsers = 0;                      // Number of registered users
    int         option;                            // User's menu option

    itemType    Items[MAX_ITEMS_ALL];              // Array to store item data
    int         NumItems = 0;                      // Number of available items

    transactionType Transacs[MAX_ITEMS_ALL];       // Array to store transaction data
    CartItemType cartItems[MAX_USERS];             // Array to store cart items for each user

    int         currentUserIndex;                  // Index of the currently logged-in user

    // Load Users.txt & Items.txt
    loadData(Users, &NumUsers, Items, &NumItems);

    // Main Menu
    do {
        currentUserIndex = -1;        
        option = printMainMenu();

        switch (option) {
            case 1:
                registerUser(Users, &NumUsers);
                break;

            case 2:
                currentUserIndex = login(Users, NumUsers);
                if (currentUserIndex >= 0) {
                    selectUserMenu(Items, &NumItems, Users[currentUserIndex].userID, cartItems, Transacs, Users, NumUsers);
                } else if (currentUserIndex == -1) {
                    printf("\t\tUser ID not found.\n");                                                           
                } else if (currentUserIndex == -2) {
                    printf("\t\tInvalid password.\n");                                                                   
                }
                break;

            case 3:
                adminMenu(Items, NumItems, Users, NumUsers);
                break;

            case 4:
                saveData(Users, NumUsers, Items, NumItems);
                break;

            default:
                printf("\n\t\tInvalid option. Please select a valid option.\n");
        }
    } while (option != 4);

    return 0;
}

/***********************************************************************************************************
    loadData function reads data from Users.txt and Items.txt files and loads them into respective arrays.
    It also updates the number of users and number of items accordingly.

    @param  Users - an array of userType struct to store user data
    @param  NumUsers - a pointer to an integer to store the number of users
    @param  Items - an array of itemType struct to store item data
    @param  NumItems - a pointer to an integer to store the number of items
    @return void

    Pre-condition: Users and Items arrays are properly defined and initialized with appropriate sizes
************************************************************************************************************/
void 
loadData (userType Users[], int *NumUsers, itemType Items[], int *NumItems) 
{
    int numRead, numRead2;

    // Load Users.txt
    FILE *USERStxt = fopen("Users.txt", "r");
    if (USERStxt != NULL) {
        while (fscanf(USERStxt, "%d %s\n%[^\n]\n%[^\n]\n%lld\n\n", &Users[*NumUsers].userID,
               Users[*NumUsers].password, Users[*NumUsers].name, Users[*NumUsers].address, &Users[*NumUsers].contactNum) == 5) {
            (*NumUsers)++;
        }
        fclose(USERStxt);
    }

    // Load Items.txt
    FILE* fp = fopen("Items.txt", "r");
    if (fp != NULL){

        while (*NumItems < MAX_ITEMS_ALL && !feof(fp)){
            numRead = fscanf(fp, "%d %d\n", &Items[*NumItems].productID, &Items[*NumItems].sellerID);

            if (numRead == 2){
                fgets(Items[*NumItems].itemName, 21, fp);
                Items[*NumItems].itemName[strcspn(Items[*NumItems].itemName, "\n")] = '\0';
                fgets(Items[*NumItems].category, 16, fp);
                Items[*NumItems].category[strcspn(Items[*NumItems].category, "\n")] = '\0';
                fgets(Items[*NumItems].description, 31, fp);
                Items[*NumItems].description[strcspn(Items[*NumItems].description, "\n")] = '\0';
                numRead2 = fscanf(fp, "%d %f\n", &Items[*NumItems].quantityAvailable, &Items[*NumItems].unitPrice);

                if (numRead2 == 2){
                    fscanf(fp, "\n");
                    (*NumItems)++;
                }
            }
        }
    }
    
    fclose(fp);

}

/***********************************************************************************************************
    countTransactions - counts the number of transactions in the Transactions.txt file

    @param  NumTransacs - pointer to the variable where the number of transactions will be stored
    @return void

    Pre-condition: Transactions.txt file exists and contains lines starting with "Transaction No. %d"
************************************************************************************************************/
void 
countTransactions(int *NumTransacs)
{
    FILE *file = fopen("Transactions.txt", "r");
    char line[500];
    int  transactionCount = 0;

    while (fgets(line, sizeof(line), file)){
        // Read each line from the file
        if (strncmp(line, "Transaction No. ", 16) == 0){
            // If line starts with "Transaction No. ", increment transaction count
            transactionCount++;
        }
    }

    fclose(file);
    *NumTransacs = transactionCount;
}

/***********************************************************************************************************
    registerUser - Registers a new user and adds them to the array of users

   @param  Users - Array of userType representing existing users & where the new user will be stored
   @param  numUsers - Pointer to an integer representing the number of existing users
   @return void

   Pre-condition: Users is a valid array of userType, numUsers points to a valid integer. This also
                  assumes that the user will input valid datatypes for user information being asked
************************************************************************************************************/
void 
registerUser(userType Users[], int *numUsers)
{
    int      userID;
    userType newUser;

    printf("                _________________________________________________________\n");
    printf("\n\t\tCreate User ID: ");
    scanf("%d", &userID);

    // Check if user ID already exists
    while (checkUserID(userID, Users, *numUsers)){
        printf("\t\tThis User ID already exists.\n\t\tEnter a different User ID: ");
        scanf("%d", &userID);
    }

    newUser.userID = userID;

    printf("\n\t\tEnter Name: ");
    scanf(" %[^\n]", newUser.name);

    printf("\t\tEnter Password: ");
    scanf("%s", newUser.password);

    printf("\t\tEnter Address: ");
    scanf(" %[^\n]", newUser.address);

    printf("\t\tEnter Contact Number: ");
    scanf("%lld", &newUser.contactNum);
    printf("\n");

    // Adds the new user to array of Users
    Users[*numUsers] = newUser;
    (*numUsers)++;

    printf("\t\tThank you %s. You have registered successfully!\n", strtok(newUser.name, " "));
    printf("\n               _________________________________________________________\n");
}

/***********************************************************************************************************
    login is used to authenticate a user by checking the user ID and password

    @param  Users - an array of userType representing the registered users
    @param  numUsers - an integer representing the number of registered users
    @return  i - index of the authenticated user in the Users array if login is successful,
            -2 - if user ID is correct but password is incorrect,
            -1 - if user ID is not found

    Pre-condition: userType struct has valid values for each user, including unique user IDs & 
                   numUsers accurately reflects the number of registered users in Users array
                   This also assumes that the user will input valid UserID and password
************************************************************************************************************/
int 
login(userType Users[], int numUsers)
{
    int  i;
    int  userID;
    char password[11];

    printf("                _________________________________________________________\n");
    printf("\n\t\tEnter User ID: ");
    scanf("%d", &userID);

    for (i = 0; i < numUsers; i++){
        if (Users[i].userID == userID){
            printf("\t\tEnter Password: ");
            scanf("%s", password);
            if (strcmp(Users[i].password, password) == 0){
                printf("\n\t\tLogin successful! Welcome back, %s.\n\n", Users[i].name);
                return i;
            }
            else
                return -2; 
        }
    }
    return -1; 
}

/***********************************************************************************************************
    checkUserID checks if the given userID exists in the array of registered users

    @param  userID - Integer representing the user ID to be checked
    @param  Users - Array of userType representing registered users
    @param  numUsers - Number of registered users
    @return 1 if userID exists in the Users array, 0 if userID does not exist

    Pre-condition: The Users array must contain valid userType struct with unique user IDs, and 
                   numUsers must be a valid integer that is the number of registered users in the 
                   array. userID parameter must be a valid integer that is to be checked. 
************************************************************************************************************/
int 
checkUserID(int userID, userType Users[], int numUsers)
{
    int i;

    for (i = 0; i < numUsers; i++){
        if (Users[i].userID == userID)
            return 1;
    }
    return 0;
}

/***********************************************************************************************************
    adminMenu is a function that displays the admin menu and performs actions based on user input

    @param  Items - an array of itemType representing items
    @param  NumItems - an integer representing the number of items in Items array
    @param  Users - an array of userType representing users
    @param  NumUsers - an integer representing the number of users in Users array
    @return void

    Pre-condition: Items and Users arrays are properly initialized and contain valid data, &
                   NumItems and NumUsers are valid integers
************************************************************************************************************/
void 
adminMenu(itemType Items[], int NumItems, userType Users[], int NumUsers)
{
    int  adminChoice;
    char password[10];

    printf("\t\tPlease enter the password for verification: ");
    scanf("%s", password);

    if (strcmp(password, "H3LLo?") == 0){
        printf("\n\t\tAccess granted.\n\n");

        do {
            printf("\n");
            printf("\t\t    ____________________________________________________\n");
            printf("\t\t    |                   ~ Admin Menu ~                 |\n");
            printf("\t\t    |                                                  |\n");
            printf("\t\t    |    [1] Show All Users                            |\n");
            printf("\t\t    |    [2] Show All Sellers                          |\n");
            printf("\t\t    |    [3] Show Total Sales in Given Duration        |\n");
            printf("\t\t    |    [4] Show Sellers Sales                        |\n");
            printf("\t\t    |    [5] Show Shopaholics                          |\n");
            printf("\t\t    |    [6] Back to Main Menu                         |\n");
            printf("\t\t    |__________________________________________________|\n");
            printf("\n\t\t     Select option: ");
            scanf("%d", &adminChoice);

            switch (adminChoice) 
            {
                case 1:
                    printUsersTable(Users, NumUsers);
                    break;

                case 2:
                    showAllSellers(Items, NumItems, Users, NumUsers);
                    break;

                case 3:
                    showTotalSalesInGivenDuration(Items, NumItems, Users, NumUsers);
                    break;

                case 4:
                    showSellersSales(Items, NumItems, Users, NumUsers);
                    break;

                case 5:
                    showShopaholics(Items, NumItems, Users, NumUsers);
                    break;

                case 6:
                    printf("\n\t\t     Returning to Main Menu...\n");
                    break;

                default:
                    printf("\n\t\t     Invalid option. Please select a valid option.\n");
                    break;
            }
        } while (adminChoice != 6);

    } else {
        printf("\n\t\tUnauthorized access not allowed.\n");
    }
}

/***********************************************************************************************************
    printUsersTable displays the sorted list of users in a formatted table

    @param  Users - an array of userType objects representing the users to be displayed
    @param  NumUsers - an integer representing the number of users in the Users array
    @return void

    Pre-condition: Users array is properly initialized & NumUsers are valid integers
************************************************************************************************************/
void 
printUsersTable(userType Users[], int NumUsers)
{
    int i, j;

    for (i = 0; i < NumUsers - 1; i++) {
        for (j = 0; j < NumUsers - i - 1; j++) {
            if (Users[j].userID > Users[j + 1].userID) {
                userType temp = Users[j];
                Users[j] = Users[j + 1];
                Users[j + 1] = temp;
            }
        }
    }

    printf("\n\n");
    printf("\t_________________________________________________________________________________________________________\n\n");
    printf("\t Displaying all users in the system:\n");
    printf("\t_________________________________________________________________________________________________________\n");
    printf("\t| User ID    | Password   | Name                 | Address                        | Phone Number        |\n");
    printf("\t|____________|____________|______________________|________________________________|_____________________|\n");
    for (i = 0; i < NumUsers; i++) {
        printf("\t| %-10d | %-10s | %-20s | %-30s | %-20lld|\n", Users[i].userID, Users[i].password, Users[i].name, Users[i].address, Users[i].contactNum);
    }
    printf("\t|____________|____________|______________________|________________________________|_____________________|\n");
    printf("\n");
    printf("\t_________________________________________________________________________________________________________\n");
}


/***********************************************************************************************************
    showAllSellers - displays all sellers with their information and number of items for sale

    @param  Items - array of itemType representing items for sale
    @param  NumItems - number of items in the Items array
    @param  Users - array of userType representing users
    @param  NumUsers - number of users in the Users array
    @return void

    Pre-condition: Items and Users arrays are properly initialized with valid data &
                   NumItems and NumUsersare valid integers
************************************************************************************************************/
void 
showAllSellers(itemType Items[], int NumItems, userType Users[], int NumUsers)
{
    int i, j;
    int numItemsForSale;

    printf("\n\n");
    printf("       ___________________________________________________________________________________________________________________\n\n");
    printf("        Displaying all sellers in the system:\n");
    printf("       ____________________________________________________________________________________________________________________\n");
    printf("       | User ID    | Password   | Name                 | Address                        | Phone Number | Items for Sale  |\n");
    printf("       |____________|____________|______________________|________________________________|______________|_________________|\n");

    for (i = 0; i < NumUsers; i++) {
        numItemsForSale = 0;
        for (j = 0; j < NumItems; j++) {
            if (Items[j].sellerID == Users[i].userID) {
                numItemsForSale++;
            }
        }

        if (numItemsForSale > 0) {
            printf("       | %-10d | %-10s | %-20s | %-30s | %-12lld | %-15d |\n", Users[i].userID, Users[i].password, Users[i].name, Users[i].address, Users[i].contactNum, numItemsForSale);
        }
    }

    printf("       |____________|____________|______________________|________________________________|______________|_________________|\n");
    printf("\n");
    printf("       ____________________________________________________________________________________________________________________\n");
}

/***********************************************************************************************************
    showTotalSalesInGivenDuration displays total sales within a given date range

    @return void
************************************************************************************************************/
void 
showTotalSalesInGivenDuration()
{
    char start_date[11], end_date[11];

    printf("\n\t\t     Enter start date (MM/DD/YYYY): ");
    scanf("%s", start_date);

    printf("\t\t     Enter end date (MM/DD/YYYY): ");
    scanf("%s", end_date);

    float totalSales = calculateTotalSales(start_date, end_date);

    printf("\n\t   ===================================================================================");
    printf("\n\t              Total Sales from %s until %s: Php %.2f", start_date, end_date, totalSales);
    printf("\n\t   ===================================================================================\n\n");
}

/***********************************************************************************************************
    calculateTotalSales calculates the total sales within a given date range

    @param  start_date: Start date of the date range (format: MM/DD/YYYY)
    @param  end_date: End date of the date range (format: MM/DD/YYYY)
    @return total sales within the date range

    Pre-condition: start_date and end_date are valid strings in the format MM/DD/YYYY
************************************************************************************************************/
float 
calculateTotalSales(const char* start_date, const char* end_date) 
{
    FILE *file = fopen("transactions.txt", "r");

    char line[256];
    bool withinDateRange = false;
    float totalSales = 0.0;

    while (fgets(line, sizeof(line), file)){
        if (strstr(line, "Date: ") != NULL){
            char transaction_date[11];
            sscanf(line, "Date: %s", transaction_date);
            if (strcmp(transaction_date, start_date) >= 0 && strcmp(transaction_date, end_date) <= 0) {
                withinDateRange = true;
            } else {
                withinDateRange = false;
            }
        } else if (withinDateRange && strstr(line, "Total Amount: ") != NULL) {
            float amount;
            sscanf(line, "Total Amount: %f", &amount);
            totalSales += amount;
        }
    }

    fclose(file);
    return totalSales;
}

/***********************************************************************************************************
    showSellersSales - displays the total sales for each seller within a specified date range

    @param  Items - array of itemType representing items for sale
    @param  NumItems - number of items in the Items array
    @param  Users - array of userType representing users
    @param  NumUsers - number of users in the Users array
    @return void

    Pre-condition: Items and Users arrays are properly initialized with valid data &
                   NumItems and NumUsersare valid integers
************************************************************************************************************/
void 
showSellersSales(itemType Items[], int NumItems, userType Users[], int NumUsers)
{
    char start_date[11], end_date[11];
    printf("\n\t\t     Enter start date (MM/DD/YYYY): ");
    scanf("%s", start_date);
    printf("\t\t     Enter end date (MM/DD/YYYY): ");
    scanf("%s", end_date);

    FILE *file = fopen("transactions.txt", "r");

    int i;
    char line[500];
    int numSellers = 0;
    typedef char String20[21];
    String20 sellerName[MAX_USERS];
    int sellerID[MAX_USERS];
    float totalSales[MAX_USERS];

    for (i = 0; i < MAX_USERS; i++)
        totalSales[i] = 0.0;

    bool withinDateRange = false;
    i = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "Date: ") != NULL)
        {
            char transaction_date[11];
            sscanf(line, "Date: %s", transaction_date);
            if (strcmp(transaction_date, start_date) >= 0 && strcmp(transaction_date, end_date) <= 0)
            {
                withinDateRange = true;
            }
            else
            {
                withinDateRange = false;
            }
        }
        else if (withinDateRange)
        {
            if (strstr(line, "Seller ID: ") != NULL)
            {
                sscanf(line, "Seller ID: %d", &sellerID[i]);
            }
            else if (strstr(line, "Seller Name: ") != NULL)
            {
                sscanf(line, "Seller Name: %[^\n]", sellerName[i]);
            }
            else if (strstr(line, "Total Amount: ") != NULL)
            {
                float amount;
                sscanf(line, "Total Amount: %f", &amount);
                totalSales[i] += amount;
                i++;
                numSellers++;
            }
        }
    }

    fclose(file);

    // Combine total sales for the same seller
    for (i = 0; i < numSellers; i++)
    {
        for (int j = i + 1; j < numSellers; j++)
        {
            if (sellerID[i] == sellerID[j])
            {
                totalSales[i] += totalSales[j];
                totalSales[j] = 0.0;
            }
        }
    }

    printf("\n\n\t\t================================================================\n");
    printf("\n\t\t Total Sales from %s until %s:\n", start_date, end_date);
    printf("\t\t________________________________________________________________\n");
    printf("\t\t|   Seller ID       |     Seller Name        |   Total Sales   |\n");
    printf("\t\t|--------------------------------------------------------------|\n");

    for (i = 0; i < numSellers; i++)
    {
        if (totalSales[i] > 0.0)
        {
            printf("\t\t|     %-14d|     %-19s|  %-15.2f|\n", sellerID[i], sellerName[i], totalSales[i]);
        }
    }

    printf("\t\t|______________________________________________________________|\n\n");
    printf("\t\t================================================================\n\n");
}

/***********************************************************************************************************
    showShopaholics displays the total amount spent by buyers within a specified date range

    @param  Items - array of itemType representing items for sale
    @param  NumItems - number of items in the Items array
    @param  Users - array of userType representing users
    @param  NumUsers - number of users in the Users array
    @return void

    Pre-condition: Items and Users arrays are properly initialized with valid data &
                   NumItems and NumUsersare valid integers
************************************************************************************************************/
void 
showShopaholics(itemType Items[], int NumItems, userType Users[], int NumUsers)
{
    char start_date[11], end_date[11];
    printf("\n\t\t     Enter start date (MM/DD/YYYY): ");
    scanf("%s", start_date);
    printf("\t\t     Enter end date (MM/DD/YYYY): ");
    scanf("%s", end_date);

    FILE *file = fopen("transactions.txt", "r");

    char line[500];
    bool withinDateRange = false;
    int numBuyers = 0;
    BuyerTotal buyerTotals[MAX_USERS];

    for (int i = 0; i < MAX_USERS; i++)
    {
        buyerTotals[i].buyerID = -1;
        buyerTotals[i].totalAmount = 0.0;
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "Date: ") != NULL)
        {
            char transaction_date[11];
            sscanf(line, "Date: %s", transaction_date);
            if (strcmp(transaction_date, start_date) >= 0 && strcmp(transaction_date, end_date) <= 0)
            {
                withinDateRange = true;
            }
            else
            {
                withinDateRange = false;
            }
        }
        else if (withinDateRange)
        {
            if (strstr(line, "Buyer ID: ") != NULL)
            {
                sscanf(line, "Buyer ID: %d", &buyerTotals[numBuyers].buyerID);
                numBuyers++;
            }
            else if (strstr(line, "Buyer Name: ") != NULL)
            {
                sscanf(line, "Buyer Name: %[^\n]", buyerTotals[numBuyers - 1].buyerName);
            }
            else if (strstr(line, "Total Amount: ") != NULL)
            {
                float amount;
                sscanf(line, "Total Amount: %f", &amount);
                // Accumulate total amount for each buyer
                if (buyerTotals[numBuyers - 1].buyerID != -1)
                {
                    buyerTotals[numBuyers - 1].totalAmount += amount;
                }
            }
        }
    }

    fclose(file);

    printf("\n\n\t\t=============================================================");
    printf("\n\t\tTotal Amount Buyers Spent from %s to %s:\n", start_date, end_date);
    printf("\t\t________________________________________________________\n");
    printf("\t\t|   Buyer ID     |     Buyer Name     |  Total Amount  |\n");
    printf("\t\t|------------------------------------------------------|\n");

    for (int i = 0; i < MAX_USERS; i++)
    {
        if (buyerTotals[i].buyerID != -1 && buyerTotals[i].totalAmount > 0.0)
        {
            float totalAmount = 0.0;
            int currentBuyerID = buyerTotals[i].buyerID;
            char currentBuyerName[21];
            strcpy(currentBuyerName, buyerTotals[i].buyerName);
            for (int j = i; j < MAX_USERS; j++)
            {
                if (buyerTotals[j].buyerID == currentBuyerID && buyerTotals[j].totalAmount > 0.0)
                {
                    totalAmount += buyerTotals[j].totalAmount;
                    buyerTotals[j].totalAmount = 0.0;
                }
            }
            // Print buyer's total amount only if it's greater than 0
            if (totalAmount > 0.0) {
                printf("\t\t|   %-13d| %-19s|  %-14.2f|\n", currentBuyerID, currentBuyerName, totalAmount);
            }
        }
    }
    printf("\t\t|______________________________________________________|\n\n");
    printf("\t\t=============================================================\n\n\n");

}

/***********************************************************************************************************
    printMainMenu displays the main menu with options for user registration, user menu, admin menu, and exit

    @return the user-selected option as an integer 
************************************************************************************************************/
int 
printMainMenu()
{
    int option;

    printf("\n");
    printf("                                 ~~~ Welcome to Shopee ~~~\n");
    printf("                _________________________________________________________\n");
    printf("               |                        MAIN MENU                        |\n");
    printf("               |                                                         |\n");
    printf("               |     [1] Register as New User                            |\n");
    printf("               |     [2] User Menu                                       |\n");
    printf("               |     [3] Admin Menu                                      |\n");
    printf("               |     [4] Exit Application                                |\n");
    printf("               |_________________________________________________________|\n\n");

    printf("\t\tEnter option: ");
    scanf("%d", &option);

    return option;
}

/***********************************************************************************************************
    selectUserMenu function displays the user menu for the Shopee program and handles user 
    input for different options

    @param  Items - an array of item types
    @param  NumItems - a pointer to the number of items
    @param  currentUserID - the ID of the current user
    @param  cartItems - an array of cart item types
    @param  Transacs - an array of transaction types
    @param  Users - an array of user types
    @param  NumUsers - the number of users
    @return void

    Pre-condition: Items, NumItems, cartItems, Transacs, Users are valid arrays with appropriate values
************************************************************************************************************/
void 
selectUserMenu(itemType Items[], int *NumItems, int currentUserID, CartItemType cartItems[], transactionType Transacs[], userType Users[], int NumUsers)
{
    int option;
    int NumItemsInCart = 0;

    do {
        printf("                _________________________________________________________\n");
        printf("\n");
        printf("                                 ~~~ Welcome to Shopee ~~~\n");
        printf("                _________________________________________________________\n");
        printf("               |                        USER MENU                        |\n");
        printf("               |                                                         |\n");
        printf("               |     [1] Sell Menu                                       |\n");
        printf("               |     [2] Buy Menu                                        |\n");
        printf("               |     [3] Exit User Menu                                  |\n");
        printf("               |_________________________________________________________|\n\n");

        printf("\t\tEnter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                selectSellMenu(Items, NumItems, currentUserID);
                break;

            case 2:
                // Check for previously saved cart and load contents from file
                loadCartData(currentUserID, cartItems, &NumItemsInCart);
                selectBuyMenu(Items, &NumItemsInCart, NumItems, currentUserID, cartItems, Transacs, Users, NumUsers);
                break;

            case 3:
                // Save cart to file and exit user menu
                saveCartData(currentUserID, cartItems, NumItemsInCart);
                printf("\t\tExiting User Menu...\n");
                break;

            default:
                printf("\nInvalid option. Please select a valid option.\n");
                break;
        }
    } while (option != 3);
}

/***********************************************************************************************************
    loadCartData loads cart data from a file for a given user ID

    @param  currentUserID - the ID of the current user
    @param  cartItems - an array of CartItemType to store the loaded cart items
    @param  NumItemsInCart - a pointer to an integer to store the number of items loaded in the cart
    @return void

    Pre-condition: currentUserID is a valid user ID, cartItems is a valid array of CartItemType, &
                   NumItemsInCart is a valid pointer to a valid integer
************************************************************************************************************/
void 
loadCartData(int currentUserID, CartItemType cartItems[], int *NumItemsInCart)
{
    char filename[10];
    sprintf(filename, "%d.txt", currentUserID); // Creates filename based on user ID
    
    FILE *cartFile = fopen(filename, "r");
    
    // Check if the file is empty
    if (cartFile != NULL) {
        fseek(cartFile, 0, SEEK_END);
        long fileSize = ftell(cartFile);
        if (fileSize == 0) {
            fclose(cartFile);
            return;
        }
        rewind(cartFile);
        
        while (*NumItemsInCart < MAX_ITEMS_ALL && !feof(cartFile)) {
            fscanf(cartFile, "%d %d\n", &cartItems[*NumItemsInCart].productID, &cartItems[*NumItemsInCart].sellerID);
            fgets(cartItems[*NumItemsInCart].itemName, 21, cartFile);
            cartItems[*NumItemsInCart].itemName[strcspn(cartItems[*NumItemsInCart].itemName, "\n")] = '\0';
            fgets(cartItems[*NumItemsInCart].category, 16, cartFile);
            cartItems[*NumItemsInCart].category[strcspn(cartItems[*NumItemsInCart].category, "\n")] = '\0';
            fscanf(cartFile, "%d %f\n", &cartItems[*NumItemsInCart].quantity, &cartItems[*NumItemsInCart].unitPrice);
            fscanf(cartFile, "\n");
            (*NumItemsInCart)++;
        }
        
        fclose(cartFile);
    }
}

/***********************************************************************************************************
    selectSellMenu displays a menu for selling items and performs actions based on user input

    @param Items - Array of itemType representing the inventory of items
    @param NumItems - Pointer to an integer representing the number of items in the inventory
    @param sellerID - Integer representing the seller's ID
    @return void

    Pre-condition: Items is a valid array of itemType, NumItems is a valid pointer to a valid integer, 
                   and sellerID is a valid integer
************************************************************************************************************/
void 
selectSellMenu(itemType Items[], int *NumItems, int sellerID)
{
    int sellOption;

    do {
        printf("\n");
        printf("                _________________________________________________________\n");
        printf("                                   ~~~ SELL MENU ~~~\n");
        printf("                _________________________________________________________\n");
        printf("               |                        OPTIONS                          |\n");
        printf("               |                                                         |\n");
        printf("               |     [1] Add New Item                                    |\n");
        printf("               |     [2] Edit Stock                                      |\n");
        printf("               |     [3] Show My Products                                |\n");
        printf("               |     [4] Show My Low Stock Products                      |\n");
        printf("               |     [5] Exit Sell Menu                                  |\n");
        printf("               |_________________________________________________________|\n\n");

        printf("\t\tEnter option: ");
        scanf("%d", &sellOption);

        switch (sellOption)
        {
            case 1:
                addNewItem(Items, NumItems, sellerID);
                break;

            case 2:
                selectEditStock(Items, NumItems, sellerID);
                break;

            case 3:
                showMyProducts(Items, NumItems, sellerID);
                break;

            case 4:
                showLowStockProducts(Items, NumItems, sellerID);
                break;

            case 5:
                break;

            default:
                printf("\n\t\tInvalid option....");
                break;
        }
    } while (sellOption != 5);
}

/***********************************************************************************************************
    addNewItem adds a new item to the list of items of the current seller

    @param  Items - Array of itemType representing the inventory of items
    @param  NumItems - Pointer to the number of items in the array
    @param  sellerID - ID of the seller adding the item
    @return void

    Pre-condition: Items is a valid array of itemType, NumItems is a valid pointer to a valid integer, 
                   and sellerID is a valid integer
************************************************************************************************************/
void 
addNewItem(itemType Items[], int *NumItems, int sellerID)
{
    int i, numSellerItems = 0;

    for (i = 0; i < *NumItems; i++) {
        if (Items[i].sellerID == sellerID) {
            numSellerItems++;
        }
    }

    if (numSellerItems == MAX_ITEMS) {
        printf("\t\tYou have reached the maximum number of items you can sell (%d items).\n", MAX_ITEMS);
        return;
    }

    printf("                _________________________________________________________\n\n");

    itemType newItem;
    printf("\t\tEnter Product ID: ");
    scanf("%d", &newItem.productID);

    for (i = 0; i < *NumItems; i++){
        if (newItem.productID == Items[i].productID){
            printf("\t\tProduct ID already exists. Please enter a unique ID.\n");

            // Call function recursively until a unique product ID is entered
            addNewItem(Items, NumItems, sellerID);
            return;
        }
    }

    printf("\t\tEnter Item Name: ");
    scanf(" %[^\n]", newItem.itemName);

    printf("\t\tEnter Item Category: ");
    scanf(" %[^\n]", newItem.category);

    printf("\t\tEnter Item Description: ");
    scanf(" %[^\n]", newItem.description);

    printf("\t\tEnter Item Quantity Available: ");
    scanf("%d", &newItem.quantityAvailable);

    printf("\t\tEnter Item Unit Price: ");
    scanf("%f", &newItem.unitPrice);

    newItem.sellerID = sellerID;

    Items[*NumItems] = newItem;
    (*NumItems)++;

    printf("\n\t\tItem ID No. %d added successfully.\n", newItem.productID);
    printf("                _________________________________________________________\n");
}

/***********************************************************************************************************
    selectEditStock takes input from the user for the desired edit option and updates the inventory 
    accordingly the seller can replenish stock, change price, change item name, change category, and 
    change description

    @param Items - Array of itemType representing the inventory of items
    @param NumItems - Pointer to an integer representing the number of items in the inventory
    @param sellerID - Integer representing the seller ID
    @return void

    Pre-condition: Items is a valid array of itemType, NumItems is a valid pointer to a valid integer, 
                   and sellerID is a valid integer
************************************************************************************************************/
void
selectEditStock(itemType Items[], int *NumItems, int sellerID)
{
    int i, editOption, productID, quantity;
    int index = -1;
    float newPrice;
    char newName[21], newCategory[16], newDescription[31];

    showMyProducts(Items, NumItems, sellerID);

    printf("\n\t\tEnter the Product ID of the item you want to edit: ");
    scanf("%d", &productID);

    for ( i = 0; i < *NumItems; i++) {
        if (productID == Items[i].productID)
            index = i;
    }
    
    if (index == -1) {
        printf("\t\tInvalid product ID. Returning to Sell Menu...\n");
        return;
    }

    do {
        printf("\n");
        printf("                                 ~~~ EDIT STOCK MENU ~~~\n");
        printf("                _________________________________________________________\n");
        printf("               |                        OPTIONS                          |\n");
        printf("               |                                                         |\n");
        printf("               |     [1] Replenish                                       |\n");
        printf("               |     [2] Change Price                                    |\n");
        printf("               |     [3] Change Item Name                                |\n");
        printf("               |     [4] Change Category                                 |\n");
        printf("               |     [5] Change Description                              |\n");
        printf("               |     [6] Finish Editing                                  |\n");
        printf("               |_________________________________________________________|\n\n");

        printf("\t\tEnter option: ");
        scanf("%d", &editOption);

        switch (editOption)
        {
            case 1:
                // replenish product
                printf("\t\tEnter quantity to add: ");
                scanf("%d", &quantity);
                Items[index].quantityAvailable += quantity;
                printf("\t\tProduct replenished successfully.\n");
                break;

            case 2:
                // change price
                printf("\t\tEnter new unit price: ");
                scanf("%f", &newPrice);
                Items[index].unitPrice = newPrice;
                printf("\t\tPrice changed successfully.\n");
                break;

            case 3:
                // change item name
                printf("\t\tEnter new item name: ");
                scanf(" %[^\n]", newName);
                strcpy(Items[index].itemName, newName);
                printf("\t\tItem name changed successfully.\n");
                break;

            case 4:
                // change category
                printf("\t\tEnter new category: ");
                scanf(" %[^\n]", newCategory);
                strcpy(Items[index].category, newCategory);
                printf("\t\tCategory changed successfully.\n");
                break;
            
            case 5:
                // change description
                printf("\t\tEnter new description: ");
                scanf(" %[^\n]", newDescription);
                strcpy(Items[index].description, newDescription);
                printf("\t\tDescription changed successfully.\n");
                break;

            case 6:
                printf("\t\tExiting Edit Stock Menu...\n");
                break;

            default:
                printf("\n\t\tInvalid option. Please select a valid option: \n");
                break;
        }

    } while (editOption != 6);

}

/***********************************************************************************************************
    showLowStockProducts displays the products with low stock of current seller

    @param  Items - an array of itemType representing the inventory of products
    @param  NumItems - a pointer to an integer representing the number of items in the inventory
    @param  sellerID - an integer representing the ID of the seller whose products are to be checked
    @return void

    Pre-condition: Items array is properly initialized and NumItems points to the correct number of items
************************************************************************************************************/
void 
showLowStockProducts(itemType Items[], int *NumItems, int sellerID)
{
    int currentItem = 0;
    char option;
    int lowStockFound = 0;

    printf("\n");

    while (currentItem < *NumItems) {
        if (Items[currentItem].sellerID == sellerID && Items[currentItem].quantityAvailable < 5) {
            printf("\t\tProduct ID: %d\n", Items[currentItem].productID);
            printf("\t\tProduct Name: %s\n", Items[currentItem].itemName);
            printf("\t\tProduct Category: %s\n", Items[currentItem].category);
            printf("\t\tProduct Description: %s\n", Items[currentItem].description);
            printf("\t\tProduct Quantity: %d\n", Items[currentItem].quantityAvailable);
            printf("\t\tProduct Price: %.2f\n", Items[currentItem].unitPrice);

            lowStockFound = 1;

            do {
                printf("\n\t\tPress N for next product, or X to exit view: ");
                scanf(" %c", &option);
            } while (option != 'N' && option != 'n' && option != 'X' && option != 'x');

            if (option == 'X' || option == 'x')
                return;
        }
        currentItem++;
    }

    if (!lowStockFound)
        printf("\n\t\tNo products are low in stock.\n");

}

/***********************************************************************************************************
    showMyProducts displays the list of products being sold of the current seller

    @param  Items - array of itemType representing the list of products
    @param  NumItems - pointer to an integer representing the number of items in the list
    @param  sellerID - integer representing the ID of the seller whose products are to be displayed
    @return void

    Pre-condition: Items array is properly initialized, NumItems is a valid pointer, and
                    sellerID is a valid ID of the current login user
************************************************************************************************************/
void 
showMyProducts(itemType Items[], int *NumItems, int sellerID)
{
    int i, j, idLen, nameLen, catLen, priceLen, quanLen;
    int count = 0;
    int maxIDLen = 10;    
    int maxNameLen = 20;
    int maxCatLen = 15;  
    int maxPriceLen = 10; 
    int maxQuanLen = 10; 

    // Determine the maximum length of each column
    for (i = 0; i < *NumItems; i++){
        if (Items[i].sellerID == sellerID){
            idLen = snprintf(NULL, 0, "%d", Items[i].productID);
            nameLen = strlen(Items[i].itemName);
            catLen = strlen(Items[i].category);
            priceLen = snprintf(NULL, 0, "%.2f", Items[i].unitPrice);
            quanLen = snprintf(NULL, 0, "%d", Items[i].quantityAvailable);

            if (idLen > maxIDLen)
                maxIDLen = idLen;

            if (nameLen > maxNameLen)
                maxNameLen = nameLen;

            if (catLen > maxCatLen)
                maxCatLen = catLen;

            if (priceLen > maxPriceLen)
                maxPriceLen = priceLen;

            if (quanLen > maxQuanLen)
                maxQuanLen = quanLen;

            count++;
        }
    }

    // Sort the items based on productID in increasing order
    for (i = 0; i < *NumItems - 1; i++){
        for (j = 0; j < *NumItems - i - 1; j++){
            if (Items[j].productID > Items[j+1].productID){
                // Swap the items
                itemType temp = Items[j];
                Items[j] = Items[j+1];
                Items[j+1] = temp;
            }
        }
    }

    printf("\t");
    for (i = 0; i < 81; i++)
        printf("%c", 196);
        
    printf("\n");
    printf("\t| %-*s | %-*s | %-*s | %-*s | %-*s |\n", maxIDLen, "ID", maxNameLen, "Item Name", maxCatLen, "Category", maxPriceLen, "Unit Price", maxQuanLen, "Quantity");
    printf("\t| %-*s | %-*s | %-*s | %-*s | %-*s |\n", maxIDLen, "----------", maxNameLen, "--------------------", maxCatLen, "---------------", maxPriceLen, "----------", maxQuanLen, "----------");

    for (i = 0; i < *NumItems; i++) {
        if (Items[i].sellerID == sellerID) {
            printf("\t| %-*d | %-*s | %-*s | %*.2f | %*d |\n", maxIDLen, Items[i].productID, maxNameLen, Items[i].itemName, maxCatLen, Items[i].category, maxPriceLen, Items[i].unitPrice, maxQuanLen, Items[i].quantityAvailable);
        }
    }

    printf("\t");
    for (i = 0; i < 81; i++)
        printf("%c", 196);

    if (count == 0)
        printf("\n\tYou currently have no products for sale.\n");

}

/***********************************************************************************************************
    selectBuyMenu displays the buy menu options to the user and performs actions based on user's input

    @param  Items - array of item structures containing product information
    @param  NumItemsInCart - pointer to the number of items in the cart
    @param  NumItems - pointer to the total number of items available
    @param  userID - ID of the currently logged in user
    @param  cartItems - array of item structures representing items in the cart
    @param  Transacs - array of transaction structures containing transaction information
    @param  Users - array of user structures containing user information
    @param  NumUsers - total number of users in the system
    @return void

    Pre-condition: Items, NumItemsInCart, NumItems, cartItems, Transacs, and Users are initialized 
                   properly and contain valid data
************************************************************************************************************/
void 
selectBuyMenu(itemType Items[], int *NumItemsInCart, int *NumItems, int userID, CartItemType cartItems[], transactionType Transacs[], userType Users[], int NumUsers)
{
    int buyOption;

    do {
        printf("                _________________________________________________________\n");
        printf("\n");
        printf("                                    ~~~ BUY MENU ~~~\n");
        printf("                _________________________________________________________\n");
        printf("               |                        OPTIONS                          |\n");
        printf("               |                                                         |\n");
        printf("               |     [1] View All Products                               |\n");
        printf("               |     [2] Show All Products by a Specific Seller          |\n");
        printf("               |     [3] Search Products by Category                     |\n");
        printf("               |     [4] Search Products by Name                         |\n");
        printf("               |     [5] Add to Cart                                     |\n");
        printf("               |     [6] Edit Cart                                       |\n");
        printf("               |     [7] Check Out Menu                                  |\n");
        printf("               |     [8] Exit Buy Menu                                   |\n");
        printf("               |_________________________________________________________|\n\n");

        printf("\t\tEnter option: ");
        scanf("%d", &buyOption);

        switch (buyOption) 
        {
            case 1:
                viewAllProducts(Items, *NumItems);
                break;

            case 2:
                showProductsBySeller(Items, *NumItems);
                break;

            case 3:
                searchProductsByCategory(Items, *NumItems);
                break;

            case 4:
                searchProductsByName(Items, *NumItems);
                break;

            case 5:
                addToCart(Items, NumItemsInCart, NumItems, userID, cartItems);
                break;

            case 6:
                editCart(cartItems, NumItemsInCart, Items, NumItems);
                break;

            case 7:
                checkOutMenu(cartItems, NumItemsInCart, Items, NumItems, Transacs, userID, Users, NumUsers);
                break;

            case 8:
                // exits the Buy Menu
                break;

            default:
                printf("\t\tInvalid option. Please select a valid option.\n");
                break;
        }
    } while (buyOption != 8);
}

/***********************************************************************************************************
    viewAllProducts displays all products grouped by seller ID

    @param  Items - array of item structures containing product information
    @param  NumItems - number of items in the array
    @return void

    Pre-condition: Items array contains valid item data, NumItems is valid integer
************************************************************************************************************/
void 
viewAllProducts(itemType Items[], int NumItems)
{
    int i, j;
    char input;
    int numSellers = 0;   
    int sellerIDs[MAX_USERS]; 
    int sellerIndex = 0;   
    int found, currentSellerID, sellerFound;

    for (i = 0; i < NumItems; i++) {
        found = 0;
        for (j = 0; j < numSellers && !found; j++) {
            if (Items[i].sellerID == sellerIDs[j])
                found = 1; // Seller ID already found
            
        }
        if (!found)
            sellerIDs[numSellers++] = Items[i].sellerID;
    }

    for (i = 0; i < numSellers-1; i++) {
        for (j = 0; j < numSellers-i-1; j++) {
            if (sellerIDs[j] > sellerIDs[j+1]) {
                int temp = sellerIDs[j];
                sellerIDs[j] = sellerIDs[j+1];
                sellerIDs[j+1] = temp;
            }
        }
    }

    do {
        printf("\t_________________________________________________________________________________\n");
        printf("\n\tSeller ID: %d\n", sellerIDs[sellerIndex]);
        printf("\t");

        for (i = 0; i < 81; i++)
            printf("%c", 196);

        printf("\n");
        printf("\t| %-*s | %-*s | %-*s | %-*s | %-*s |\n", 10, "ID", 20, "Item Name", 15, "Category", 10, "Unit Price", 10, "Quantity");
        printf("\t| %-*s | %-*s | %-*s | %-*s | %-*s |\n", 10, "----------", 20, "--------------------", 15, "---------------", 10, "----------", 10, "----------");

        // Print the table for current seller
        currentSellerID = sellerIDs[sellerIndex];
        sellerFound = 0; 
        for (i = 0; i < NumItems; i++){
            if (Items[i].sellerID == currentSellerID){
                sellerFound = 1;
                printf("\t| %-*d | %-*s | %-*s | %*.2f | %*d |\n", 10, Items[i].productID, 20, Items[i].itemName, 15, Items[i].category, 10, Items[i].unitPrice, 10, Items[i].quantityAvailable);
            }
        }

        if (!sellerFound)
            printf("\t| %-*d | %-*s | %-*s | %*.2f | %*d |\n", 10, currentSellerID, 20, "--------------------", 15, "---------------", 10, 0.0, 10, 0);
        
        printf("\t");
        for (i = 0; i < 81; i++)
            printf("%c", 196);

        printf("\n");
        printf("\n");

        printf("\tPress 'N' for next seller, or 'X' to exit and go back to Buy Menu: ");
        scanf(" %c", &input);

        if (input == 'n' || input == 'N') {
            sellerIndex++;
            if (sellerIndex == numSellers) {
                sellerIndex = 0;
            }
        }

    } while (input != 'x' && input != 'X');
}

/***********************************************************************************************************
    showProductsBySeller displays the products of the desired seller by the user

    @param  Items - array of item structures containing product information
    @param  NumItems - number of items in the array
    @return void

    Pre-condition: Items array contains valid item data, NumItems is valid integer
************************************************************************************************************/
void 
showProductsBySeller(itemType Items[], int NumItems)
{
    int sellerID;
    int found = 0;

    printf("\t\tEnter seller's ID: ");
    scanf("%d", &sellerID);

    printf("\t_________________________________________________________________________________\n");
    printf("\n\tSeller ID: %d\n", sellerID);
    printf("\t");

    for (int i = 0; i < 81; i++)
        printf("%c", 196);

    printf("\n");
    printf("\t| %-*s | %-*s | %-*s | %-*s | %-*s |\n", 10, "ID", 20, "Item Name", 15, "Category", 10, "Unit Price", 10, "Quantity");
    printf("\t| %-*s | %-*s | %-*s | %-*s | %-*s |\n", 10, "----------", 20, "--------------------", 15, "---------------", 10, "----------", 10, "----------");

    for (int i = 0; i < NumItems; i++) {
        if (Items[i].sellerID == sellerID) {
            found = 1; // Seller found
            printf("\t| %-*d | %-*s | %-*s | %*.2f | %*d |\n", 10, Items[i].productID, 20, Items[i].itemName, 15, Items[i].category, 10, Items[i].unitPrice, 10, Items[i].quantityAvailable);
        }
    }

    if (!found) {
        printf("\t| %-*s |\n", 81, "No products found for the given seller ID.");
    }

    printf("\t|_______________________________________________________________________________|\n");
}

/***********************************************************************************************************
    searchProductsByCategory searches for products of entered category and displays the results

    @param  Items - array of item structures containing product information
    @param  NumItems - number of items in the array
    @return void

    Pre-condition: Items array contains valid item data, NumItems is valid integer
************************************************************************************************************/
void 
searchProductsByCategory(itemType Items[], int NumItems)
{
    char category[15];
    int found = 0;
    int currentItem = 0;
    char input;

    printf("\t\tEnter the category to search: ");
    scanf(" %[^\n]", category);

    // Convert the input category to lowercase
    for (int i = 0; category[i]; i++) {
        category[i] = tolower(category[i]);
    }

    do {
        // Convert the category of the currentItem to lowercase
        char currentItemCategory[15];
        strcpy(currentItemCategory, Items[currentItem].category);
        for (int i = 0; currentItemCategory[i]; i++) {
            currentItemCategory[i] = tolower(currentItemCategory[i]);
        }

        if (strcmp(currentItemCategory, category) == 0) {
            printf("\n\t\tProduct ID: %d\n", Items[currentItem].productID);
            printf("\t\tProduct Name: %s\n", Items[currentItem].itemName);
            printf("\t\tProduct Category: %s\n", Items[currentItem].category);
            printf("\t\tProduct Description: %s\n", Items[currentItem].description);
            printf("\t\tProduct Quantity: %d\n", Items[currentItem].quantityAvailable);
            printf("\t\tProduct Price: %.2f\n", Items[currentItem].unitPrice);
            found = 1;

            printf("\n\t\tPress N for next product, or X to exit: ");
            scanf(" %c", &input);
            if (input == 'X' || input == 'x') {
                // Do nothing, the loop will exit naturally
            } else if (input != 'N' && input != 'n') {
                printf("\t\tInvalid input. Exiting view.\n");
                input = 'X';
            }
        }
        currentItem++;

        if (currentItem == NumItems && input != 'X' && input != 'x' && found) {
            printf("\t\tNo more products in the category.\n");
            printf("\t\tPress X to exit or any other key to continue: ");
            scanf(" %c", &input);
            if (input == 'X' || input == 'x') {
                // Do nothing, the loop will exit naturally
            } else {
                currentItem = 0;
            }
        }
    } while (currentItem < NumItems && input != 'X' && input != 'x');

    if (!found)
        printf("\t\tNo products found in the given category.\n");
}

/***********************************************************************************************************
    searchProductsByName searches for products in an array of Items by product name keyword entered
    by the user

    @param  Items - array of item structures containing product information
    @param  NumItems - number of items in the array
    @return void

    Pre-condition: Items array contains valid item data, NumItems is valid integer
************************************************************************************************************/
void 
searchProductsByName(itemType Items[], int NumItems)
{
    char searchKeyword[32];       
    int found = 0;  
    int numResults = 0;       
    itemType searchResults[MAX_ITEMS]; 
    int currentResult = 0; 
    char input;   

    printf("\t\tEnter product name keyword: ");
    scanf(" %99[^\n]", searchKeyword);

    for (int i = 0; searchKeyword[i]; i++)
        searchKeyword[i] = tolower(searchKeyword[i]);
    
    for (int i = 0; i < NumItems; i++) {
        char itemNameLowerCase[100];
        strcpy(itemNameLowerCase, Items[i].itemName);

        for (int j = 0; itemNameLowerCase[j]; j++)
            itemNameLowerCase[j] = tolower(itemNameLowerCase[j]);

        if (strstr(itemNameLowerCase, searchKeyword) != NULL) {
            searchResults[numResults++] = Items[i];
            found = 1;
        }
    }

    if (found) {
        printf("\n\t\tSearch Results:\n");

        while (currentResult < numResults) {
            printf("\t\tProduct ID: %d\n", searchResults[currentResult].productID);
            printf("\t\tProduct Name: %s\n", searchResults[currentResult].itemName);
            printf("\t\tProduct Category: %s\n", searchResults[currentResult].category);
            printf("\t\tProduct Description: %s\n", searchResults[currentResult].description);
            printf("\t\tProduct Quantity: %d\n", searchResults[currentResult].quantityAvailable);
            printf("\t\tProduct Price: %.2f\n", searchResults[currentResult].unitPrice);
            printf("\n");

            if (currentResult < numResults - 1) {
                printf("\t\tPress N for next product, or X to exit: ");
                scanf(" %c", &input);
                printf("\n");
                if (input == 'N' || input == 'n'){
                    currentResult++;
                } else {
                    printf("\t\tInvalid input. Exiting search.\n");
                    currentResult++;
                }
            } 
            else {
                printf("\t\tNo more products found.\n");
                printf("\t\tPress X to exit or any other key to continue: ");
                scanf(" %c", &input);
                printf("\n");
                if (input == 'X' || input == 'x'){
                    currentResult = numResults;
                } else {
                    currentResult = 0;
                }
            }
        }
    } 
    else {
        printf("\t\tNo products found with the specified keyword.\n");
    }
}

/***********************************************************************************************************
    addToCart adds an item desired by the buyer to the cart

    @param  Items - array of itemType representing the available items
    @param  NumItemsInCart - pointer to an int representing the number of items currently in the cart
    @param  NumItems - pointer to an int representing the total number of available items
    @param  userID - int representing the user ID
    @param  cartItems - array of CartItemType representing the items in the cart
    @return void

    Pre-condition: Items and cartItems arrays are properly initialized, NumItemsInCart and NumItems
                   are valid pointers to integers
************************************************************************************************************/
void 
addToCart(itemType Items[], int *NumItemsInCart, int *NumItems, int userID, CartItemType cartItems[])
{
    int productID, productIndex, quantity;

    if (*NumItemsInCart >= 10) {
        printf("\t\tCart is full. Please proceed to Edit Cart or Check Out first before adding more items.\n");
        return;
    }

    printf("\t\tEnter the product ID: ");
    scanf("%d", &productID);

    productIndex = 0; 
    while (productIndex < *NumItems && Items[productIndex].productID != productID) {
        productIndex++;
    }

    if (productIndex == *NumItems){
        printf("\t\tProduct not found. Please enter a valid product ID.\n");
        return;
    }

    if (Items[productIndex].sellerID == userID) {
        printf("\t\tYou cannot buy your own product.\n");
        return;
    }

    printf("\t\tEnter the quantity: ");
    scanf("%d", &quantity);

    if (quantity > Items[productIndex].quantityAvailable) {
        printf("\t\tInsufficient quantity. Please enter a valid quantity.\n");
        return;
    }

    cartItems[*NumItemsInCart].productID = Items[productIndex].productID;
    strcpy(cartItems[*NumItemsInCart].itemName, Items[productIndex].itemName);
    strcpy(cartItems[*NumItemsInCart].category, Items[productIndex].category);
    cartItems[*NumItemsInCart].unitPrice = Items[productIndex].unitPrice;
    cartItems[*NumItemsInCart].sellerID = Items[productIndex].sellerID;
    cartItems[*NumItemsInCart].quantity= quantity;

    (*NumItemsInCart)++; 

    printf("\t\tItem added to cart successfully.\n");

}

/***********************************************************************************************************
    editCart function allows the user to edit items in the cart
    @param  cartItems - array of CartItemType representing items in the cart
    @param  numItemsInCart - pointer to an integer representing the number of items in the cart
    @param  items - array of itemType representing all available items
    @param  NumItems - pointer to an integer representing the total number of available items
    @return void

    Pre-condition: cartItems, numItemsInCart, items, and NumItems are valid and properly initialized
************************************************************************************************************/
void 
editCart(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems)
{
    int editOption;
    
    do {
        // Show items in cart
        printf("\n");
        printf("\t=================================================================================\n");
        printf("\t                             ~ ITEMS IN YOUR CART ~                              \n");
        printf("\t=================================================================================\n");
        printf("\t| %-*s | %-*s | %-*s | %-*s | %-*s |\n", 10, "ID", 20, "Item Name", 15, "Category", 10, "Unit Price", 10, "Quantity");
        printf("\t| %-*s | %-*s | %-*s | %-*s | %-*s |\n", 10, "----------", 20, "--------------------", 15, "---------------", 10, "----------", 10, "----------");

        for (int i = 0; i < *numItemsInCart; i++)
            printf("\t| %-*d | %-*s | %-*s | %*.2f | %*d |\n", 10, cartItems[i].productID, 20, cartItems[i].itemName, 15, cartItems[i].category, 10, cartItems[i].unitPrice, 10, cartItems[i].quantity);
    
        printf("\t|_______________________________________________________________________________|\n\n");


        printf("                _________________________________________________________\n");
        printf("\n");
        printf("                                 ~~~ EDIT CART MENU ~~~\n");
        printf("                _________________________________________________________\n");
        printf("               |                        OPTIONS                          |\n");
        printf("               |                                                         |\n");
        printf("               |     [1] Remove all items from Seller                    |\n");
        printf("               |     [2] Remove Specific Item                            |\n");
        printf("               |     [3] Edit Quantity                                   |\n");
        printf("               |     [4] Finish Edit Cart                                |\n");
        printf("               |_________________________________________________________|\n\n");
        printf("\t\tEnter option: ");
        scanf("%d", &editOption);

        switch (editOption){
            case 1:
                removeItemsFromSeller(cartItems, numItemsInCart, items, NumItems);
                break;

            case 2:
                removeSpecificItem(cartItems, numItemsInCart, items, NumItems);
                break;

            case 3:
                editQuantity(cartItems, numItemsInCart, items, NumItems);
                break;

            case 4:
                // Finish Edit Cart
                break;

            default:
                printf("\t\tInvalid option. Please select a valid option.\n");
                break;
        }
    } while (editOption != 4);
}

/***********************************************************************************************************
    removeItemsFromSeller removes items from the cart based on the seller ID

    @param  cartItems - array of cart items
    @param  numItemsInCart - pointer to the number of items in the cart
    @param  items - array of available items
    @param  NumItems - pointer to the total number of available items
    @return void

    Pre-condition: cartItems and items are valid arrays, numItemsInCart and NumItems are valid pointers
************************************************************************************************************/
void 
removeItemsFromSeller(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems){
    int sellerID, removed;

    printf("\t\tEnter seller ID: ");
    scanf("%d", &sellerID);
    removed = 0;

    for (int i = 0; i < *numItemsInCart; i++) {
        if (cartItems[i].sellerID == sellerID){
            // Remove item from cart
            for (int j = i; j < *numItemsInCart - 1; j++)
                cartItems[j] = cartItems[j + 1];

            (*numItemsInCart)--;
            removed = 1;
            i--;
        }
    }

    if (removed)
        printf("\t\tAll items from Seller with ID %d have been removed from the cart.\n", sellerID);
    else
        printf("\t\tNo items from Seller with ID %d found in the cart.\n", sellerID);
}

/***********************************************************************************************************
    removeSpecificItem removes the desired selected item by the buyer

    @param  cartItems - array of cart items
    @param  numItemsInCart - pointer to the number of items in the cart
    @param  items - array of available items
    @param  NumItems - pointer to the total number of available items
    @return void

    Pre-condition: cartItems and items are valid arrays, numItemsInCart and NumItems are valid pointers
************************************************************************************************************/
void 
removeSpecificItem(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems)
{
    int productID;
    int removed = 0;

    printf("\t\tEnter product ID: ");
    scanf("%d", &productID);

    for (int i = 0; i < *numItemsInCart; i++) {
        if (cartItems[i].productID == productID){
            // Remove item from cart
            for (int j = i; j < *numItemsInCart - 1; j++)
                cartItems[j] = cartItems[j + 1];

            (*numItemsInCart)--;
            removed = 1;
            i--; 
        }
    }

    if (removed)
        printf("\t\tItem with product ID %d has been removed from the cart.\n", productID);
    else
        printf("\t\tItem with product ID %d not found in the cart.\n", productID);
}

/***********************************************************************************************************
    editQuantity updates the quantity of an item in the cart

    @param  cartItems - array of cart items
    @param  numItemsInCart - pointer to the number of items in the cart
    @param  items - array of available items
    @param  NumItems - pointer to the total number of available items
    @return void

    Pre-condition: cartItems and items are valid arrays, numItemsInCart and NumItems are valid pointers
************************************************************************************************************/
void 
editQuantity(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems) 
{
    int productID, newQuantity;
    int found = 0; 
    int i, j;

    printf("\t\tEnter product ID: ");
    scanf("%d", &productID);

    i = 0;
    while (!found && i < *numItemsInCart){
        if (cartItems[i].productID == productID){
            printf("\t\tEnter new quantity: ");
            scanf("%d", &newQuantity);

            // Check if the new quantity is more than the available quantity
            j = 0;
            while (j < *NumItems && items[j].productID != productID){
                j++;
            }
            if (j < *NumItems){
                if (newQuantity > items[j].quantityAvailable) {
                    printf("\t\tQuantity entered is more than available quantity.\n");
                    return;
                }
            }

            cartItems[i].quantity = newQuantity;
            
            printf("\t\tQuantity updated successfully.\n");
            found = 1;
        }
        i++;
    }

    if (!found)
        printf("\t\tProduct ID not found in cart.\n");

}

/***********************************************************************************************************
    saveCartData saves the cart data of a user to a file

    @param  currentUserID - integer representing the ID of the current user
    @param  cartItems - array of CartItemType representing the cart items
    @param  NumItemsInCart - integer representing the number of items in the cart
    @return void

    Pre-condition: cartItems is a valid array of CartItemType with NumItemsInCart number of items,
                   currentUserID is the buyer's ID
************************************************************************************************************/
void 
saveCartData(int currentUserID, CartItemType cartItems[], int NumItemsInCart)
{
    char filename[10];
    sprintf(filename, "%d.txt", currentUserID); // Create filename based on user ID
    FILE *cartFile = fopen(filename, "w");
    if (cartFile != NULL) {
        for (int i = 0; i < NumItemsInCart; i++) {
            fprintf(cartFile, "%d %d\n%s\n%s\n%d %.2f\n\n", cartItems[i].productID, cartItems[i].sellerID,
                    cartItems[i].itemName, cartItems[i].category, cartItems[i].quantity, cartItems[i].unitPrice);
        }
        fclose(cartFile);
    }
}

/***********************************************************************************************************
    checkOutMenu  allows the user to check out items in the cart and perform various actions based 
    on user input

    @param  cartItems - array of CartItemType representing the items in the cart
    @param  numItemsInCart - pointer to an integer representing the number of items in the cart
    @param  items - array of itemType representing the available items in the store
    @param  NumItems - pointer to an integer representing the number of available items
    @param  Transacs - array of transactionType representing the transactions
    @param  userID - integer representing the user ID
    @param  Users - array of userType representing the user information
    @param  NumUsers - integer representing the number of users
    @return void

    Pre-condition: cartItems, items, Transacs, and Users arrays are properly initialized with valid 
                   values, numItemsInCart, NumItems, and NumUsers are valid pointers to integers
************************************************************************************************************/
void 
checkOutMenu(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems, transactionType Transacs[], int userID, userType Users[], int NumUsers)
{
    int checkOutOption;
    int i, j;
    int itemID, quantity;
    float price, updatedPrice;
    int NumTransacs = 0;

    if (*numItemsInCart == 0) {
        printf("\n\t\t___________________________________________________\n");
        printf("\n\t\t   No items in cart. Redirecting to Buy Menu...."     );
        printf("\n\t\t___________________________________________________\n");
        return;
    }

    char day[3], month[3], year[5];
    printf("\t\tEnter date (MM/DD/YYYY): ");
    scanf("%2s/%2s/%4s", month, day, year);

    // one final check is done with the availability 
    for (i = 0; i < *numItemsInCart; i++) {
        itemID = cartItems[i].productID;
        quantity = cartItems[i].quantity;
        price = cartItems[i].unitPrice;
    
        j = 0;
        while (j < *NumItems){
            if (items[j].productID == itemID) {
                if (items[j].quantityAvailable > quantity && items[j].unitPrice != price) {
                    printf("\n\t\tSome items in the cart have been updated. Please review the changes.\n");

                    printf("\n\t\tItem ID: %d\n", itemID);
                    printf("\t\tOld Price: $%.2f\n", price);
                    printf("\t\tNew Price: $%.2f\n", items[j].unitPrice);

                    printf("\t\tYou can still go to Edit Cart to make further modifications.\n");

                    updatedPrice = items[j].unitPrice;
                    cartItems[i].unitPrice = updatedPrice;
                }
                else if (quantity > items[j].quantityAvailable && items[j].unitPrice == price) {
                    printf("\t\tItem: %s\n", items[j].itemName);
                    printf("\t\tQuantity you wished to check out: %d\n", quantity);
                    printf("\t\tStock available: %d\n", items[j].quantityAvailable);
                    printf("\t\tPlease update the quantity of the product to be checked out in Edit Cart.\n");
                    printf("\t\tRedirecting back to Buy Menu....\n");
                    return;
                }
            }
            j++; 
        }
    }
    
    do {
        countTransactions(&NumTransacs);

        printf("                _________________________________________________________\n");
        printf("\n");
        printf("                                 ~~~ CHECK OUT MENU ~~~\n");
        printf("                _________________________________________________________\n");
        printf("               |                        OPTIONS                          |\n");
        printf("               |                                                         |\n");
        printf("               |     [1] Check Out All Items in Cart                     |\n");
        printf("               |     [2] Check Out Items by a Specific Seller            |\n");
        printf("               |     [3] Check Out a Specific Item                       |\n");
        printf("               |     [4] Exit Check Out Menu                             |\n");
        printf("               |_________________________________________________________|\n\n");

        printf("\t\tEnter option: ");
        scanf("%d", &checkOutOption);

        switch (checkOutOption) 
        {
            case 1:
                checkOutAll(cartItems, numItemsInCart, items, NumItems, Transacs, day, month, year, userID, Users, NumUsers, &NumTransacs);
                break;
            
            case 2:
                checkOutSpecificSeller(cartItems, numItemsInCart, items, NumItems, Transacs, day, month, year, userID, Users, NumUsers, &NumTransacs);
                break;
        
            case 3:
                checkOutSpecificItem(cartItems, numItemsInCart, items, NumItems, Transacs, day, month, year, userID, Users, NumUsers, &NumTransacs);
                break;
        
            case 4:
                // Exit Check Out Menu
                return;
                break;
            
            default: 
                printf("\t\tInvalid option. Please enter a valid option (1-4).\n");
                break;
        
        }

        if (*numItemsInCart == 0) {
        printf("\n\t\t____________________________________________________\n");
        printf("\n\t\t No items more in cart. Redirecting to Buy Menu...."     );
        printf("\n\t\t____________________________________________________\n");
        return;
    }

    } while (checkOutOption != 4);
    
}

/***********************************************************************************************************
    checkOutAll processes the items in the cart and creates transactions for each unique seller ID

    @param  cartItems - an array of CartItemType representing the items in the cart
    @param  numItemsInCart - a pointer to an integer representing the number of items in the cart
    @param  items - an array of itemType representing the available items
    @param  NumItems - a pointer to an integer representing the number of available items
    @param  Transacs - an array of transactionType representing the transactions
    @param  day - a string representing the day of the transaction
    @param  month - a string representing the month of the transaction
    @param  year - a string representing the year of the transaction
    @param  userID - an integer representing the user ID of the buyer
    @param  Users - an array of userType representing the users
    @param  NumUsers - an integer representing the number of users
    @param  NumTransacs - a pointer to an integer representing the number of transactions
    @return void

    Pre-condition: cartItems array contains valid CartItemType objects, items array contains valid 
                   itemType objects, userID is a valid user ID, and Users array contains valid userType 
                   objects with valid userID values
************************************************************************************************************/
void 
checkOutAll(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems, transactionType Transacs[], char day[], char month[], char year[], int userID, userType Users[], int NumUsers, int *NumTransacs)
{
    int sellerID;
    int numTransacs = 0;
    int transactIndex = *NumTransacs; 
    int itemIndex, itemUpdated;
    int tempIndex = *NumTransacs;
    int i, j;
    int transacNo;
    char buyerName[21];

    while (*numItemsInCart > 0)
        {
        sellerID = cartItems[0].sellerID;  
        itemIndex = 0;      

        // Create a new transaction for each unique seller ID
        Transacs[transactIndex].buyerID = userID;
        Transacs[transactIndex].sellerID = sellerID;
        Transacs[transactIndex].totalAmount = 0;
        strcpy(Transacs[transactIndex].date.day, day);
        strcpy(Transacs[transactIndex].date.month, month);
        strcpy(Transacs[transactIndex].date.year, year);

        int itemsAdded = 0; 

        for (i = 0; i < *numItemsInCart && itemsAdded < 5; i++) {
            if (cartItems[i].sellerID == sellerID) {

                // Store item information in the transaction
                Transacs[transactIndex].itemsTransac[itemIndex].productID = cartItems[i].productID;
                strcpy(Transacs[transactIndex].itemsTransac[itemIndex].itemName, cartItems[i].itemName);
                Transacs[transactIndex].itemsTransac[itemIndex].quantityAvailable = cartItems[i].quantity;
                Transacs[transactIndex].itemsTransac[itemIndex].unitPrice = cartItems[i].unitPrice;
                Transacs[transactIndex].totalAmount += cartItems[i].quantity * cartItems[i].unitPrice;

                itemUpdated = 0; 

                for (j = 0; j < *NumItems && !itemUpdated; j++) {
                    if (items[j].productID == cartItems[i].productID) {
                        items[j].quantityAvailable -= cartItems[i].quantity;
                        itemUpdated = 1;
                    }
                }

                itemIndex++;
                itemsAdded++;

                for (j = i; j < (*numItemsInCart - 1); j++) {
                    cartItems[j] = cartItems[j + 1];
                }

                (*numItemsInCart)--;
                i--;
            }
        }

        Transacs[transactIndex].numItems = itemIndex;      
        numTransacs++;           
        transactIndex++;
    }


    // Display transaction summary
    printf("\n\n");
    printf("\tTransaction Summary:\n");

    for (i = 0; i < numTransacs; i++)
    {
        printf("\t====================================================================================\n");
        printf("\tTransaction %d\n", i + 1);
        printf("\t------------------------------------------------------------------------------------\n");
        printf("\t| Quantity | Product ID |       Item Name      | Unit Price | Total Price for Item |\n");
        printf("\t------------------------------------------------------------------------------------\n");

        for (j = 0; j < Transacs[tempIndex].numItems; j++)
        {
            printf("\t| %-9d| %-11d| %-21s|   %-9.2f|   %-17.2f  |\n",
                Transacs[tempIndex].itemsTransac[j].quantityAvailable,
                Transacs[tempIndex].itemsTransac[j].productID,
                Transacs[tempIndex].itemsTransac[j].itemName,
                Transacs[tempIndex].itemsTransac[j].unitPrice,
                (Transacs[tempIndex].itemsTransac[j].unitPrice * Transacs[tempIndex].itemsTransac[j].quantityAvailable));
        }

        printf("\t------------------------------------------------------------------------------------\n");
        printf("\t Total Amount Due: Php %.2f                                                         \n", Transacs[tempIndex].totalAmount);
        printf("\t Payable to Seller ID: %d                                                           \n", Transacs[tempIndex].sellerID);
        printf("\t Seller Name: %s                                                                    \n", getSellerName(Transacs[tempIndex].sellerID, items, *NumItems, Users, NumUsers));
        printf("\t------------------------------------------------------------------------------------\n");
        printf("\n");
        printf("\t Transaction Complete! Your Items will be delivered in 2-5 business days.\n");
        printf("\n");
        printf("\t====================================================================================\n");
        printf("\n");
        tempIndex++;
    }

    tempIndex = *NumTransacs;
    transacNo = (*NumTransacs + 1);
    strcpy(buyerName, getBuyerName(userID, Users, NumUsers));
    
    // Add transaction information to Transactions.txt
    FILE *fp = fopen("Transactions.txt", "a");
    if (fp != NULL)
    {
        for (int i = 0; i < numTransacs; i++)
        {   
            fprintf(fp, "Transaction No. %d\n", transacNo);
            fprintf(fp, "Date: %s/%s/%s\n", Transacs[tempIndex].date.month, Transacs[tempIndex].date.day, Transacs[tempIndex].date.year);
            fprintf(fp, "Items Bought: ");
            
            for (j = 0; j < Transacs[tempIndex].numItems; j++) 
                fprintf(fp, "%s; ", Transacs[tempIndex].itemsTransac[j].itemName);

            fprintf(fp, "\nBuyer ID: %d\n", Transacs[tempIndex].buyerID);
            fprintf(fp, "Buyer Name: %s\n", buyerName);
            fprintf(fp, "Seller ID: %d\n", Transacs[tempIndex].sellerID);
            fprintf(fp, "Seller Name: %s\n", getSellerName(Transacs[tempIndex].sellerID, items, *NumItems, Users, NumUsers));
            fprintf(fp, "Total Amount: %.2f\n", Transacs[tempIndex].totalAmount);
            fprintf(fp, "\n\n");
            tempIndex++;
            transacNo++;
        }
        fclose(fp);
    }
   
    return;
}

/***********************************************************************************************************
    checkOutSpecificSeller performs a checkout for a specific seller

    @param  cartItems - an array of CartItemType representing the items in the cart
    @param  numItemsInCart - a pointer to an integer representing the number of items in the cart
    @param  items - an array of itemType representing the available items
    @param  NumItems - a pointer to an integer representing the number of available items
    @param  Transacs - an array of transactionType representing the transactions
    @param  day - a string representing the day of the transaction
    @param  month - a string representing the month of the transaction
    @param  year - a string representing the year of the transaction
    @param  userID - an integer representing the user ID of the buyer
    @param  Users - an array of userType representing the users
    @param  NumUsers - an integer representing the number of users
    @param  NumTransacs - a pointer to an integer representing the number of transactions
    @return void

    Pre-condition: cartItems array contains valid CartItemType objects, items array contains valid 
                   itemType objects, userID is a valid user ID, and Users array contains valid userType 
                   objects with valid userID values
************************************************************************************************************/
void 
checkOutSpecificSeller(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems, transactionType Transacs[], char day[], char month[], char year[], int userID, userType Users[], int NumUsers, int *NumTransacs)
{
    int sellerID, enteredSellerID;
    int numTransacs = 1;
    int transactIndex = *NumTransacs;
    int tempIndex = *NumTransacs;
    int itemIndex, itemUpdated, sellerIDExists;
    int i, j;
    int transacNo;
    char buyerName[21];

    printf("\t\tEnter seller ID: ");
    scanf("%d", &enteredSellerID);

    // Check if the seller ID entered by the user is valid
    sellerIDExists = 0; 
    for (i = 0; i < *NumItems && !sellerIDExists; i++) {
        if (items[i].sellerID == enteredSellerID) {
            sellerIDExists = 1;
        }
    }

    if (!sellerIDExists) {
        printf("\t\tInvalid seller ID. Redirecting back to Check Out Menu....\n");
        return;
    }

    itemIndex = 0;
    if (sellerIDExists) {
        for (i = 0; i < *numItemsInCart; i++) {

            sellerID = enteredSellerID;

            if (cartItems[i].sellerID == sellerID) {

                Transacs[transactIndex].buyerID = userID;
                Transacs[transactIndex].sellerID = sellerID;
                strcpy(Transacs[transactIndex].date.day, day);
                strcpy(Transacs[transactIndex].date.month, month);
                strcpy(Transacs[transactIndex].date.year, year);
                Transacs[transactIndex].itemsTransac[itemIndex].productID = cartItems[i].productID;
                strcpy(Transacs[transactIndex].itemsTransac[itemIndex].itemName, cartItems[i].itemName);
                Transacs[transactIndex].itemsTransac[itemIndex].quantityAvailable = cartItems[i].quantity;
                Transacs[transactIndex].itemsTransac[itemIndex].unitPrice = cartItems[i].unitPrice;
                Transacs[transactIndex].totalAmount += cartItems[i].quantity * cartItems[i].unitPrice;

                itemUpdated = 0; 

                for (j = 0; j < *NumItems && !itemUpdated; j++) {
                    if (items[j].productID == cartItems[i].productID) {
                        items[j].quantityAvailable -= cartItems[i].quantity;
                        itemUpdated = 1;
                    }
                }

                itemIndex++;

                // Remove item from cart by shifting remaining items
                for (j = i; j < (*numItemsInCart - 1); j++) {
                    cartItems[j] = cartItems[j + 1];
                }

                (*numItemsInCart)--;
                i--;

            }

            Transacs[transactIndex].numItems = itemIndex;

            // Limit each transaction to a maximum of 5 different items
            if (itemIndex >= 5) {
                numTransacs++;
                transactIndex++;
                itemIndex = 0;
            }
        }
    }

    // Display transaction summary
    printf("\n");
    for (i = 0; i < numTransacs; i++)
    {
        printf("\t====================================================================================\n");
        printf("\tTransaction Summary:\n");
        printf("\t------------------------------------------------------------------------------------\n");
        printf("\t| Quantity | Product ID |       Item Name      | Unit Price | Total Price for Item |\n");
        printf("\t------------------------------------------------------------------------------------\n");

        for (j = 0; j < Transacs[tempIndex].numItems; j++)
        {
            printf("\t| %-9d| %-11d| %-21s|   %-9.2f|   %-17.2f  |\n",
                Transacs[tempIndex].itemsTransac[j].quantityAvailable,
                Transacs[tempIndex].itemsTransac[j].productID,
                Transacs[tempIndex].itemsTransac[j].itemName,
                Transacs[tempIndex].itemsTransac[j].unitPrice,
                (Transacs[tempIndex].itemsTransac[j].unitPrice * Transacs[tempIndex].itemsTransac[j].quantityAvailable));
        }

        printf("\t------------------------------------------------------------------------------------\n");
        printf("\t Total Amount Due: Php %.2f                                                         \n", Transacs[tempIndex].totalAmount);
        printf("\t Payable to Seller ID: %d                                                           \n", Transacs[tempIndex].sellerID);
        printf("\t Seller Name: %s                                                                    \n", getSellerName(Transacs[tempIndex].sellerID, items, *NumItems, Users, NumUsers));
        printf("\t------------------------------------------------------------------------------------\n");
        printf("\n");
        printf("\t Transaction Complete! Your Items will be delivered in 2-5 business days.\n");
        printf("\n");
        printf("\t====================================================================================\n");
        printf("\n");

        tempIndex++;
    }

    tempIndex = *NumTransacs;
    transacNo = (*NumTransacs + 1);
    strcpy(buyerName, getBuyerName(userID, Users, NumUsers));

    // Add transaction information to Transactions.txt
    FILE *fp = fopen("Transactions.txt", "a");
    if (fp != NULL)
    {
        for (int i = 0; i < numTransacs; i++)
        {
            fprintf(fp, "Transaction No. %d\n", transacNo);
            fprintf(fp, "Date: %s/%s/%s\n", Transacs[tempIndex].date.month, Transacs[tempIndex].date.day, Transacs[tempIndex].date.year);
            fprintf(fp, "Items Bought: ");
            
            // Print items bought
            for (j = 0; j < Transacs[tempIndex].numItems; j++) 
                fprintf(fp, "%s; ", Transacs[tempIndex].itemsTransac[j].itemName);

            fprintf(fp, "\nBuyer ID: %d\n", Transacs[tempIndex].buyerID);
            fprintf(fp, "Buyer Name: %s\n", buyerName);
            fprintf(fp, "Seller ID: %d\n", Transacs[tempIndex].sellerID);
            fprintf(fp, "Seller Name: %s\n", getSellerName(Transacs[tempIndex].sellerID, items, *NumItems, Users, NumUsers));
            fprintf(fp, "Total Amount: %.2f\n", Transacs[tempIndex].totalAmount);
            fprintf(fp, "\n\n");
            tempIndex++;
            transacNo++;
        }
        fclose(fp);
    }
   
    return;
}

/***********************************************************************************************************
    checkOutSpecificItem - Processes a specific item checkout in a shopping car

    @param  cartItems - an array of CartItemType representing the items in the cart
    @param  numItemsInCart - a pointer to an integer representing the number of items in the cart
    @param  items - an array of itemType representing the available items
    @param  NumItems - a pointer to an integer representing the number of available items
    @param  Transacs - an array of transactionType representing the transactions
    @param  day - a string representing the day of the transaction
    @param  month - a string representing the month of the transaction
    @param  year - a string representing the year of the transaction
    @param  userID - an integer representing the user ID of the buyer
    @param  Users - an array of userType representing the users
    @param  NumUsers - an integer representing the number of users
    @param  NumTransacs - a pointer to an integer representing the number of transactions
    @return void

    Pre-condition: cartItems array contains valid CartItemType objects, items array contains valid 
                   itemType objects, userID is a valid user ID, and Users array contains valid userType 
                   objects with valid userID values
************************************************************************************************************/
void 
checkOutSpecificItem(CartItemType cartItems[], int *numItemsInCart, itemType items[], int *NumItems, transactionType Transacs[], char day[], char month[], char year[], int userID, userType Users[], int NumUsers, int *NumTransacs)
{
    int itemID, quantity, itemIndex, itemUpdated;
    int i, j;
    int transactIndex = *NumTransacs;
    int numTransacs = 0;
    int found = 0;
    char sellerName[21];
    char buyerName[21];

    printf("\t\tEnter Product ID: ");
    scanf("%d", &itemID);

    // Check if the item ID exists in the cart
    for (i = 0; i < *numItemsInCart && !found; i++) {
        if (cartItems[i].productID == itemID) {
            quantity = cartItems[i].quantity;
            found = 1;
        }
    }

    if (!found) {
        printf("\t\tItem not found in the cart. Redirecting back to Check Out Menu...\n");
        return;
    }

    if (found)
    {
        itemIndex = 0;

        Transacs[transactIndex].buyerID = userID;
        Transacs[transactIndex].sellerID = cartItems[i-1].sellerID;
        strcpy(Transacs[transactIndex].date.day, day);
        strcpy(Transacs[transactIndex].date.month, month);
        strcpy(Transacs[transactIndex].date.year, year);
        Transacs[transactIndex].itemsTransac[itemIndex].productID = cartItems[i-1].productID;
        strcpy(Transacs[transactIndex].itemsTransac[itemIndex].itemName, cartItems[i-1].itemName);
        Transacs[transactIndex].itemsTransac[itemIndex].quantityAvailable = cartItems[i-1].quantity;
        Transacs[transactIndex].itemsTransac[itemIndex].unitPrice = cartItems[i-1].unitPrice;
        Transacs[transactIndex].totalAmount += cartItems[i-1].quantity * cartItems[i-1].unitPrice;

        Transacs[transactIndex].numItems++; 
        numTransacs++; 
    }


    itemUpdated = 0;

    for (j = 0; j < *NumItems && !itemUpdated; j++) {
        if (items[j].productID == cartItems[i-1].productID) {
            items[j].quantityAvailable -= quantity;
            itemUpdated = 1; 
        }
    }

    // Remove item from cart by shifting remaining items
    for (j = i-1; j < (*numItemsInCart - 1); j++) {
        cartItems[j] = cartItems[j+1];
    }

    (*numItemsInCart)--; 

    // Display transaction summary
    printf("\n");
    printf("\t====================================================================================\n");
    printf("\tTransaction Summary:\n");
    printf("\t------------------------------------------------------------------------------------\n");
    printf("\t| Quantity | Product ID |       Item Name      | Unit Price | Total Price for Item |\n");
    printf("\t------------------------------------------------------------------------------------\n");

    printf("\t| %-9d| %-11d| %-21s|   %-9.2f|   %-17.2f  |\n",
        Transacs[transactIndex].itemsTransac[0].quantityAvailable,
        Transacs[transactIndex].itemsTransac[0].productID,
        Transacs[transactIndex].itemsTransac[0].itemName,
        Transacs[transactIndex].itemsTransac[0].unitPrice,
        (Transacs[transactIndex].itemsTransac[0].unitPrice * Transacs[transactIndex].itemsTransac[0].quantityAvailable));

    printf("\t------------------------------------------------------------------------------------\n");
    printf("\t Total Amount Due: Php %.2f                                                         \n", Transacs[transactIndex].totalAmount);
    printf("\t Payable to Seller ID: %d                                                           \n", Transacs[transactIndex].sellerID);
    printf("\t Seller Name: %s                                                                    \n", getSellerName(Transacs[transactIndex].sellerID, items, *NumItems, Users, NumUsers));
    printf("\t------------------------------------------------------------------------------------\n");
    printf("\n");
    printf("\t Transaction Complete! Your Items will be delivered in 2-5 business days.\n");
    printf("\n");
    printf("\t====================================================================================\n");
    printf("\n");

    strcpy(sellerName, getSellerName(Transacs[transactIndex].sellerID, items, *NumItems, Users, NumUsers));
    strcpy(buyerName, getBuyerName(userID, Users, NumUsers));

    // Add transaction information to Transactions.txt
    FILE *fp = fopen("Transactions.txt", "a");
    if (fp != NULL)
    {
        fprintf(fp, "Transaction No. %d\n", (*NumTransacs + 1));
        fprintf(fp, "Date: %s/%s/%s\n", Transacs[transactIndex].date.month, Transacs[transactIndex].date.day, Transacs[transactIndex].date.year);
        fprintf(fp, "Items Bought: ");

        // Print items bought
        fprintf(fp, "%s; ", Transacs[transactIndex].itemsTransac[0].itemName);

        fprintf(fp, "\nBuyer ID: %d\n", Transacs[transactIndex].buyerID);
        fprintf(fp, "Buyer Name: %s\n", buyerName);
        fprintf(fp, "Seller ID: %d\n", Transacs[transactIndex].sellerID);
        fprintf(fp, "Seller Name: %s\n", sellerName);
        fprintf(fp, "Total Amount: %.2f\n", Transacs[transactIndex].totalAmount);
        fprintf(fp, "\n\n");

        fclose(fp);
    }
    return;
}

/***********************************************************************************************************
    getSellerName retrieves the name of the seller with the given sellerID from the arrays of 
    items and users

    @param  sellerID - the ID of the seller whose name is to be retrieved
    @param  items - array of items containing sellerID and other item details
    @param  numItems - the number of items in the items array
    @param  users - array of users containing userID, name, and other user details
    @param  numUsers - the number of users in the users array
    @return the name of the seller with the given sellerID if found, otherwise "Unknown"

    Pre-condition: items and users arrays have valid data & sellerID, userID, and name are 
                   valid and unique
************************************************************************************************************/
char* 
getSellerName(int sellerID, itemType items[], int numItems, userType users[], int numUsers)
{
    for (int i = 0; i < numItems; i++)
    {
        if (items[i].sellerID == sellerID)
        {
            for (int j = 0; j < numUsers; j++)
            {
                if (users[j].userID == sellerID)
                {
                    return users[j].name;
                }
            }
        }
    }
    return "Unknown";
}

/***********************************************************************************************************
    getBuyerName retrieves the name of the buyer with the given buyerID from the array of users

    @param  buyerID - the ID of the buyer whose name is to be retrieved
    @param  Users - array of users containing userID, name, and other user details
    @param  NumUsers - the number of users in the Users array
    @return the name of the buyer with the given buyerID if found, otherwise "Unknown"

    Pre-condition: Users array  has valid data & buyerID and userID are valid and unique
************************************************************************************************************/
char* 
getBuyerName(int buyerID, userType Users[], int NumUsers)
{
    for (int i = 0; i < NumUsers; i++)
    {
        if (Users[i].userID == buyerID)
        {
            return Users[i].name;
        }
    }
    return "Unknown";
}

/***********************************************************************************************************
    saveData saves the user and item data to respective text files

    @param  Users - array of userType containing user data to be saved
    @param  NumUsers - the number of users in the Users array
    @param  Items - array of itemType containing item data to be saved
    @param  NumItems - the number of items in the Items array
    @return void

    Pre-condition: Users and Items arrays have valid data & User and item data is properly formatted
************************************************************************************************************/
void 
saveData(userType* Users, int NumUsers, itemType* Items, int NumItems)
{
    printf("\n\t\tSaving user data...\n");
    FILE *userFile = fopen("Users.txt", "w");
    if (userFile != NULL){
        for (int i = 0; i < NumUsers; i++){
            fprintf(userFile, "%d %s\n%s\n%s\n%lld\n\n", Users[i].userID, Users[i].password, 
                    Users[i].name, Users[i].address, Users[i].contactNum);
        }
        fclose(userFile);
    }

    printf("\t\tSaving item data...\n");
    FILE *itemFile = fopen("Items.txt", "w");
    if (itemFile != NULL){
        for (int i = 0; i < NumItems; i++){
            fprintf(itemFile, "%d %d\n%s\n%s\n%s\n%d %.2f\n\n", Items[i].productID, Items[i].sellerID,
                    Items[i].itemName, Items[i].category, Items[i].description, Items[i].quantityAvailable, Items[i].unitPrice);
    }
        fclose(itemFile);
    }

    printf("\t\tExiting application...\n");
    printf("\t\tThank you for using Shopee!\n\n");
}


