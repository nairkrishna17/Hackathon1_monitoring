#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 50
#define MAX_REPORT_LEN 255

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char report[MAX_REPORT_LEN];
    char injury_type[MAX_NAME_LEN];  // To store injury type (internal/external)
} Patient;

Patient *patients = NULL;
int patient_count = 0;
int patient_capacity = 0;

void initialize_patients() {
    patient_capacity = 10;  // Initial capacity
    patients = malloc(patient_capacity * sizeof(Patient));
    if (patients == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
}

void resize_patients() {
    patient_capacity *= 2;
    patients = realloc(patients, patient_capacity * sizeof(Patient));
    if (patients == NULL) {
        perror("Failed to reallocate memory");
        exit(EXIT_FAILURE);
    }
}

void add_patient() {
    if (patient_count >= patient_capacity) {
        resize_patients();
    }

    Patient p;
    p.id = patient_count + 1;

    printf("Hello Doctor! Please add the patient's details.\n");

    printf("Enter patient name: ");
    if (fgets(p.name, MAX_NAME_LEN, stdin) == NULL) {
        perror("Error reading input");
        return;
    }
    p.name[strcspn(p.name, "\n")] = 0;  // Remove newline character

    printf("Is the injury internal or external? ");
    if (fgets(p.injury_type, MAX_NAME_LEN, stdin) == NULL) {
        perror("Error reading input");
        return;
    }
    p.injury_type[strcspn(p.injury_type, "\n")] = 0;  // Remove newline character

    // Convert to lowercase for consistency
    for (int i = 0; p.injury_type[i]; i++) {
        p.injury_type[i] = tolower(p.injury_type[i]);
    }

    printf("Enter report for %s: ", p.name);
    if (fgets(p.report, MAX_REPORT_LEN, stdin) == NULL) {
        perror("Error reading input");
        return;
    }
    p.report[strcspn(p.report, "\n")] = 0;  // Remove newline character

    patients[patient_count++] = p;
    printf("Patient and report added successfully.\n");
}

void view_reports() {
    if (patient_count == 0) {
        printf("No patients added yet.\n");
        return;
    }

    printf("\nPatient Reports:\n");
    for (int i = 0; i < patient_count; i++) {
        printf("Patient ID: %d\n", patients[i].id);
        printf("Name: %s\n", patients[i].name);
        printf("Injury Type: %s\n", patients[i].injury_type);
        printf("Report: %s\n", patients[i].report);
        printf("-------------------------\n");
    }
}

void search_patient() {
    char search_name[MAX_NAME_LEN];
    printf("Enter patient name to search: ");
    if (fgets(search_name, MAX_NAME_LEN, stdin) == NULL) {
        perror("Error reading input");
        return;
    }
    search_name[strcspn(search_name, "\n")] = 0;  // Remove newline character

    int found = 0;
    for (int i = 0; i < patient_count; i++) {
        if (strstr(patients[i].name, search_name) != NULL) {
            printf("Patient ID: %d\n", patients[i].id);
            printf("Name: %s\n", patients[i].name);
            printf("Injury Type: %s\n", patients[i].injury_type);
            printf("Report: %s\n", patients[i].report);
            printf("-------------------------\n");

            // Display zone information based on injury type
            if (strcmp(patients[i].injury_type, "internal") == 0) {
                printf("The patient is admitted to the Yellow Zone. Please take care.\n");
            } else if (strcmp(patients[i].injury_type, "external") == 0) {
                printf("The patient is admitted to the Red Zone. Please take care.\n");
            } else {
                printf("Injury type not recognized. Please ensure the correct type is entered.\n");
            }

            found = 1;
            break;  // Stop searching after finding the patient
        }
    }
    if (!found) {
        printf("No patient found with the name '%s'.\n", search_name);
    }
}

void cleanup() {
    free(patients);
}

int main() {
    initialize_patients();
    int choice;

    do {
        printf("\nHealth Monitoring System\n");
        printf("1. Add Patient and Report\n");
        printf("2. View Reports\n");
        printf("3. Search Patient\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            choice = 0; // Reset choice to invalid value
        }
        getchar();  // Consume newline character left by scanf()

        switch (choice) {
            case 1:
                add_patient();
                break;
            case 2:
                view_reports();
                break;
            case 3:
                search_patient();
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    cleanup();
    return 0;
}
