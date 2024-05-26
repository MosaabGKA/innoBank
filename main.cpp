#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <time.h>
#include <windows.h>
using namespace std;

string excess;
#define remove_stdin_excess getline(cin, excess)

struct account {
    string firstName;
    string lastName;
    string address;
    string phoneNumber;
    int age = 0;
    double balance = 0;
};

void changeColor(int color);

void showMainMenu();

void addNewAccount();

struct account getAccountInfo(int charsBefore);

void displayAccount();

int accountSearch(string name, bool show);

int maxLineLen(string fileName);

void updateAccount();

void deleteAccount(int charsBefore);

void deletionProcess();

void transferMoney();

int main() {
    changeColor(0x84);
    cout << "Welcome to innoBank Digital Management System!" << endl;
    cout << "(c) 2024: Mosaab - Eyad - A. Fahmy - A. Shaban" << endl;
    showMainMenu();
    return 0;
}

void showMainMenu() {
    changeColor(0x2);
    cout << "\nMAIN MENU\nChoose action:\n1) Add New Account\n2) Display Account Details\n";
    cout << "3) Update Existing Account\n4) Transfer Money\n5) Delete Account\n0) Exit" << endl;
    char operation;
    cin >> operation;
    remove_stdin_excess;
    switch (operation) {
        case '1':
            addNewAccount();
            break;
        case '2':
            displayAccount();
            break;
        case '3':
            updateAccount();
            break;
        case '4':
            transferMoney();
            break;
        case '5':
            deletionProcess();
            break;
        case '0':
            changeColor(0x7);
            exit(0);
        default:
            cout << operation << "is invalid choice!" << endl;
            showMainMenu();
    }
}

void addNewAccount() {
    changeColor(0x1);
    cout << "\nADDING NEW ACCOUNT\nConfirm you want to add new account: (y/n)" << endl;
    char confirm;
    cin >> confirm;
    remove_stdin_excess;
    if (confirm == 'n' || confirm == 'N') showMainMenu();
    else if (confirm != 'y' && confirm != 'Y') {
        cout << "Wrong choice." << endl;
        addNewAccount();
    }
    struct account newAccount;
    cout << "Enter account holder's first name:\n";
    getline(cin, newAccount.firstName);
    cout << "Enter account holder's last name:\n";
    getline(cin, newAccount.lastName);
    cout << "Enter account holder's address:\n";
    getline(cin, newAccount.address);
    cout << "Enter account holder's phone number:\n";
    getline(cin, newAccount.phoneNumber);
    cout << "Enter account holder's age:\n";
    cin >> newAccount.age;
    while (newAccount.age < 18) {
        cout << "You Can't Open Account Because You Are Less Than 18\n";
        cout << "1) Add Another Person\n0) Back main menu\n";
        char choice;
        cin >> choice;
        remove_stdin_excess;
        if (choice == '1') addNewAccount();
        else if (choice == '0') showMainMenu();
        else cout << "Invalid Operation\n";
    }
    cout << "Enter the initial balance of the account:\n";
    cin >> newAccount.balance;
    FILE *accounts = fopen("accountList.txt", "a");
    fprintf(accounts, "%s\t%s\t%s\t%s\t%d\t%.2lf\n", &newAccount.firstName[0], &newAccount.lastName[0],
            &newAccount.address[0], &newAccount.phoneNumber[0], newAccount.age, newAccount.balance);
    fclose(accounts);
    cout << "Account added successfully!\nPress Enter to get back to main menu.\n";
    getch();
    showMainMenu();
}

int maxLineLen(string fileName) {
    int mx = 0, len = 1;
    char c;
    FILE *file = fopen(&fileName[0], "r");
    while ((c = fgetc(file)) != EOF) {
        len++;
        if (c == '\n') {
            mx = max(mx, len);
            len = 1;
        }
    }
    fclose(file);
    return mx;
}

