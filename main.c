#include <conio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

#define MAX_INPUT 10
#define MAX_INPUT_USER 25

// Структура для зберігання іформації про користувача
struct User {
    char username[30];
    char password[30];
    char name[30];
    char lastname[30];
    char role[30];
    int age;
};


typedef struct TrainStation {
    char arrivalStation[50];
    int trainNum;
    char timeDeparture[10];
    char timeArrival[10];
    float costTicket;
    
    struct TrainStation* next;
    struct TrainStation* prev;

} TrainStation;    

TrainStation* head = NULL;

char username[50];
char password[50];

bool isNewUser = false;


// Функція для додавання нового потягу 
void addTrain(struct TrainStation** head); 

// Функція для виводу всієї інформації про потяги
void printTrain(struct TrainStation* head);

// Функція для видалення певного потягу за його номером/станцією прибуття (це забезпечено змінною deleteType)
void deleteTrain(struct TrainStation** head, const char* deleteType);

// Функція для  зміни інформації потягу за його номером
void changeTrainInfo(struct TrainStation** head);

// Функція для виводу інфографіки
void printPicture();

// Функція для визначення чи коректного був введений час
bool isValidTimeFormat(const char* timeStr);

// Функція для введеня та визначення коректного формату введення ціни квитка
void validateTrainCost(float* costTicket);

// Функція для виводу довідки про потяги зазначеного напрямку
void printTrainsSameArrivalSt(TrainStation* head);

// Функція для виводу довідки про потяги зазначеного номеру/зазначеного напрямку (це забезпечено змінною searchType)
void printMatchingTrains(TrainStation* head, int trainNumSearch, const char* arrivalStationSearch, const char* searchType);

// Функція для сортування за часом відправлення
void sortByDepartureTime(struct TrainStation** head);

// Функції для керування/входу в обліковий запис
void displayUserMainMenu();
void accountLogin();
void createAccount();
bool isUsernameTaken(const char* username);
void userMenu();
void gotoxy(int x, int y);
void deleteUser();

// Функція для запису у файл
void writeToFile(TrainStation *head, const char *filename);

// Функція для "зчитування" з файлу
TrainStation* createLinkedListFromBinaryFile(const char* filename);


int main() {
    int choice;
    char sure = 'n'; // Змінна для виконання запитів необоротних процесів
    while (true) {
        system("cls");
        printPicture();
        gotoxy(18, 3);
        printf("WELCOME TO TRAIN-STATION SCHEDULE SYSTEM");
        gotoxy(18, 5);
        printf("*****************************************");

	    gotoxy(14, 7);
        printf("Created by student Popazov Petro, Group AI - 221");

        gotoxy(3, 9);
        printf("The program represents a train station schedule system Odesa-Holovna");

        gotoxy(24, 12);
        printf("1. Create Account...\n");

        gotoxy(24, 14);
        printf("2. Aready a user? Login...\n");

        gotoxy(24, 16);
        printf("3. Exit the program...\n");

        gotoxy(16, 19);
        printf("Waiting for your choice. Choose (1-3): ");
        while (scanf("%d", &choice) != 1) {
        printf("\n \n Error: invalid input. Please enter an integer number: ");
        while (getchar() != '\n');
        }

        switch (choice) {
            case 1:
            createAccount();
            sleep(2);
            break;

            case 2:
            getchar();
            accountLogin();
            sleep(2);
            break;

            case 3:
            printf("\n \n Exiting program.\n");
            printf("\n Are you sure you want to exit? y/n : ");
            scanf(" %c", &sure);
            getchar();
            if (sure == 'y'){
                printf("\n \n The program will be closed...");
                sleep(2);
                exit(0);
            }
            else{
                printf("\n Continue working...");
                sleep(1);
            }
            break;
                
            default:
            fflush(stdin);
            printf("\n \n Error: invalid choice. Please try again.");
            sleep(2);
            break;
        }
    }
    return 0;
}

///////////////////////////////////////Функції для управління записами про поїзди//////////////////////////////////////////////////////////////////////////////////////

