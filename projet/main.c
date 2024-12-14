#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define FILENAME "patients.txt"

typedef struct {
    int id;
    char name[50];
    char address[100];
    int age;
    char gender[10];
    char disease[100];
    char service[20];
    int roomNumber;
} Patient;

// Function to compare patients by name
int comparePatients(const void *a, const void *b) {
    Patient *p1 = (Patient *)a;
    Patient *p2 = (Patient *)b;
    return strcmp(p1->name, p2->name);
}

// Function to read all patients from the file
int readPatients(Patient patients[]) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        return 0; // If file doesn't exist, return empty list
    }

    int count = 0;
    while (fscanf(file, "%d|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n",
                  &patients[count].id, patients[count].name, patients[count].address,
                  &patients[count].age, patients[count].gender, patients[count].disease,
                  patients[count].service, &patients[count].roomNumber) != EOF) {
        count++;
    }
    fclose(file);
    return count;
}

// Function to write all patients back to the file
void writePatients(Patient patients[], int count) {
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d|%s|%s|%d|%s|%s|%s|%d\n",
                patients[i].id, patients[i].name, patients[i].address,
                patients[i].age, patients[i].gender, patients[i].disease,
                patients[i].service, patients[i].roomNumber);
    }
    fclose(file);
}

// Function to add a new patient
void addPatient() {
    Patient patients[MAX_PATIENTS];
    int count = readPatients(patients);

    if (count >= MAX_PATIENTS) {
        printf("Capacité maximale atteinte.\n");
        return;
    }

    Patient p;
    printf("Entrez l'ID du patient: ");
    scanf("%d", &p.id);
    getchar(); // Consommer le caractère '\n'

    printf("Entrez le   nom complet du patient: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    printf("Entrez l'adresse du patient: ");
    fgets(p.address, sizeof(p.address), stdin);
    p.address[strcspn(p.address, "\n")] = '\0';

    printf("Entrez l'age du patient: ");
    scanf("%d", &p.age);
    getchar();

    printf("Entrez le sexe du patient (H/F): ");
    fgets(p.gender, sizeof(p.gender), stdin);
    p.gender[strcspn(p.gender, "\n")] = '\0';

    printf("Entrez la description de la maladie: ");
    fgets(p.disease, sizeof(p.disease), stdin);
    p.disease[strcspn(p.disease, "\n")] = '\0';

    printf("Entrez le service (Consultation(C)/Urgence(U)): ");
    fgets(p.service, sizeof(p.service), stdin);
    p.service[strcspn(p.service, "\n")] = '\0';

    printf("Entrez le numéro de chambre: ");
    scanf("%d", &p.roomNumber);

    // Add the new patient to the list
    patients[count] = p;
    count++;

    // Sort patients alphabetically
    qsort(patients, count, sizeof(Patient), comparePatients);

    // Save the sorted list back to the file
    writePatients(patients, count);

    printf("Patient ajouté et liste triée avec succès.\n");
}
// Function to delete a patient
void deletePatient() {
    Patient patients[MAX_PATIENTS];
    int count = readPatients(patients);

    if (count == 0) {
        printf("Aucun patient trouvé.\n");
        return;
    }

    int deleteId;
    printf("Entrez l'ID du patient à supprimer: ");
    scanf("%d", &deleteId);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (patients[i].id == deleteId) {
            found = 1;
            for (int j = i; j < count - 1; j++) {
                patients[j] = patients[j + 1];
            }
            count--;
            break;
        }
    }

    if (found) {
        // Save the updated list back to the file
        writePatients(patients, count);
        printf("Patient supprimé avec succès.\n");
    } else {
        printf("Patient introuvable.\n");
    }
}
void searchAndModifyPatient() {
    Patient patients[MAX_PATIENTS];
    int count = readPatients(patients);

    if (count == 0) {
        printf("Aucun patient trouvé dans le fichier.\n");
        return;
    }

    char searchQuery[50];
    int searchById = 0;

    printf("Rechercher par: \n1. ID\n2. Nom\nChoix: ");
    scanf("%d", &searchById);
    getchar(); // Consume newline

    if (searchById == 1) {
        printf("Entrez l'ID du patient: ");
        int searchId;
        scanf("%d", &searchId);
        getchar();

        for (int i = 0; i < count; i++) {
            if (patients[i].id == searchId) {
                printf("Patient trouvé:\n");
                printf("ID: %d\nNom: %s\nAdresse: %s\nÂge: %d\nSexe: %s\nMaladie: %s\nService: %s\nChambre: %d\n",
                    patients[i].id, patients[i].name, patients[i].address, patients[i].age,
                    patients[i].gender, patients[i].disease, patients[i].service, patients[i].roomNumber);

                modifyOrPrintPatient(&patients[i]);
                writePatients(patients, count); // Save modifications
                return;
            }
        }
        printf("Patient introuvable.\n");
    } else if (searchById == 2) {
        printf("Entrez le nom du patient: ");
        fgets(searchQuery, sizeof(searchQuery), stdin);
        searchQuery[strcspn(searchQuery, "\n")] = '\0';

        for (int i = 0; i < count; i++) {
            if (strcmp(patients[i].name, searchQuery) == 0) {
                printf("Patient trouvé:\n");
                printf("ID: %d\nNom: %s\nAdresse: %s\nÂge: %d\nSexe: %s\nMaladie: %s\nService: %s\nChambre: %d\n",
                    patients[i].id, patients[i].name, patients[i].address, patients[i].age,
                    patients[i].gender, patients[i].disease, patients[i].service, patients[i].roomNumber);

                modifyOrPrintPatient(&patients[i]);
                writePatients(patients, count); // Save modifications
                return;
            }
        }
        printf("Patient introuvable.\n");
    } else {
        printf("Choix invalide.\n");
    }
}

