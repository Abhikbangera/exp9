#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 20
#define MAX_LEN 20

char prod[MAX][MAX_LEN];
char items[MAX][MAX][MAX_LEN];
int itemCount[MAX];

int n; // number of productions

// check if item already exists
int exists(int state, char *str) {
    for (int i = 0; i < itemCount[state]; i++) {
        if (strcmp(items[state][i], str) == 0)
            return 1;
    }
    return 0;
}

// add item to state
void addItem(int state, char *str) {
    if (!exists(state, str)) {
        strcpy(items[state][itemCount[state]++], str);
    }
}

// closure function
void closure(int state) {
    int changed = 1;

    while (changed) {
        changed = 0;

        for (int i = 0; i < itemCount[state]; i++) {
            char *dot = strchr(items[state][i], '.');

            if (dot && *(dot + 1) != '\0') {
                char B = *(dot + 1);

                // if non-terminal
                if (B >= 'A' && B <= 'Z') {
                    for (int j = 0; j < n; j++) {
                        if (prod[j][0] == B) {
                            char newItem[MAX_LEN];
                            sprintf(newItem, "%c->.%s", B, prod[j] + 3);

                            if (!exists(state, newItem)) {
                                addItem(state, newItem);
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

// goto function
int gotoState(int from, char symbol, int to) {
    for (int i = 0; i < itemCount[from]; i++) {
        char temp[MAX_LEN];
        strcpy(temp, items[from][i]);

        char *dot = strchr(temp, '.');

        if (dot && *(dot + 1) == symbol) {
            // swap dot and symbol
            char t = *dot;
            *dot = *(dot + 1);
            *(dot + 1) = t;

            addItem(to, temp);
        }
    }

    if (itemCount[to] == 0)
        return 0;

    closure(to);
    return 1;
}

// compare states
int sameState(int a, int b) {
    if (itemCount[a] != itemCount[b]) return 0;

    for (int i = 0; i < itemCount[a]; i++) {
        if (!exists(b, items[a][i]))
            return 0;
    }
    return 1;
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (format A->BC):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", prod[i]);
    }

    // Augmented grammar
    char start = prod[0][0];

    printf("\nAugmented Grammar:\n");
    printf("Z->%c\n", start);
    for (int i = 0; i < n; i++) {
        printf("%s\n", prod[i]);
    }

    int states = 0;

    // Initial item
    char temp[MAX_LEN];
    sprintf(temp, "Z->.%c", start);

    addItem(0, temp);
    closure(0);
    states++;

    int processed = 0;

    while (processed < states) {
        for (char ch = 'A'; ch <= 'Z'; ch++) {
            int newState = states;

            if (gotoState(processed, ch, newState)) {

                int found = -1;
                for (int k = 0; k < states; k++) {
                    if (sameState(k, newState)) {
                        found = k;
                        break;
                    }
                }

                if (found == -1) {
                    states++;
                } else {
                    itemCount[newState] = 0;
                }
            }
        }

        for (char ch = 'a'; ch <= 'z'; ch++) {
            int newState = states;

            if (gotoState(processed, ch, newState)) {

                int found = -1;
                for (int k = 0; k < states; k++) {
                    if (sameState(k, newState)) {
                        found = k;
                        break;
                    }
                }

                if (found == -1) {
                    states++;
                } else {
                    itemCount[newState] = 0;
                }
            }
        }

        processed++;
    }

    // print states
    printf("\nLR(0) Item Sets:\n");
    for (int i = 0; i < states; i++) {
        printf("\nI%d:\n", i);
        for (int j = 0; j < itemCount[i]; j++) {
            printf("%s\n", items[i][j]);
        }
    }

    return 0;
}