void displayUserMainMenu(char *username){
    system("cls");
   
    int choice;
    char sure = 'n'; 
    char arrStatSearch[50]; // Змінна для пошуку за станцією прибуття case 6
    int trainNumSearch; // Змінна для пошуку за номером потягу case 7

    FILE* fileAccount  = fopen("accounts.bin", "rb");
    if (fileAccount  == NULL) {
        printf("\n Error in opening file \n");
        return;
    }

    bool found_user = false;
    struct User user;

    gotoxy(0, 6);
    while(true){
        // Прочитати та показати інформацію про користувача
        while (fread(&user, sizeof(struct User), 1, fileAccount) == 1) {
            if (strcmp(user.username, username) == 0) {
                found_user = true;
                gotoxy(30, 1);
                printf("WELCOME, %s %s", user.name, user.lastname);
                gotoxy(28, 2);
                printf("**************************");
                gotoxy(85, 4);
                printf("==== YOUR ACCOUNT INFO ====");
                gotoxy(85, 6);
                printf("USERNAME...%s", user.username);
                gotoxy(85, 8);
                printf("NAME...%s", user.name);
                gotoxy(85, 10);
                printf("Surname...%s", user.lastname);
                gotoxy(85, 12);
                printf("Age...%d", user.age);
                gotoxy(85, 14);
                printf("Role...%s", user.role);
                gotoxy(85, 16);
                printf("===========================");
                break;
            }
        }

        rewind(fileAccount );

        if (!found_user) {
            printf("\n User %s not found.\n", username);
            return;
        }

        gotoxy(0, 4);
        printf("====================================Main-Menu====================================\n");
        printf("1. Add a new train \n");
        printf("2. Delete a train \n");
        printf("3. Change information about a specific train \n");
        printf("4. Output all the information\n");
        printf("5. Output information about a train with specific destination \n");
        printf("6. Output arrival time of a train with specific destination \n");
        printf("7. Output the departure time of a train with specific train number\n");
        printf("8. Delete train by specified destination \n");
        printf("9. Sort trains according to the departure time \n");
        printf("10. Go to user-backup menu \n");
        printf("11. Exit \n");
        printf("===============================================================================\n");
    
        printf("Enter your choice (1-11): ");
        while (scanf("%d", &choice) != 1) {
        printf("\n \n Error: invalid input. Please enter an integer number: ");
        while (getchar() != '\n');
        }
        getchar();
        printf("\n");

        switch(choice){
            case 1:
            addTrain(&head);
            printf("\n Please wait...");
            sleep(3);
            system("cls");
            break;

            case 2:
            printTrain(head);
            printf("2. Delete a train\n");
            deleteTrain(&head, "byTrainNumber");
            sleep(2);
            system("cls");
            break;

            case 3:
            printTrain(head);
            changeTrainInfo(&head);
            sleep(2);
            system("cls");
            break;

            case 4:
            printf("4. Output all the information \n");
            printTrain(head);
            printf("To continue press enter...");
            getch();
            system("cls");
            break;

            case 5:
            printf("5. Output information about a train with specified destination \n");
            printTrainsSameArrivalSt(head);

            printf("To continue press enter...");
            getch();
            system("cls");
            break;

            case 6:
            printf("6. Output arrival time of a train with specified destination \n");
            printf("\n \n Enter arrival station of a train you want to get arrival time from: ");
            
            fgets(arrStatSearch, 50, stdin);
            arrStatSearch[strcspn(arrStatSearch, "\n")] = '\0';

            printMatchingTrains(head, 0, arrStatSearch, "arrivalStation");

            printf("\nTo continue press enter...");
            getch();
            system("cls");
            break;

            case 7:
            printf("7. Output the departure time of a train with specifed train number\n");
            printf("\n \n Enter number of train you want to get deparure time for: ");
            while (scanf("%d", &trainNumSearch) != 1) {
            printf("\n \n Error: invalid input. Please enter an integer number: ");
            while (getchar() != '\n');
            }

            printMatchingTrains(head, trainNumSearch, "\0", "trainNum");

            printf("To continue press enter...");
            getch();
            system("cls");
            break;

            case 8:
            printf("8. Delete information about trains with specific destination \n");
            printTrain(head);
            deleteTrain(&head, "byArrivalStation");
            
            sleep(2);
            system("cls");
            break;

            case 9:
            printf("9. Sort trains according to the departure time \n");
            sortByDepartureTime(&head);

            printf("To continue press enter...");
            getch();
            system("cls");
            break;

            case 10:
            userMenu();
            system("cls");
            break;

            case 11:
            printf("11. Exit \n");
            printf("\n The program will be closed. Are you sure you want to exit? y/n : ");
            scanf(" %c", &sure);
            getchar();
            if (sure == 'y'){
                    char doBackUp = 'n';
                    printf("\n Do you want to save all information? y/n: ");
                    scanf("%c", &doBackUp);
                    if (doBackUp == 'y'){
                        writeToFile(head, username);
                        sleep(2);
                    }
                    else {
                        printf("\n Back up data has not been saved!");
                    }
                    while (head != NULL) {
                        struct TrainStation* tempDelete = head;
                        head = head->next;
                        free(tempDelete);
                    }
                    printf("\n \n Exiting program...");
                    sleep(2);
                    system("cls");
                    fclose(fileAccount );
                    exit(0);
                }
                else {
                    printf("\n Continue working.");
                    sleep(1);
                    system("cls");
                }
            break;

            default:
            printf("Error: you have entered an invalid option. Try again. \n");
            sleep(1);
            system("cls");
            break;
        }
    }
    return;
}

