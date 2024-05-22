#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    int rollNumber;
    char name[50];
    float fees;
    struct Student *next;
} Student;

Student *headStudent = NULL;

void addStudent() {
    Student *newStudent = (Student *)malloc(sizeof(Student));
    printf("Enter roll number, name, and fees: ");
    scanf("%d%s%f", &(newStudent->rollNumber), newStudent->name, &(newStudent->fees));
    newStudent->next = headStudent;
    headStudent = newStudent;
}

void displayStudents() {
    Student *currentStudent = headStudent;
    printf("Roll No.   Name          Fees\n");
    printf("----------------------------------\n");
    while (currentStudent != NULL) {
        printf("%d      %s       %.2f\n", currentStudent->rollNumber, currentStudent->name, currentStudent->fees);
        currentStudent = currentStudent->next;
    }
    printf("\n");
}

void deleteStudent() {
    Student *currentStudent = headStudent;
    Student *previousStudent = NULL;

    if (currentStudent != NULL) {
        int rollNumberToDelete;
        printf("Enter roll number to delete: ");
        scanf("%d", &rollNumberToDelete);

        while (currentStudent->rollNumber != rollNumberToDelete && currentStudent != NULL) {
            previousStudent = currentStudent;
            currentStudent = currentStudent->next;
        }

        if (currentStudent == NULL) {
            printf("Record not found.\n");
        } else {
            if (previousStudent == NULL) {
                headStudent = currentStudent->next;
            } else {
                previousStudent->next = currentStudent->next;
            }
            free(currentStudent);
            printf("Record deleted.\n");
        }
    } else {
        printf("No records to delete.\n");
    }
}

int main() {
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
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}