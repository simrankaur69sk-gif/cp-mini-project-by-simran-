/* Library Management System (short version)
   Compile: gcc main.c -o library
   Data is saved in the file library.dat */

#include <stdio.h>

#define BOOK_START 1001      /* first book ID */
#define MEMBER_START 5001    /* first member ID */

struct Book {
    int id;
    char title[50];
    char author[50];
    int available;           /* 1 = available, 0 = issued */
    int issuedTo;            /* ID of the member who has the book (0 if none) */
};

struct Member {
    int id;
    char name[50];
    char phone[15];
};

/* sample data to start with (replaced by the saved data when library.dat exists) */
struct Book books[100] = {
    {1001, "The C Programming Language", "Kernighan & Ritchie", 1, 0},
    {1002, "Let Us C", "Yashavant Kanetkar", 1, 0},
    {1003, "Data Structures Using C", "Reema Thareja", 0, 5001},
    {1004, "Wings of Fire", "A. P. J. Abdul Kalam", 1, 0},
    {1005, "Godan", "Munshi Premchand", 0, 5002}
};
struct Member members[100] = {
    {5001, "Aarav Sharma", "9876543210"},
    {5002, "Priya Singh", "9812345678"},
    {5003, "Rohan Verma", "9899001122"}
};
int bookCount = 5, memberCount = 3;

void saveData() {
    FILE *fp = fopen("library.dat", "wb");
    if (fp == NULL) { printf("Warning: could not save data.\n"); return; }
    fwrite(&bookCount, sizeof(int), 1, fp);
    fwrite(books, sizeof(struct Book), bookCount, fp);
    fwrite(&memberCount, sizeof(int), 1, fp);
    fwrite(members, sizeof(struct Member), memberCount, fp);
    fclose(fp);
}

void loadData() {
    FILE *fp = fopen("library.dat", "rb");
    if (fp == NULL)
        return;              /* first run: keep the sample data */
    fread(&bookCount, sizeof(int), 1, fp);
    fread(books, sizeof(struct Book), bookCount, fp);
    fread(&memberCount, sizeof(int), 1, fp);
    fread(members, sizeof(struct Member), memberCount, fp);
    fclose(fp);
}

void addBook() {
    struct Book *b = &books[bookCount];
    if (bookCount == 100) { printf("Library is full.\n"); return; }
    b->id = BOOK_START + bookCount;
    printf("Title: ");
    scanf(" %49[^\n]", b->title);
    printf("Author: ");
    scanf(" %49[^\n]", b->author);
    b->available = 1;
    b->issuedTo = 0;
    bookCount++;
    saveData();
    printf("Book added. ID = %d\n", b->id);
}

void viewBooks() {
    int i;
    printf("\n%-6s %-28s %-22s %s\n", "ID", "TITLE", "AUTHOR", "STATUS");
    for (i = 0; i < bookCount; i++) {
        printf("%-6d %-28.28s %-22.22s ", books[i].id, books[i].title, books[i].author);
        if (books[i].available == 1)
            printf("Available\n");
        else
            printf("Issued to member %d\n", books[i].issuedTo);
    }
}

void registerMember() {
    struct Member *m = &members[memberCount];
    if (memberCount == 100) { printf("Member list is full.\n"); return; }
    m->id = MEMBER_START + memberCount;
    printf("Name: ");
    scanf(" %49[^\n]", m->name);
    printf("Phone: ");
    scanf("%14s", m->phone);
    memberCount++;
    saveData();
    printf("Member registered. ID = %d\n", m->id);
}

void viewMembers() {
    int i;
    printf("\n%-8s %-30s %s\n", "ID", "NAME", "PHONE");
    for (i = 0; i < memberCount; i++)
        printf("%-8d %-30.30s %s\n", members[i].id, members[i].name, members[i].phone);
}

void issueBook() {
    int bid, mid;
    printf("Book ID: ");
    scanf("%d", &bid);
    printf("Member ID: ");
    scanf("%d", &mid);

    if (bid < BOOK_START || bid >= BOOK_START + bookCount)
        printf("Book not found.\n");
    else if (mid < MEMBER_START || mid >= MEMBER_START + memberCount)
        printf("Member not found.\n");
    else if (books[bid - BOOK_START].available == 0)
        printf("This book is already issued.\n");
    else {
        books[bid - BOOK_START].available = 0;
        books[bid - BOOK_START].issuedTo = mid;
        saveData();
        printf("Book issued.\n");
    }
}

void returnBook() {
    int bid;
    printf("Book ID: ");
    scanf("%d", &bid);

    if (bid < BOOK_START || bid >= BOOK_START + bookCount)
        printf("Book not found.\n");
    else if (books[bid - BOOK_START].available == 1)
        printf("This book is not issued.\n");
    else {
        books[bid - BOOK_START].available = 1;
        books[bid - BOOK_START].issuedTo = 0;
        saveData();
        printf("Book returned.\n");
    }
}

int main() {
    int choice;
    loadData();
    do {
        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n"
               "1. Add book\n2. View books\n3. Register member\n4. View members\n"
               "5. Issue book\n6. Return book\n0. Exit\n"
               "Enter your choice: ");
        if (scanf("%d", &choice) != 1)
            choice = 0;      /* if a letter is typed, exit */

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: registerMember(); break;
            case 4: viewMembers(); break;
            case 5: issueBook(); break;
            case 6: returnBook(); break;
            case 0: break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    saveData();
    printf("Data saved. Goodbye!\n");
    return 0;
}