void addTrain(struct TrainStation** head){
    printf("1. Add a new train\n");
    TrainStation* newTrain = (TrainStation*)malloc(sizeof(TrainStation));

    if (newTrain == NULL){
        printf("\n Error: Memory allocation failed for new train. The program will be closed. \n");
        return;
    }

    char trainNumStr[15]; // буфер, щоб прочитати номер потягу, як строку

    printf("\n  NOTE! \n \n 1. For Arrival station use no more than 50 characters. \n 2. Train number and cost of the tickets should be less than 10 characters. \n 3. For arrival and departure time use this format HH:MM \n \n");

    printf("Enter train number: ");
    while (true) {
        trainNumStr[0] = '\0';
        scanf("%s", trainNumStr);
        bool isValid = true;
        for (int i = 0; trainNumStr[i] != '\0'; i++) {
            if (!isdigit(trainNumStr[i]) || strlen(trainNumStr) >= MAX_INPUT) {
                isValid = false;
                break;
            }
        }      
        if (isValid && sscanf(trainNumStr, "%d", &newTrain->trainNum) == 1 && newTrain->trainNum >= 0) {
            // Перевірити, чи зайнятий такий номер потягу
            TrainStation* temp = *head;
            while (temp != NULL) {
                if (temp->trainNum == newTrain->trainNum) {
                    printf("\n Error: Train number %d is already used. Try again with a different number: ", newTrain->trainNum);
                    isValid = false;
                    break;
                }
                temp = temp->next;
            }
            if (isValid) {
                break;
            }   
        } 
        else {
            printf("Error: You have not entered a valid number or a positive number. Try again, using only digits (0-9) and up to 9 digits: ");
            fflush(stdin);
        }
    }
    fflush(stdin);
    // Тут перевірка не виконується, бо у назві станції можуть бути присутні різні символи
    printf("Enter Arrival Station: ");
    fgets(newTrain->arrivalStation, 50, stdin);
    newTrain->arrivalStation[strcspn(newTrain->arrivalStation, "\n")] = '\0';

    /* Для коректного введення станцій із звома словами, використано функцію fgets, 
    яка бере поток stdin, як вхідний та максимальну кількість символів - 50. За для того, щоб прибрати перехід на нову строку, 
    використано функцію strcspn. Перехід на нову строчку (\n) заміняється термінатором */ 

    do
    {   printf("Enter departure time. Format HH:MM: ");
        fgets(newTrain->timeDeparture, 10, stdin);
        newTrain->timeDeparture[strcspn(newTrain->timeDeparture, "\n")] = '\0';

        if (!isValidTimeFormat(newTrain->timeDeparture))
            printf("Error: invalid input. Please enter a valid time in HH:MM format.\n");
    } while (!isValidTimeFormat(newTrain->timeDeparture));

    do
    {   printf("Enter arrival time. Format HH:MM: ");
        fgets(newTrain->timeArrival, 10, stdin);
        newTrain->timeArrival[strcspn(newTrain->timeArrival, "\n")] = '\0';

        if (!isValidTimeFormat(newTrain->timeArrival))
            printf("Error: invalid input. Please enter a valid time in HH:MM format.\n");
    } while (!isValidTimeFormat(newTrain->timeArrival));

    
    float costTicket;
    validateTrainCost(&costTicket);
    newTrain->costTicket = costTicket;

    newTrain->next = NULL;
    newTrain->prev = NULL;

    int opNewTrain; // змінна для вибру одного з варіантів вставлення нової інформації у двозв'язний список

    printf("\n1. Insert at the beginning.\n");
    printf("2. Insert at the end.\n");
    printf("3. Insert after a specific train number. \n");

    printf("\n Where do you want a new train to appear?: ");
    while (scanf("%d", &opNewTrain) != 1) {
    printf("\n \n Error: invalid input. Please enter an integer number: ");
    while (getchar() != '\n');
    }
    printf("\n");

    switch (opNewTrain){

        case 1:
        printf("1. Insert at the beginning.\n");
        
        newTrain->prev = NULL;

        // Якщо head не NULL, оновити показник prev, щоб він вказував на нову ноду newTrain
        if (*head != NULL) {
            (*head)->prev = newTrain; 
            newTrain->next = *head; 
        }       

        *head = newTrain; 

        printf("\n Train with Train Number %d added successfully.\n", newTrain->trainNum);
        break;

        case 2:
        printf("2. Insert at the end\n");
        if (*head == NULL){
            *head = newTrain;
        }
        else{
            TrainStation* current = *head;
            while (current->next != NULL){
                current = current->next;
            }
            current->next = newTrain;
            newTrain->prev = current;
        }
        printf("\n Train with Train Number %d added successfully.\n", newTrain->trainNum);
        break;

        case 3:
        printf("3. Insert after a specific train number.\n");
        int afterTrain;
        printTrain(*head);
        printf("Enter the train number after which you want to insert the new train: ");
        while (scanf("%d", &afterTrain) != 1) {
        printf("\n \n Error: invalid input. Please enter an integer number: ");
        while (getchar() != '\n');
        }
        
        TrainStation* current = *head;

        // Знайти вузол, після якої всавити нову
        while (current != NULL && current->trainNum != afterTrain){
            current = current->next;
        }
        if (current == NULL){ 
            printf("Error: train with train number %d not found.\n", afterTrain);
        }
        else {
            newTrain->prev = current;
            newTrain->next = current->next;

            current->next = newTrain;

            // Оновити prev показник, якщо він існує
            if (newTrain->next != NULL){
                newTrain->next->prev = newTrain;
            }
            printf("\n Train with Train Number %d added successfully after train number %d.\n", newTrain->trainNum, afterTrain);
        }
        break;
       
       
        default:
        printf("\n\n\n Error: you have entered an invalid option. Try again. \n");
        break;
        
    }
}