struct account getAccountInfo(int charsBefore) {
    struct account acc;
    FILE *accounts = fopen("accountList.txt", "r");
    fseek(accounts, charsBefore, SEEK_SET);
    char c = fgetc(accounts);
    acc.firstName += c;
    while ((c = fgetc(accounts)) != '\t')
        acc.firstName += c;
    while ((c = fgetc(accounts)) != '\t')
        acc.lastName += c;
    while ((c = fgetc(accounts)) != '\t')
        acc.address += c;
    while ((c = fgetc(accounts)) != '\t')
        acc.phoneNumber += c;
    while ((c = fgetc(accounts)) != '\t') {
        acc.age *= 10;
        acc.age += c - '0';
    }
    while ((c = fgetc(accounts)) != '.' && c != '\n') {
        acc.balance *= 10;
        acc.balance += c - '0';
    }
    int frac = 1;
    while (c != '\n' && (c = fgetc(accounts)) != '\n') {
        frac *= 10;
        acc.balance += double(c - '0') / frac;
    }
    fclose(accounts);
    return acc;
}

int accountSearch(string name, bool show) {
    FILE *accounts = fopen("accountList.txt", "r");
    int maxLineLength = maxLineLen("accountList.txt"), charsBefore = 0;
    char line[maxLineLength]{};
    while (fgets(line, maxLineLength, accounts)) {
        bool found = true;
        for (int i = 0; i < name.size(); i++) {
            if (name[i] != line[i] && (line[i] != '\t' && name[i] != ' ')) {
                found = false;
                break;
            }
        }
        if (found && show) {
            changeColor(0x4);
            cout << "User Found!\n";
            string labels[]{
                "First Name: ", "\nLast Name: ", "\nAddress: ",
                "\nPhone Number: ", "\nAge: ", "\nBalance: "
            };
            changeColor(0x3);
            auto ptr = labels;
            cout << *ptr++;
            for (auto c: line) {
                if (c == '\t') {
                    cout << *ptr++;
                } else {
                    cout << c;
                    if (c == '\n') break;
                }
            }
            fclose(accounts);
            return charsBefore;
        } else if(found) {
            fclose(accounts);
            return charsBefore;
        }
        charsBefore += (int) strlen(line) + 1;
    }
    changeColor(0x4);
    cout << "User Not Found!\n";
    fclose(accounts);
    return -1;
}

void displayAccount() {
    changeColor(0x0E);
    cout <<
            "\nACCOUNT DISPLAY SYSTEM\nChoose:\n1) Display All Accounts\n"
            "2) Display Details of Specific Account\n0) Back to main menu\n";
    char option;
    cin >> option;
    remove_stdin_excess;
    if (option == '0') {
        showMainMenu();
    } else if (option == '1') {
        cout << "Please make program window in FULL SCREEN mode...then press enter\n";
        getchar();
        changeColor(0xD);
        cout <<
                "[First Name]       "
                "[Last Name]        "
                "[Address]          "
                "[Phone Number]     "
                "[Age]              "
                "[Balance]\n";
        int maxLineLength = maxLineLen("accountList.txt");
        char line[maxLineLength]{};
        FILE *accounts = fopen("accountList.txt", "r");
        while (fgets(line, maxLineLength, accounts)) {
            int chars = 0;
            changeColor(0x3);
            for (char c: line) {
                if (c == '\n') {
                    cout << c;
                    break;
                }
                if (c == '\t') {
                    while (++chars % 20)
                        cout << ' ';
                    continue;
                }
                cout << c;
                chars++;
            }
        }
        fclose(accounts);
        showMainMenu();
    } else if (option == '2') {
        cout << "Enter the name of account's holder:\n";
        string name;
        getline(cin, name);
        if (accountSearch(name, 1) == -1) displayAccount();
        else showMainMenu();
    } else {
        cout << "Wrong Choice!\n";
        displayAccount();
    }
}

