#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define True 1
#define False 0
#define CREDENTIALS_LENGTH 30

const char *ACCOUNT_FILE = "accounts.dat";
const char *USER_FILE = "users.dat";
int loggedin = False;
char current_user[50];

typedef struct
{
    char username[50];
    char password[50];
} User;

typedef struct
{
    int account_number;
    char account_holder[50];
    float balance;
} Account;

void sign_handler();
void login_handler();
void logout_handler();
void input_handler(char *username, char *password, char *typee);
void open_account();
void withdraw_handler();
void deposit_handler();
void change_password();
void view_account_details();
void fix_text_last(char *text);

int main()
{
    int choice;

    while (True)
    {
        printf("\nWelcome to the Banking System\n");
        printf("1. Sign In\n");
        printf("2. Log In\n");
        printf("3. Open Account\n");
        printf("4. Withdraw\n");
        printf("5. Deposit\n");
        printf("6. Check Account!\n");
        printf("7. Change Password\n");
        printf("8. Logout\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            sign_handler();
            break;
        case 2:
            login_handler();
            break;
        case 3:
            open_account();
            break;
        case 4:
            withdraw_handler();
            break;
        case 5:
            deposit_handler();
            break;
        case 6:
            view_account_details();
            break;
        case 7:
            change_password();
            break;
        case 8:
            logout_handler();
            break;
        case 9:
            printf("Exiting the Banking System. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

void sign_handler()
{
    if (!loggedin)
    {
        FILE *user_file = fopen(USER_FILE, "rb");
        if (user_file == NULL)
        {
            perror("Failed to open user file!");
            return;
        }

        char username[CREDENTIALS_LENGTH];
        char password[CREDENTIALS_LENGTH];
        input_handler(username, password, "signin");

        User new_user;

        while (fread(&new_user, sizeof(User), 1, user_file))
        {
            if (strcmp(new_user.username, username) == 0 && strcmp(new_user.password, password) == 0)
            {
                printf("User already exists!");
                fclose(user_file);
                return;
            }
        }
        fclose(user_file);

        user_file = fopen(USER_FILE, "ab");

        strncpy(new_user.username, username, CREDENTIALS_LENGTH);
        strncpy(new_user.password, password, CREDENTIALS_LENGTH);

        fwrite(&new_user, sizeof(User), 1, user_file);
        fclose(user_file);
    }
    else
    {
        printf("\nYou are already logged in with %s username!\n", current_user);
    }
}

void login_handler()
{
    if (!loggedin)
    {
        FILE *user_file = fopen(USER_FILE, "rb");
        if (user_file == NULL)
        {
            user_file = fopen(USER_FILE, "wb");
            if (user_file == NULL)
            {
                perror("Failed to open user file!");
                return;
            }

            fclose(user_file);

            user_file = fopen(USER_FILE, "rb");
        }

        char username[CREDENTIALS_LENGTH];
        char password[CREDENTIALS_LENGTH];
        input_handler(username, password, "login");

        User user;

        while (fread(&user, sizeof(User), 1, user_file))
        {
            if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0)
            {
                loggedin = True;
                printf("\nLogin successful! Welcome %s\n", user.username);
                strncpy(current_user, user.username, sizeof(current_user));
                fclose(user_file);
                return;
            }
        }

        printf("\nUser not found!\n");
        fclose(user_file);
    }
    else
    {
        printf("\nYou are already logged in with %s username!\n", current_user);
    }
}

void logout_handler()
{
    if (loggedin)
    {
        loggedin = False;
        printf("\nLogout Successfully!\n");
    }

    else
    {
        printf("\nYou are not login yet!\n");
    }
}

void input_handler(char *username, char *password, char *typee)
{
    if (strcmp(typee, "login") != 0 && strcmp(typee, "signin") != 0)
    {
        printf("\nEnter new password: ");
        fgets(password, CREDENTIALS_LENGTH, stdin);
        fix_text_last(password);
    }
    else
    {
        printf("\nEnter username: ");
        fgets(username, CREDENTIALS_LENGTH, stdin);
        fix_text_last(username);
        printf("Enter password: ");
        fgets(password, CREDENTIALS_LENGTH, stdin);
        fix_text_last(password);
    }
}

void open_account()
{
    if (loggedin)
    {
        FILE *account_file = fopen(ACCOUNT_FILE, "rb");

        if (account_file == NULL)
        {
            perror("Failed to open account file!");
            return;
        }

        Account new_account;

        int taccount_number;

        strncpy(new_account.account_holder, current_user, sizeof(new_account.account_holder));
        printf("\nEnter account number: ");
        scanf("%d", &taccount_number);

        while (fread(&new_account, sizeof(Account), 1, account_file))
        {
            if (new_account.account_number == taccount_number)
            {
                printf("\nThis account number already exists!\n");
                fclose(account_file);
                return;
            }
        }
        fclose(account_file);

        account_file = fopen(ACCOUNT_FILE, "ab");

        new_account.account_number = taccount_number;
        new_account.balance = 0.0; // Initial balance
        fwrite(&new_account, sizeof(Account), 1, account_file);
        fclose(account_file);
    }
    else
    {
        printf("\nYou must be logged in to open an account.\n");
        return;
    }
}

void withdraw_handler()
{
    if (loggedin)
    {
        FILE *account_file = fopen(ACCOUNT_FILE, "r+b");

        if (account_file == NULL)
        {
            account_file = fopen(ACCOUNT_FILE, "wb");
            if (account_file == NULL)
            {
                perror("Failed to open account file!");
                return;
            }
            fclose(account_file);

            account_file = fopen(ACCOUNT_FILE, "r+b");
        }

        Account account;
        int account_number;
        float withdraw_amount;

        printf("\nEnter account number to withdraw from: ");
        scanf("%d", &account_number);
        while (fread(&account, sizeof(Account), 1, account_file))
        {
            if (account.account_number == account_number && strcmp(account.account_holder, current_user) == 0)
            {
                printf("Enter amount to withdraw: ");
                scanf("%f", &withdraw_amount);
                if (withdraw_amount > account.balance)
                {
                    printf("\nInsufficient balance.\n");
                }
                else
                {
                    account.balance -= withdraw_amount;
                    printf("\nWithdrawal successful! New balance: %.2f\n", account.balance);
                    // Update the account file
                    fseek(account_file, -sizeof(Account), SEEK_CUR);
                    fwrite(&account, sizeof(Account), 1, account_file);
                }
                fclose(account_file);
                return;
            }
        }
        printf("\nAccount not found open account first!\n");
        fclose(account_file);
    }
    else
    {
        printf("\nYou must be logged in to withdraw from an account.\n");
        return;
    }
}

void deposit_handler()
{
    if (loggedin)
    {
        FILE *account_file = fopen(ACCOUNT_FILE, "r+b");

        if (account_file == NULL)
        {
            account_file = fopen(ACCOUNT_FILE, "wb");
            if (account_file == NULL)
            {
                perror("Failed to open account file!");
                return;
            }
            fclose(account_file);

            account_file = fopen(ACCOUNT_FILE, "r+b");
        }

        Account account;
        int account_number;
        float deposit_amount;

        printf("\nEnter account number to deposit into: ");
        scanf("%d", &account_number);
        while (fread(&account, sizeof(Account), 1, account_file))
        {
            if (account.account_number == account_number && strcmp(account.account_holder, current_user) == 0)
            {
                printf("Enter amount to deposit: ");
                scanf("%f", &deposit_amount);
                account.balance += deposit_amount;
                printf("\nDeposit successful! New balance: %.2f\n", account.balance);
                // Update the account file
                fseek(account_file, -sizeof(Account), SEEK_CUR);
                fwrite(&account, sizeof(Account), 1, account_file);
                fclose(account_file);
                return;
            }
        }
        printf("\nAccount not found open account first!\n");
        fclose(account_file);
    }
    else
    {
        printf("\nYou must be logged in to deposit in an account.\n");
        return;
    }
}

void change_password()
{
    if (loggedin)
    {
        FILE *change_pass = fopen(USER_FILE, "r+b");

        if (change_pass == NULL)
        {
            perror("Failed to open account file!");
            return;
        }

        User user;
        char tpassword[50];

        while (fread(&user, sizeof(User), 1, change_pass))
        {
            if (strcmp(user.username, current_user) == 0)
            {
                input_handler(current_user, tpassword, "changepass");
                fseek(change_pass, -sizeof(User), SEEK_CUR);
                strncpy(user.password, tpassword, CREDENTIALS_LENGTH);
                strncpy(user.username, current_user, CREDENTIALS_LENGTH);
                fwrite(&user, sizeof(User), 1, change_pass);
                printf("\nChanged password successfully!\n");
                fclose(change_pass);
                return;
            }
        }
        printf("\nUser not found!\n");
        fclose(change_pass);
    }
    else
    {
        printf("\nYou must be logged in to change your password.\n");
        return;
    }
}

void view_account_details()
{
    if (loggedin)
    {
        FILE *account_file = fopen(ACCOUNT_FILE, "rb");
        FILE *user_file = fopen(USER_FILE, "rb");

        if (account_file == NULL)
        {
            account_file = fopen(ACCOUNT_FILE, "wb");
            if (account_file == NULL)
            {
                perror("Failed to open account file!");
                return;
            }
            fclose(account_file);
        }

        account_file = fopen(ACCOUNT_FILE, "rb");

        if (user_file == NULL)
        {
            perror("Failed to open account file!");
            return;
        }

        Account account;
        User check_user_p;

        char old_password[CREDENTIALS_LENGTH];

        fgets(old_password, CREDENTIALS_LENGTH, stdin);
        fix_text_last(old_password);

        while (fread(&check_user_p, sizeof(User), 1, account_file))
        {
            if(strcmp(check_user_p.password, old_password) != 0){
                printf("Wrong Password!");
                fclose(account_file);
                fclose(user_file);
                return;
            }
        }

        while (fread(&account, sizeof(Account), 1, account_file))
        {
            if (strcmp(account.account_holder, current_user) == 0)
            {
                printf("\nAccount Holder name: %s\nAccount number %d\nAccount balance %.2f\n", account.account_holder, account.account_number, account.balance);
                fclose(account_file);
                return;
            }
        }
        printf("\nYour account is not open yet! First open it!\n");
        fclose(account_file);
    }
    else
    {
        printf("\nYou must be logged in to view account details.\n");
        return;
    }
}

void fix_text_last(char *text)
{
    int index = strcspn(text, "\n");
    text[index] = '\0'; // Remove newline character
    if (index == 0)
    {
        printf("\nInput cannot be empty.\n");
        exit(1);
    }
}


// adding soon!
/*
⚠️ Security (non-critical for learning, but important):

Passwords are stored as plain text.

No limits on login attempts.

No encryption — fine for now, but worth noting if you're aiming for real-world usability.

⚠️ Account updates not visible instantly (logic flaw):

If you deposit money and immediately check account, you may have to enter the account number again (since account number is needed during deposit, but not stored).

If user has multiple accounts, you're only printing first matched account in view_account_details. This might hide the one just modified.
*/