bool isValidTimeFormat(const char* timeStr) {
    // Перевірити чи 5 символів було введено (HH:MM)
    if (strlen(timeStr) != 5) {
        return false;
    }

    // "Витягнути" часи та хвилини з введеного
    int hours, minutes;
    if (sscanf(timeStr, "%d:%d", &hours, &minutes) != 2) {
        return false;
    }

    // Перевірити чи знаходяться часи та хвилини у дозволеному проміжку
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        return false;
    }

    return true;
}

void deleteTrain(struct TrainStation** head, const char* deleteType) {
    int deleteTrain; // Змінна для збереження номера потягу для видалення 
    char deleteArrivalStation[50]; // Змінна для збереження ст. прибуття потягу для видалення 
    
    char sure = 'n';
    TrainStation* del_node = *head;

    if (del_node == NULL) {
        printf("\n Error: there are no trains to delete. Add one first and try again.\n");
        return;
    }

    if (strcmp(deleteType, "byTrainNumber") == 0) {
        printf("\nEnter the train number to delete: ");
        while (scanf("%d", &deleteTrain) != 1) {
        printf("\n \n Error: invalid input. Please enter an integer number: ");
        while (getchar() != '\n');
        }
        getchar();

        // Знайти потяг за його номером
        while (del_node != NULL && del_node->trainNum != deleteTrain) {
            del_node = del_node->next;
        }
    } 
      else {
        printf("Enter the arrival station to delete: ");
        fgets(deleteArrivalStation, 50, stdin);
        deleteArrivalStation[strcspn(deleteArrivalStation, "\n")] = '\0';

        // Знайти потяг за станцією прибуття
        while (del_node != NULL && strcmp(del_node->arrivalStation, deleteArrivalStation) != 0) {
            del_node = del_node->next;
        }
    } 

    if (del_node == NULL) {
        if (strcmp(deleteType, "byTrainNumber") == 0) {
            printf("Train #%d not found!\n", deleteTrain);
        } else  {
            printf("Train with arrival station <%s> not found!\n", deleteArrivalStation);
        }
        return;
    }   

    // Перевірити, чи є декілька потягів з однаковою станцією прибуття
    TrainStation* temp = *head;
    int count = 0;
    while (temp != NULL) {
        if (strcmp(temp->arrivalStation, del_node->arrivalStation) == 0) {
            count++;
        }
        temp = temp->next;
    }   

    if (count > 1) {
        printf("There are multiple trains with the same arrival station <%s>.\n", del_node->arrivalStation);
        printf("Please specify the train number to delete: ");
        while (scanf("%d", &deleteTrain) != 1) {
        printf("\n \n Error: invalid input. Please enter an integer number: ");
        while (getchar() != '\n');
        }
        getchar();

        // Пошук за номером
        del_node = *head;
        while (del_node != NULL && del_node->trainNum != deleteTrain) {
            del_node = del_node->next;
        }

        if (del_node == NULL) {
            printf("\nTrain with number %d not found!\n", deleteTrain);
            return;
        }
    }

    // Видалити потяг
    printf("\nAre you sure, you want to delete train #%d with arrival station <%s> y/n : ", deleteTrain, del_node->arrivalStation);
    scanf(" %c", &sure);
    if (sure == 'y'){
        if (*head == del_node) {
            *head = del_node->next;
        }
        if (del_node->prev != NULL) {
            del_node->prev->next = del_node->next;
        }
        if (del_node->next != NULL) {
            del_node->next->prev = del_node->prev;
        }
        free(del_node);
    }
    else {
        printf("\n Train was NOT deleted!");
        return;
    }

    printf("Train was deleted successfully.\n");

    return;
}