void updateAccount() {
    changeColor(0x9);
    cout <<
            "ACCOUNT MODIFYING SYSTEM\nEnter full name of the account holder you want to modify or (0) to back to main menu:\n";
    string name;
    getline(cin, name);
    if (name == "0") showMainMenu();
    int charsBefore = accountSearch(name, 1);
    if (charsBefore == -1) updateAccount();
    struct account acc = getAccountInfo(charsBefore);
    changeColor(0x4);
    cout << "Confirm you want to modify the account details of ";
    changeColor(0x3);
    cout << acc.firstName;
    changeColor(0x4);
    cout << ": (y/n)\n";
    changeColor(0x9);
    char confirm;
    cin >> confirm;
    remove_stdin_excess;
    while (confirm != 'y' && confirm != 'Y' && confirm != 'n' && confirm != 'N') {
        cout << "Wrong Choice!\n";
        cout << "Confirm you want to modify " << acc.firstName << "'s account: (y/n)\n";
        cin >> confirm;
        remove_stdin_excess;
    }
    if (confirm == 'n' || confirm == 'N') showMainMenu();
    FILE *accounts = fopen("accountList.txt", "r");
    FILE *accountsTemp = fopen("accountTempList.txt", "w");
    for (int i = 1; i < charsBefore; i++) {
        char c = fgetc(accounts);
        fputc(c, accountsTemp);
    }
    changeColor(0x3);
    cout << "EDITING INFO FOR: " << acc.firstName << '\n';
    changeColor(0x9);
    cout <<
            "Choose info to edit:\n1) First Name\n2) Last Name\n"
            "3) Address\n4) Phone Number\n5) Age\n6) Balance\n0) Back to main menu\n";
    int choice;
    cin >> choice;
    remove_stdin_excess;
    if (choice == 0) {
        fclose(accounts);
        fclose(accountsTemp);
        showMainMenu();
    }
    string update;
    cout << "Updated info:\n";
    cin >> update;
    if (choice == 1) acc.firstName = update;
    else if (choice == 2) acc.lastName = update;
    else if (choice == 3) acc.address = update;
    else if (choice == 4) acc.phoneNumber = update;
    else if (choice == 5) acc.age = stoi(update);
    else if (choice == 6) acc.balance = stod(update);
    fprintf(accountsTemp, "%s\t%s\t%s\t%s\t%d\t%.2lf\n", &acc.firstName[0], &acc.lastName[0],
            &acc.address[0], &acc.phoneNumber[0], acc.age, acc.balance);
    char c;
    while ((c = fgetc(accounts)) != '\n' && c != EOF);
    while (c != EOF && (c = fgetc(accounts)) != EOF)
        fputc(c, accountsTemp);
    fclose(accounts);
    fclose(accountsTemp);
    FILE *newAccounts = fopen("accountList.txt", "w");
    FILE *newAccountsTemp = fopen("accountTempList.txt", "r");
    while ((c = fgetc(newAccountsTemp)) != EOF)
        fputc(c, newAccounts);
    fclose(accounts);
    fclose(accountsTemp);
    cout << acc.firstName << "'s updated successfully!\n";
    showMainMenu();
}

void deleteAccount(int charsBefore) {
    FILE *accounts = fopen("accountList.txt", "r");
    FILE *accountsTemp = fopen("accountTempList.txt", "w");
    for (int i = 1; i < charsBefore; i++) {
        char c = fgetc(accounts);
        fputc(c, accountsTemp);
    }
    char c;
    while ((c = fgetc(accounts)) != '\n' && c != EOF);
    // fputc(c, accountsTemp);
    while (c != EOF && (c = fgetc(accounts)) != EOF)
        fputc(c, accountsTemp);
    fclose(accounts);
    fclose(accountsTemp);
    FILE *newAccounts = fopen("accountList.txt", "w");
    FILE *newAccountsTemp = fopen("accountTempList.txt", "r");
    while ((c = fgetc(newAccountsTemp)) != EOF)
        fputc(c, newAccounts);
    fclose(newAccounts);
    fclose(newAccountsTemp);
}


void deletionProcess() {
    changeColor(0x4);
    cout << "ACCOUNT DELETION\nEnter full name of the account holder you want to delete or (0) to back main menu:\n";
    string name;
    getline(cin, name);
    if (name == "0") showMainMenu();
    int charsBefore = accountSearch(name, 1);
    if (charsBefore == -1) deletionProcess();
    struct account acc = getAccountInfo(charsBefore);
    char confirm;
    changeColor(0x4);
    cout << "Confirm you want to delete the account of " ;
    changeColor(0x9);
    cout << acc.firstName;
    changeColor(0x4);
    cout << ": (y/n)\n";
    cin >> confirm;
    while (confirm != 'y' && confirm != 'Y' && confirm != 'n' && confirm != 'N') {
        remove_stdin_excess;
        cout << "Wrong Choice!\n";
        cout << "Confirm you want to delete this account: (y/n)\n";
        cin >> confirm;
    }
    if (confirm == 'n' || confirm == 'N') showMainMenu();
    deleteAccount(charsBefore);
    changeColor(0x9);
    cout << acc.firstName << "'s";
    changeColor(0x4);
    cout << " account deleted successfully!\n";
    showMainMenu();
}