// Function to ask the user to print or modify the patient's information
void modifyOrPrintPatient(Patient *p) {
    int choice;
    printf("\nQue voulez-vous faire ?\n1. Imprimer les informations\n2. Modifier les informations\nChoix: ");
    scanf("%d", &choice);
    getchar(); // Consume newline

    if (choice == 1) {
        printf("\n--- Informations du patient ---\n");
        printf("ID: %d\nNom: %s\nAdresse: %s\nÂge: %d\nSexe: %s\nMaladie: %s\nService: %s\nChambre: %d\n",
            p->id, p->name, p->address, p->age,
            p->gender, p->disease, p->service, p->roomNumber);
    } else if (choice == 2) {
        printf("\n--- Modification des informations du patient ---\n");

        printf("Entrez le nouveau nom (laissez vide pour ne pas changer): ");
        char buffer[50];
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(p->name, buffer);
        }

        printf("Entrez la nouvelle adresse (laissez vide pour ne pas changer): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(p->address, buffer);
        }

        printf("Entrez le nouvel âge (0 pour ne pas changer): ");
        int newAge;
        scanf("%d", &newAge);
        getchar();
        if (newAge > 0) {
            p->age = newAge;
        }

        printf("Entrez le nouveau sexe (laissez vide pour ne pas changer): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(p->gender, buffer);
        }

        printf("Entrez la nouvelle maladie (laissez vide pour ne pas changer): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(p->disease, buffer);
        }

        printf("Entrez le nouveau service (laissez vide pour ne pas changer): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(p->service, buffer);
        }

        printf("Entrez le nouveau numéro de chambre (0 pour ne pas changer): ");
        int newRoom;
        scanf("%d", &newRoom);
        if (newRoom > 0) {
            p->roomNumber = newRoom;
        }

        printf("\nInformations mises à jour avec succès.\n");
    } else {
        printf("Choix invalide.\n");
    }
}


int main() {
    int choice;
    do {
        printf("\n--- Système de Gestion Hospitalière ---\n");
        printf("1. Ajouter un patient\n");
        printf("2. Supprimer un patient\n");
        printf("3. rechercher ou modifier un patients info\n");
        printf("4. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                deletePatient();
                break;
            case 3:
                searchAndModifyPatient();
                break;
            case 4:
                printf("Au revoir!\n");
                break;

            default:
                printf("Choix invalide.\n");
        }
    } while (choice != 4);

    return 0;
}
