# Bankingsystem

A simple console-based banking system written in C.

## Features

- User registration and login
- Open up to 3 accounts per user
- Deposit and withdraw money
- View account details
- Change password
- Logout functionality

## Build Instructions

### On Linux/macOS:
```bash
gcc -o bankingsystem bankingsystem.c
./bankingsystem
````

### On Windows:

```bash
gcc -o bankingsystem.exe bankingsystem.c
bankingsystem.exe
```

> Uses `<conio.h>` for Windows and `<termios.h>` for Linux/macOS for password masking.

## Data Files

* `users.dat`: Stores user credentials
* `accounts.dat`: Stores account data

Files are created automatically if they don't exist.

## Notes

* Login is required to manage accounts.
* Passwords are masked during input.
* Maximum of 3 accounts allowed per user.

> See in-code comments for known security issues and improvement areas.