void changeTrainInfo(struct TrainStation** head) {
    printf("3. Change the information about a specific train \n");

    int trainChange;
    printf("Enter train number for which you want to change information: ");
    while (scanf("%d", &trainChange) != 1) {
        printf("\n \n Error: invalid input. Please enter an integer number: ");
        while (getchar() != '\n');
    }
    getchar();

    TrainStation* current = *head;
    bool found = false;

    while (current != NULL) {
        if (current->trainNum == trainChange) {
            found = true;
            printf("Enter Arrival Station: ");
            fgets(current->arrivalStation, 50, stdin);
            current->arrivalStation[strcspn(current->arrivalStation, "\n")] = '\0';

            do {
                printf("Enter departure time. Format HH:MM: ");
                fgets(current->timeDeparture, 10, stdin);
                current->timeDeparture[strcspn(current->timeDeparture, "\n")] = '\0';

                if (!isValidTimeFormat(current->timeDeparture))
                    printf("Error: invalid input. Please enter a valid time in HH:MM format.\n");
            } while (!isValidTimeFormat(current->timeDeparture));

            do {
                printf("Enter arrival time. Format HH:MM: ");
                fgets(current->timeArrival, 10, stdin);
                current->timeArrival[strcspn(current->timeArrival, "\n")] = '\0';

                if (!isValidTimeFormat(current->timeArrival))
                    printf("Error: invalid input. Please enter a valid time in HH:MM format.\n");
            } while (!isValidTimeFormat(current->timeArrival));

            float costTicket;
            validateTrainCost(&costTicket);
            current->costTicket = costTicket;

            printf("\n Information for train number %d has been updated. \n", trainChange);
            printf("\n \n Please wait...");
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("\n \nError: train with number %d was not found. Try one more time. \n", trainChange);
        return;
    }

    return;
}

void validateTrainCost(float* costTicket) {
    char trainCostStr[MAX_INPUT];
    
    while (true) {
        trainCostStr[0] = '\0';
        printf("Enter cost of ticket: ");
        scanf("%s", trainCostStr);
        bool isValid = true;
        
        // Перевірити, щоб введена строка була з цифр та виключити крапку з перевірки, щоб введене не перевищувало межі
        for (int i = 0; trainCostStr[i] != '\0'; i++) {
            if ((!isdigit(trainCostStr[i]) && trainCostStr[i] != '.') || strlen(trainCostStr) >= MAX_INPUT) {
                isValid = false;
                break;
            }
        }
        
        if (isValid && sscanf(trainCostStr, "%f", costTicket) == 1 && *costTicket >= 0) {
            getchar();
            fflush(stdin);
            break;
        } else {
            printf("\nError: you have not entered a valid positive float number. Try again, use only (0-9) and a decimal point. \n");
            getchar();
            fflush(stdin);
        }
    }
    
    return;
}

void printTrain(struct TrainStation* head){
    if(head == NULL){
        printf("\n \n Error: no trains found. Add one first and try again.\n");
        return;
    }

    printf("=====================================Time-Table=====================================\n");
    printf("%-15s %-20s %-15s %-15s %-15s\n", 
    "Train Number", "Arrival Station", "Departure Time", "Arrival Time", "Cost of Ticket");
    printf("===================================================================================\n");
    
    TrainStation* current = head;
    while(current != NULL){
        printf("%-15d %-20s %-15s %-15s %-15.2f\n", 
        current->trainNum, current->arrivalStation, current->timeDeparture, current->timeArrival, current->costTicket);
        current = current->next;
    }
    printf("====================================================================================\n\n");

    return;
}

void printTrainsSameArrivalSt(TrainStation* head) {

    char arrStatSearch[50];
    printf("\n Enter station, where you want to get: ");
    fgets(arrStatSearch, 50, stdin);
    arrStatSearch[strcspn(arrStatSearch, "\n")] = '\0';

    TrainStation* current = head;
    bool found = false;
    // Знайти хоча б один запис з такою станцією прибуття
    while (current != NULL) {
        if (strcmp(current->arrivalStation, arrStatSearch) == 0) {
            found = true;
            break;
        }
        current = current->next;
    }

    if (found) {  
        printf("\nTrain table with arrival station '%s':\n\n", arrStatSearch);
        printf("=====================================Time-Table=====================================\n");
        printf("%-15s %-20s %-15s %-15s %-15s\n",
            "Train Number", "Arrival Station", "Departure Time", "Arrival Time", "Cost of Ticket");
        printf("===================================================================================\n");

        current = head;
        while (current != NULL) {
            if (strcmp(current->arrivalStation, arrStatSearch) == 0) {
                printf("%-15d %-20s %-15s %-15s %-15.2f\n",
                    current->trainNum, current->arrivalStation, current->timeDeparture, current->timeArrival, current->costTicket);
            }
            current = current->next;
        }

        printf("====================================================================================\n\n");
    } else {
        printf("\n \nNo trains found with the arrival station %s .\n \n", arrStatSearch);
    }

    return;
}

void printMatchingTrains(TrainStation* head, int trainNumSearch, const char* arrivalStationSearch, const char* searchType) {
    TrainStation* current = head;
    bool found = false;

    while (current != NULL) {
        if (strcmp(searchType, "trainNum") == 0 && current->trainNum == trainNumSearch) {
            printf("\n Train with number %d departures at %s \n\n", current->trainNum, current->timeDeparture);
            found = true;
        } else if (strcmp(searchType, "arrivalStation") == 0 && strcmp(current->arrivalStation, arrivalStationSearch) == 0) {
            printf("\n Train to <%s> arrives at %s\n\n", current->arrivalStation, current->timeArrival);
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        printf("\n \n Error: no trains found!\n");
    }

    return;
}

void sortByDepartureTime(struct TrainStation** head) {
    bool swapped = false;
    TrainStation* current = *head;
    TrainStation* last = NULL;

    // Перевірити, щоб було більше 1 запису
    if (*head == NULL || (*head)->next == NULL) {
        printf("\n \nError: no data to sort, or there is only one train -> nothing to sort!\n");
        return;
    }

    do {
        swapped = false;
        current = *head;

        while (current->next != last) {
            // Порівняти час відправлення та за потреби поміняти вузли
            if (strcmp(current->timeDeparture, current->next->timeDeparture) > 0) {
                TrainStation* temp = current->next;
                current->next = temp->next;

                if (temp->next != NULL) {
                    temp->next->prev = current;
                }

                temp->prev = current->prev;
                temp->next = current;
                current->prev = temp;

                if (temp->prev != NULL) {
                    temp->prev->next = temp;
                } else {
                    *head = temp;
                }

                swapped = true;
                current = temp;  // Оновити поточний покажчик після заміни
            } else {
                current = current->next;  // Перехід до наступного вузла
            }
        }

        last = current;
    } while (swapped);

    printTrain(*head);

    return;
}

//////////////////////////////////////////Функції для управління обліковим записом////////////////////////////////////////////////////////////////////////////

void gotoxy(int x, int y){
    COORD c;

	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

    return;
}

void createAccount() {
    struct User user;

    system("cls");
    printf("\n\nCREATING NEW ACCOUNT... \n\n");

    printf("Note!!! \n 1. For all input MAXIMUM characters - 25 \n 2. For age enter only digits \n \n \n");

    printf("Enter username: ");
    while (true) {
        scanf("%s", user.username);
        if (strlen(user.username) > MAX_INPUT_USER) {
            printf("Error: Input exceeds the maximum length of %d characters. Please enter a shorter username: ", MAX_INPUT_USER);
        } else {
            break;
        }
    }

    if (isUsernameTaken(user.username)) {
        printf("\n Error: Username [%s] is already taken. Please choose a different username.\n", user.username);
        return;
    }

    printf("Enter password: ");
    while (true) {
        scanf("%s", user.password);
        if (strlen(user.password) > MAX_INPUT_USER) {
            printf("Error: Input exceeds the maximum length of %d characters. Please enter a shorter password: ", MAX_INPUT_USER);
        } else {
            break;
        }
    }

    printf("Enter your name: ");
    while (true) {
        scanf("%s", user.name);
        if (strlen(user.name) > MAX_INPUT_USER) {
            printf("Error: Input exceeds the maximum length of %d characters. Please enter a shorter name: ", MAX_INPUT_USER);
        } else {
            break;
        }
    }

    printf("Enter your lastname: ");
    while (true) {
        scanf("%s", user.lastname);
        if (strlen(user.lastname) > MAX_INPUT_USER) {
            printf("Error: Input exceeds the maximum length of %d characters. Please enter a shorter lastname: ", MAX_INPUT_USER);
        } else {
            break;
        }
    }

    printf("Enter your age: ");
    while (scanf("%d", &user.age) != 1) {
        printf("\nError: Invalid input. Please enter only digits! Try again: ");
        fflush(stdin);
    }

    printf("Enter your role: ");
    while (true) {
        scanf("%s", user.role);
        if (strlen(user.role) > MAX_INPUT_USER) {
            printf("Error: Input exceeds the maximum length of %d characters. Please enter a shorter role: ", MAX_INPUT_USER);
        } else {
            break;
        }
    }

    FILE* fileAccount = fopen("accounts.bin", "ab");
    if (fileAccount == NULL) {
        printf("\nError: The file cannot be opened. Exiting the function...");
        return;
    }

    fwrite(&user, sizeof(struct User), 1, fileAccount);
    fclose(fileAccount);

    printf("\n\nACCOUNT HAS BEEN CREATED.\n\n");

    isNewUser = true;

    printf("\n\nPress enter to login...");
    getch();

    accountLogin();
    return;
}

bool isUsernameTaken(const char* username) {
    FILE* fileAccount = fopen("accounts.bin", "rb");
    if (fileAccount == NULL) {
        //Файлу не існує ->  -> не має записів -> логін не зайнятий
        return false;
    }

    struct User user;
    while (fread(&user, sizeof(struct User), 1, fileAccount) == 1) {
        if (strcmp(user.username, username) == 0) {
            fclose(fileAccount);
            return true; // Логін зайнятий
        }
    }

    fclose(fileAccount);
    return false; // Логін не зайнятий
}

void accountLogin() {
    
    fflush(stdin);

    system("cls");

    gotoxy(34, 2);
    printf("ACCOUNT LOGIN");

    gotoxy(19, 4);
    printf("*******************************************");

    gotoxy(30, 8);
    printf("======= LOG IN =======");

    gotoxy(30, 10);
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);

    gotoxy(30, 12);
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);

    // Прибрати перехід на новий рядок
    username[strcspn(username, "\n")] = '\0';
    password[strcspn(password, "\n")] = '\0';

    FILE* fileAccount = fopen("accounts.bin", "rb");
    if (fileAccount == NULL) {
        printf("\n Error: the file cannot be opened. Exiting the function...\n");
        return;
    }

    // Перевірити чи логін та пароль співпадають з тими, що у файлі 
    bool found = false;
    struct User user;
    while (fread(&user, sizeof(struct User), 1, fileAccount)) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            found = true;
            break;
        }
    }

    fclose(fileAccount);

    // Якщо такий користувач знайшовся...
    if (found) {
        system("cls");
        gotoxy(30, 5);
        printf("LOGIN SUCCESSFUL....");

        if (!isNewUser){
            head = createLinkedListFromBinaryFile(username);
            if (head == NULL) {
                printf("\n \n \tError: could not read data from the back up file.\n");
            }
            else {
                printf("\n \n \n \t Backed up data has been restored! You can continue working.");
            }
            sleep(2);
        }
        else {
            printf("\n \n \n \t \t You are new here. Press ENTER to get started!");
            getchar();
        }

        displayUserMainMenu(username);
    }
    else {
        printf("\n Error: login failed. The username or password may be incorrect. \n Please try again. You will be automatically redirected to the login menu.");
        sleep(4);
    }
    return;
}

