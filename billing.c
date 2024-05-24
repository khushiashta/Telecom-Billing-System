#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

sqlite3 *db;
char *err_msg = 0;

void executeSQL(const char *sql) {
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(1);
    }
}

void createTable() {
    const char *sql = "CREATE TABLE IF NOT EXISTS Students("
                      "rollNumber INTEGER PRIMARY KEY,"
                      "name TEXT,"
                      "fees REAL);";
    executeSQL(sql);
}

void addStudent() {
    int rollNumber;
    char name[50];
    float fees;

    printf("Enter roll number, name, and fees: ");
    scanf("%d %s %f", &rollNumber, name, &fees);

    // Check if the roll number already exists
    char check_sql[256];
    snprintf(check_sql, sizeof(check_sql), "SELECT * FROM Students WHERE rollNumber = %d;", rollNumber);
    
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db, check_sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(res);
        return;
    }

    if (sqlite3_step(res) == SQLITE_ROW) {
        printf("A student with roll number %d already exists. Please enter a different roll number.\n", rollNumber);
        sqlite3_finalize(res);
        return;
    }
    sqlite3_finalize(res);

    // If roll number does not exist, insert the new student
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO Students (rollNumber, name, fees) VALUES (%d, '%s', %.2f);", rollNumber, name, fees);
    executeSQL(sql);

    printf("Student added successfully.\n");
}

void displayStudents() {
    const char *sql = "SELECT * FROM Students;";

    sqlite3_stmt *res;

    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    printf("Roll No.   Name          Fees\n");
    printf("----------------------------------\n");

    while (sqlite3_step(res) == SQLITE_ROW) {
        printf("%d      %s       %.2f\n", sqlite3_column_int(res, 0), sqlite3_column_text(res, 1), sqlite3_column_double(res, 2));
    }

    sqlite3_finalize(res);
}

void deleteStudent() {
    int rollNumber;

    printf("Enter roll number to delete: ");
    scanf("%d", &rollNumber);

    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM Students WHERE rollNumber = %d;", rollNumber);
    executeSQL(sql);

    if (sqlite3_changes(db) > 0) {
        printf("Record deleted successfully.\n");
    } else {
        printf("Record not found.\n");
    }
}

int main() {
    int rc = sqlite3_open("students.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    createTable();

    int choice;
    while (1) {
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Delete Student\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                deleteStudent();
                break;
            case 4:
                sqlite3_close(db);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