void transferMoney() {
    changeColor(0x5);
    cout << "MONEY TRANSFER\nEnter full name of person who will give the money or (0) to back to main menu:\n";
    string senderAccount, receiverAccount;
    getline(cin, senderAccount);
    if (senderAccount == "0") showMainMenu();
    int senderPosition = accountSearch(senderAccount, 1);
    while (senderPosition == -1) {
        changeColor(0x5);
        cout << "Enter full name of person who will give the money or (0) to back to main menu:\n";
        getline(cin, senderAccount);
        if (senderAccount[0] == '0') showMainMenu();
        senderPosition = accountSearch(senderAccount, 1);
    }
    cout << "Enter full name of person who will receive the money:\n";
    getline(cin, receiverAccount);
    int receiverPosition = accountSearch(receiverAccount, 1);
    while (receiverPosition == -1)
        receiverPosition = accountSearch(senderAccount, 1);
    if (senderPosition == receiverPosition) {
        changeColor(0x4);
        cout << "Cannot transfer from and to the same account!\n";
        transferMoney();
    }
    struct account sender = getAccountInfo(senderPosition), receiver = getAccountInfo(receiverPosition);
    senderAccount = sender.firstName, receiverAccount = receiver.firstName;
    char confirm;
    changeColor(0x4);
    cout << "Confirm you want to transfer money from ";
    changeColor(0x9);
    cout << senderAccount;
    changeColor(0x4);
    cout << " to ";
    changeColor(0x9);
    cout << receiverAccount;
    changeColor(0x4);
    cout << " (y/n):\n";
    cin >> confirm;
    remove_stdin_excess;
    while (confirm != 'y' && confirm != 'Y' && confirm != 'n' && confirm != 'N') {
        changeColor(0x4);
        cout << "Wrong Choice!\n";
        cout << "Confirm you want to transfer money from " << senderAccount << " to " << receiverAccount << " (y/n):\n";
        cin >> confirm;
        remove_stdin_excess;
    }
    if (confirm == 'n' || confirm == 'N') showMainMenu();
    double transferAmount;
    changeColor(0x5);
    cout << "Enter amount of money to transfer:\n";
    cin >> transferAmount;
    while (sender.balance - transferAmount < 0) {
        changeColor(0x4);
        cout << senderAccount << "'s balance is not sufficient (only " << sender.balance << ")\n";
        cout << "Enter lower amount (enter 0 to cancel):\n";
        cin >> transferAmount;
        if(transferAmount == 0) showMainMenu();
    }
    sender.balance -= transferAmount;
    receiver.balance += transferAmount;
    deleteAccount(senderPosition);
    receiverPosition = accountSearch(receiver.firstName, 0);
    deleteAccount(receiverPosition);
    FILE *accounts = fopen("accountList.txt", "a");
    fprintf(accounts, "%s\t%s\t%s\t%s\t%d\t%.2lf\n", &sender.firstName[0], &sender.lastName[0],
            &sender.address[0], &sender.phoneNumber[0], sender.age, sender.balance);
    fprintf(accounts, "%s\t%s\t%s\t%s\t%d\t%.2lf\n", &receiver.firstName[0], &receiver.lastName[0],
            &receiver.address[0], &receiver.phoneNumber[0], receiver.age, receiver.balance);
    fclose(accounts);
    FILE *logs = fopen("Logs.txt", "a");
    time_t t = time(NULL);
    struct tm *ptr = localtime(&t);
    string senderFullName = sender.firstName + ' ' + sender.lastName;
    string receiverFullName = receiver.firstName + ' ' + receiver.lastName;
    fprintf(
        logs, "%s --> transferring %.2lf from %s's account to %s's account; %s's new balance: %.2lf & %s's new balance: %.2lf\n\n",
        asctime(ptr), transferAmount, &senderFullName[0], &receiverFullName[0], &senderFullName[0], sender.balance,
        &receiverFullName[0], receiver.balance
    );
    fclose(logs);
    cout << "Transfer Complete!\n";
    showMainMenu();
}

void changeColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