void userMenu(){
    
    int choice;
    
    system("cls");
    printPicture();
    gotoxy(28, 2);
    printf("USER-BACK-UP-MENU");

    gotoxy(18, 4);
    printf("******************************************");
    
    gotoxy(25, 6);
    printf("1. Backup your data.");
    gotoxy(25, 8);
    printf("2. Use your backup.");
    gotoxy(25, 10);
    printf("3. Delete user.");
    gotoxy(25, 12);
    printf("4. Go back to the main menu.");
    gotoxy(21, 15);
    printf("Enter what you want to do (1-4): ");
    while (scanf("%d", &choice) != 1) {
    printf("\n \n Error: invalid input. Please enter an integer number: ");
    while (getchar() != '\n');
    }

    switch(choice){
        case 1:
        writeToFile(head, username);
        sleep(2);
        break;

        case 2:
        head = createLinkedListFromBinaryFile(username);
        if (head == NULL) {
            printf("\n \nError: could not read data from the back up file.\n");
            return;
        }
        else {
            printf("\n \n Backed up data has been restored! You can continue working.");
        }
        sleep(2);
        break;

        case 3:
        deleteUser();
        sleep(3);
        break;

        case 4:
        return;
        break;

        default:
        printf("\n \nError: invalid input. Try one more time.");
        break;
    }

    return;
}

