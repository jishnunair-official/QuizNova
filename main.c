#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    char question[256];
    char optionA[100], optionB[100], optionC[100], optionD[100];
    char correct;
} Question;

char username[50];
int score = 0;

void clear() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void loadQuestions(Question *qList, int *qCount, char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error loading questions from %s\n", filename);
        exit(1);
    }

    *qCount = 0;
    while (fscanf(fp, " %[^\n]\n %[^\n]\n %[^\n]\n %[^\n]\n %[^\n]\n %c\n",
                  qList[*qCount].question,
                  qList[*qCount].optionA,
                  qList[*qCount].optionB,
                  qList[*qCount].optionC,
                  qList[*qCount].optionD,
                  &qList[*qCount].correct) != EOF) {
        (*qCount)++;
    }
    fclose(fp);
}

void startQuiz(Question *questions, int qCount) {
    char answer;
    int i;
    for (i = 0; i < qCount; i++) {
        clear();
        printf("\nQ%d: %s\n", i + 1, questions[i].question);
        printf("A. %s\nB. %s\nC. %s\nD. %s\n", questions[i].optionA, questions[i].optionB, questions[i].optionC, questions[i].optionD);
        printf("Your answer: ");
        scanf(" %c", &answer);

        if (toupper(answer) == toupper(questions[i].correct)) {
            score += 10;
            printf("Correct!\n");
        } else {
            printf("Wrong! Correct answer: %c\n", questions[i].correct);
        }
        printf("Press Enter to continue...");
        getchar(); getchar();
    }
}

void saveHighScore() {
    FILE *fp = fopen("highscore.txt", "a");
    if (fp) {
        fprintf(fp, "%s %d\n", username, score);
        fclose(fp);
    }
}

void showHighScores() {
    FILE *fp = fopen("highscore.txt", "r");
    char name[50];
    int scr;
    printf("\n---- High Scores ----\n");
    if (fp) {
        while (fscanf(fp, "%s %d", name, &scr) != EOF) {
            printf("%s - %d\n", name, scr);
        }
        fclose(fp);
    } else {
        printf("No scores yet.\n");
    }
}

int main() {
    int choice, level;
    char category[20], filePath[50];
    Question qList[MAX];
    int qCount = 0;

    clear();
    printf("Welcome to QuizNova!\n");
    printf("Enter your name: ");
    scanf("%s", username);

    do {
        clear();
        printf("\n1. Start Quiz\n2. View High Scores\n3. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("\nSelect Category:\n1. General Knowledge\n2. Technology\nChoice: ");
            scanf("%d", &choice);
            strcpy(category, (choice == 1) ? "gk" : "tech");

            printf("\nSelect Difficulty:\n1. Easy\n2. Medium\n3. Hard\nChoice: ");
            scanf("%d", &level);
            char *levelStr = (level == 1) ? "easy" : (level == 2) ? "medium" : "hard";

            sprintf(filePath, "questions_%s_%s.txt", category, levelStr);
            loadQuestions(qList, &qCount, filePath);
            startQuiz(qList, qCount);
            saveHighScore();

            printf("\nYour final score: %d\n", score);
            printf("Press Enter to return to main menu...");
            getchar(); getchar();

            score = 0; // reset score
        } else if (choice == 2) {
            showHighScores();
            printf("Press Enter to return to menu...");
            getchar(); getchar();
        }

    } while (choice != 3);

    printf("\nThank you for playing QuizNova!\n");
    return 0;
}
