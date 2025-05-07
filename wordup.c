// Name: Zachary Russell
// Date: May 6, 2025
// Purpose: Project 10
#include <stdio.h>
#include <stdbool.h>

#define MAX_GUESS 6
#define WORD_L 5
#define WORD "mystery.txt"

// Function to get the length of a string, excluding the newline character
int get_string_L(const char *str) {
    int L = 0;
    while (str[L] != '\0' && str[L] != '\n') {
        L++;
    }
    return L;
}

// Function to read the word from the file
int read_word(char *word) {
    FILE *file = fopen(WORD, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char temp_word[256];
    int i;

    if (fgets(temp_word, 256, file) != NULL) {
        size_t len = get_string_L(temp_word);
        if (len == WORD_L) {
            for (i = 0; i < WORD_L; i++) {
                word[i] = temp_word[i];
            }
            word[WORD_L] = '\0';
            fclose(file);
            return 0;
        } else {
            fclose(file);
            printf("Error: The word in the file is not 5 letters long.\n");
            return 1;
        }
    } else {
        fclose(file);
        printf("Error: Could not read the word from the file.\n");
        return 1;
    }

    fclose(file);
    return 1; // Added a return here to handle the case where fgets fails.
}

bool confirm_input(const char *guess) {
    int L = get_string_L(guess);
    if (L != WORD_L) {
        return false;
    }
    for (int i = 0; i < WORD_L; i++) {
        if (!((guess[i] >= 'a' && guess[i] <= 'z') || (guess[i] >= 'A' && guess[i] <= 'Z'))) {
            return false;
        }
    }
    return true;
}

char uppercase(char ch) {
    if (ch >= 'a' && ch <= 'z') {
        return ch - 32;
    }
    return ch;
}

void disp_result(int won, const char *word) {
    if (won) {
        printf("Congratulations! You guessed the word: %s\n", word);
    } else {
        printf("You ran out of guesses. The word was: %s\n", word);
    }
}

int string_compare_without_case(const char *str1, const char *str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        char c1 = uppercase(str1[i]);
        char c2 = uppercase(str2[i]);
        if (c1 != c2) {
            return (c1 > c2) ? 1 : -1;
        }
        i++;
    }
    if (str1[i] == '\0' && str2[i] == '\0')
        return 0;
    return (str1[i] == '\0') ? -1 : 1;
}

void disp_previous_guesses(const char previous_guesses[][WORD_L + 1], int num_guesses, const char *word) {
    printf("Previous Guesses:\n");
    for (int i = 0; i <= num_guesses; i++) {
        char disp_guess[WORD_L + 1];
        char feedback[WORD_L + 1];
        for (int k = 0; k < WORD_L + 1; k++) {
            disp_guess[k] = '\0';
            feedback[k] = '\0';
        }

        for (int j = 0; j < WORD_L; j++) {
            if (previous_guesses[i][j] == word[j]) {
                disp_guess[j] = previous_guesses[i][j];
            } else {
                disp_guess[j] = previous_guesses[i][j];
            }
        }
        disp_guess[WORD_L] = '\0';

        for (int j = 0; j < WORD_L; j++) {
            if (previous_guesses[i][j] != word[j]) {
                for (int k = 0; k < WORD_L; k++) {
                    if (uppercase(previous_guesses[i][j]) == uppercase(word[k])) {
                        int already_correct = 0;
                        for (int l = 0; l < WORD_L; l++) {
                            if (previous_guesses[i][l] == word[l]) {
                                 if (uppercase(previous_guesses[i][j]) == uppercase(word[l])) {
                                    already_correct = 1;
                                }
                            }
                        }
                        if (!already_correct)
                            feedback[j] = '^';
                    }
                }
            }
        }
        feedback[WORD_L] = '\0';

        printf("%s\n", disp_guess);
        if (get_string_L(feedback) > 0)
            printf("%s\n", feedback);
    }
}

int main() {
    char word[WORD_L + 1];
    char guess[WORD_L + 1];
    char previous_guesses[MAX_GUESS][WORD_L + 1];
    int num_guesses = 0;
    bool won = false;

    if (read_word(word) != 0) {
        printf("Failed to read the word from the file.\n");
        return 1;
    }
    printf("Welcome to the Word Guessing Game!\n");
    printf("Try to guess the %d-letter word. You have %d attempts.\n", WORD_L, MAX_GUESS);

    while (num_guesses < MAX_GUESS) {
        printf("\nGuess %d/%d: Enter your guess: ", num_guesses + 1, MAX_GUESS);
        if (scanf("%s", guess) != 1) {
            printf("Invalid input.  Exiting.\n");
            return 1;
        }

        if (!confirm_input(guess)) {
            printf("Invalid input. Please enter a %d-letter word containing only letters.\n", WORD_L);
            continue;
        }

        for (int i = 0; i < WORD_L; i++) {
            guess[i] = uppercase(guess[i]);
        }
        for (int i = 0; i < WORD_L + 1; i++)
            previous_guesses[num_guesses][i] = guess[i];

        if (string_compare_without_case(guess, word) == 0) {
            won = true;
            break;
        }

        disp_previous_guesses(previous_guesses, num_guesses, word);
        num_guesses++;
    }
    disp_result(won, word);

    return 0;
}