void deleteUser() {

    char sure = 'n';
    char deleteUser[20];
    bool found = false;

    system("cls");

    
    printf("\n Enter username you want to delete: ");
    scanf("%s", deleteUser);
    
    fflush(stdin);
    printf("\nAre you sure, you want to delete account <%s> y/n: ", deleteUser);
    scanf("%c", &sure);
    if(sure == 'n'){
        printf("\n \n Account <%s> was NOT deleted. Continue working...", deleteUser);
        return;
    }

    FILE* file = fopen("accounts.bin", "rb+");
    if (file == NULL){
        printf("Error: couldn`t open the file");
        return;
    }

    struct User user;
    
    while (fread(&user, sizeof(struct User), 1, file)) {
        if (strcmp(user.username, deleteUser) == 0) {
            found = true;
            break;
        }
    }

    if (found) {
        // Переміститися на 1 запис назад
        fseek(file, -sizeof(struct User), SEEK_CUR);

        // Заповнити 0 -> видалити
        memset(&user, 0, sizeof(struct User));
        fwrite(&user, sizeof(struct User), 1, file);

        // Видалити бекап користувача
        remove(deleteUser);
        printf("\n \n Account <%s> was deleted and all back up file was also deleted!", deleteUser);
    }
    else{ 
        printf("\n\n Account <%s> was NOT found and NOT deleted. Continue warking...", deleteUser);
        return;
    }

    if(strcmp(username, deleteUser) == 0){
        printf("\n\n You have just deleted your OWN account. The program will be closed \n");
        sleep(2);
        exit(0);
    }
    fclose(file);
    return;
}

///////////////////////////////////Функції для управляння файлами/////////////////////////////////////////////////////////////////////////////////////////

void writeToFile(TrainStation *head, const char *filename){
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error: failed to open file\n");
        return;
    }

    while (head != NULL){
        fwrite(head, sizeof(TrainStation), 1, fp);
        head = head->next;
    }
    fclose(fp);
    printf("\n Data has been sucessfully backed up.");

    return;
} 

TrainStation* createLinkedListFromBinaryFile(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return NULL;
    }

    TrainStation* head = NULL;
    TrainStation* tail = NULL;

    while (!feof(fp)) {
        TrainStation* new_node = (TrainStation*)malloc(sizeof(TrainStation));
        size_t num_read = fread(new_node, sizeof(TrainStation), 1, fp);
        if (num_read == 0) {
            free(new_node);
            break;
        }
        new_node->prev = tail;
        new_node->next = NULL;
        if (tail == NULL) {
            head = new_node;
        } else {
            tail->next = new_node;
        }
        tail = new_node;
    }

    fclose(fp);
    return head;
}

void printPicture(){

    gotoxy(80,2);
    printf("       .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    gotoxy(80,3);
    printf("      | O D E S A  -  H O L O V N A  |\n");
    gotoxy(80,4);
    printf("      '-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.'\n\n");

    gotoxy(90,8);
    printf("       ___I_ \n");
    gotoxy(90,9);
    printf("     _|[] | |___\n");
    gotoxy(90,10);
    printf("   _|____________|___\n");
    gotoxy(90,11);
    printf("  |o o o o o o o o o /)\n");
    gotoxy(90,12);
    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    gotoxy(90,13);
    printf("    o               o\n");
    gotoxy(90,14);
    printf("   o                 o\n");
}