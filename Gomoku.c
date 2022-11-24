#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 15

const char HORZ = 196;
const char VERT = 179;
const char TL = 218;
const char TM = 194;
const char TR = 191;
const char BL = 192;
const char BM = 193;
const char BR = 217;
const int numW = 3;  // Width for writing out an integer
const int charW = 1; // Width for writing out a character
const int numPerLine = 15;
const char VERT_START = 195;
const char VERT_END = 180;
const char VERT_MID = 197;
const char *inputCommand = "Previous move/Next move/Stop [p/n/s]: ";
const char *endOfHistory = "This is the end of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const char *startOfGame = "This is the start of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const char *invalidInput = "Illegal input, please try again: ";

enum Stone
{
    NA,
    X,
    O
};

struct Move
{
    int row;
    int col;
    enum Stone stone;
};

static char *lineString(char left, char mid, char right, char horz, int wcell, int ncell)
{
    static char result[100];
    strcpy(result, "");
    char cell[10];
    for (int i = 0; i < wcell; i++)
    {
        cell[i] = horz;
    }
    cell[wcell] = '\0';
    strncat(result, &left, 1);
    char cellMid[20];
    strcpy(cellMid, cell);
    strncat(cellMid, &mid, 1);
    for (int i = 0; i < ncell - 1; i++)
        strcat(result, cellMid);
    char cellRight[20];
    strcpy(cellRight, cell);
    strncat(cellRight, &right, 1);
    strcat(result, cellRight);
    return result;
}

void displayBoard(enum Stone arr[][15], int size)
{
    // UPPERLINE
    printf("   ");
    for (int i = 0; i < size; i++)
    {
        printf("  %c ", (char)(i + 'a'));
    }
    printf("\n");
    printf("   %s\n", lineString(TL, TM, TR, HORZ, 3, numPerLine));

    // MIDDLE
    for (int i = 0; i < size; i++)
    {
        printf("%2d %c", size - i, VERT);
        for (int j = 0; j < size; j++)
        {
            if (arr[i][j] == NA)
                printf("   %c", VERT);
            if (arr[i][j] == X)
                printf(" X %c", VERT);
            if (arr[i][j] == O)
                printf(" O %c", VERT);
        }
        printf("\n");
        if (i != size - 1)
            printf("   %s\n", lineString(VERT_START, VERT_MID, VERT_END, HORZ, 3, numPerLine));
    }

    // UNDERLINE
    printf("   %s\n", lineString(BL, BM, BR, HORZ, 3, numPerLine));
}

void displayBoardSimple(enum Stone arr[][15], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d", arr[i][j]);
        }
    }
    printf("\n");
}

bool makeMove(enum Stone board[][15], int size, char *playerMove, bool isFirstPlayerTurn)
{
    // BEGIN TODO
    // The format of playerMove is "1a", "2b", "3c", ..., "15o"
    // The first and sometimes second character is the row number, the next character is the column letter.
    // The row number start from 1, the column letter start from 'a'.
    // If the move is valid, update the board and return true.
    // If the move is invalid, return false.

    // Declare variables
    int row, col;

    // format the input
    if (strlen(playerMove) > 3)
    {
        return false;
    }
    else if (strlen(playerMove) == 3)
    {
        char rot[2];
        strncpy(rot, playerMove, 2);
        row = atoi(rot - 0);
        col = playerMove[2] - 'a';
    }
    else if (strlen(playerMove) == 2)
    {
        row = playerMove[0] - '0';
        col = playerMove[1] - 'a';
    }
    else
    {
        return false;
    }

    // Condition for a legal move
    if (row > 15 || col > 15)
    {
        return false;
    }
    if (isFirstPlayerTurn)
    {
        board[15 - row][col] = X;
    }
    else
    {
        board[15 - row][col] = O;
    }
    return true;
    // END TODO
}

bool hasWon(enum Stone board[][15], int size, bool isFirstPlayerTurn)
{
    // BEGIN TODO
    enum Stone stone;
    if (isFirstPlayerTurn)
        stone = X;
    else
        stone = O;
    // Check horizontal
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size - 4; j++)
        {
            // five in a row horizontally with the same stone type (X or O)
            if (board[i][j] == stone && board[i][j + 1] == stone && board[i][j + 2] == stone && board[i][j + 3] == stone && board[i][j + 4] == stone)
            {
                if (board[i][j + 5] == stone || board[i][j - 1] == stone)
                    return false;
                else
                {
                    if (board[i][j - 1] != stone)
                    {
                        if (board[i][j + 5] == NA)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if (board[i][j + 5] != stone)
                    {
                        if (board[i][j - 1] == NA)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                        return true;
                }
            }
        }
    }
    // Check vertical
    for (int i = 0; i < size - 4; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // five in a row from top to bottom i = row, j = col (i = 0, j = 0) -> (i = 4, j = 0)
            if (board[i][j] == stone && board[i + 1][j] == stone && board[i + 2][j] == stone && board[i + 3][j] == stone && board[i + 4][j] == stone)
            {
                // check above or below, if one of the two is stone, return false (i = 5, j = 0) because this make six in a row which violate the winning conditions
                if (board[i + 5][j] == stone || board[i - 1][j] == stone)
                    return false;
                else //
                {
                    // check above, if it is not stone, consider the below
                    if (board[i - 1][j] != stone)
                    {
                        // if the bellow is NA, then the checks in five row is not blocked, return true
                        if (board[i + 5][j] == NA)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if (board[i + 5][j] != stone)
                    {
                        if (board[i - 1][j] == NA)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                        return true;
                }
            }
        }
    }
    // Check diagonal with positive slope
    for (int i = 0; i < size - 4; i++)
    {
        for (int j = 0; j < size - 4; j++)
        {
            if (board[i][j] == stone && board[i + 1][j + 1] == stone && board[i + 2][j + 2] == stone && board[i + 3][j + 3] == stone && board[i + 4][j + 4] == stone)
            {
                if (board[i + 5][j + 5] == stone || board[i - 1][j - 1] == stone)
                    return false;
                else
                {
                    if (board[i - 1][j - 1] != stone)
                    {
                        if (board[i + 5][j + 5] == NA)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if (board[i + 5][j + 5] != stone)
                    {
                        if (board[i - 1][j - 1] == NA)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                        return true;
                }
            }
        }
    }
    // Check diagonal with negative slope
    for (int i = 0; i < size - 4; i++)
    {
        for (int j = 4; j < size; j++)
        {
            if (board[i][j] == stone && board[i + 1][j - 1] == stone && board[i + 2][j - 2] == stone && board[i + 3][j - 3] == stone && board[i + 4][j - 4] == stone)
            {
                if (board[i + 5][j - 5] == stone || board[i - 1][j + 1] == stone)
                    return false;
                else
                {
                    if (board[i - 1][j + 1] != stone)
                    {
                        if (board[i + 5][j - 5] == NA)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if (board[i + 5][j - 5] != stone)
                    {
                        if (board[i - 1][j + 1] == NA)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                        return true;
                }
            }
        }
    }
    return false;
    // END TODO
}

void displayHistory(char *history, int numOfMoves)
{
    // BEGIN TODO
    struct Move moves[100];
    int count = 0;
    for (int i = 0; i < numOfMoves; i++)
    {
        char temp[3];
        strncpy(temp, history + 1 + 3 * i, 2);
        long rot = strtol(temp, NULL, 10);
        int row = rot - 1;
        int col = history[0 + 3 * i] - 'a';
        if (i % 2 == 0)
            moves[i].stone = X;
        else
            moves[i].stone = O;
        moves[i].row = row;
        moves[i].col = col;
    }
    for (int i = 0; i < numOfMoves; i++)
    {
        if (i % 2 == 0)
        {
            count++;
            printf("%d. ", count);
        }
        printf("%c%d ", moves[i].col + 'a', moves[i].row + 1);
        if (i % 2 == 1)
            printf("\n");
    }
    // END TODO
}

void startGame()
{
    enum Stone game[15][15];
    for (int i = 0; i < MAX_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZE; j++)
        {
            game[i][j] = NA;
        }
    }
    displayBoard(game, MAX_SIZE);
    char playerMove[50];
    bool player1Turn = true;
    printf("Player 1 turn: ");
    while (true)
    {
        scanf("%s", playerMove);
        if (strcmp(playerMove, "ff") == 0)
        {
            printf("%s", ((player1Turn) ? "Player 2 won " : "Player 1 won "));
            break;
        }
        else if (makeMove(game, MAX_SIZE, playerMove, player1Turn))
        {
            if (hasWon(game, MAX_SIZE, player1Turn))
            {
                displayBoard(game, MAX_SIZE);
                printf("%s", ((player1Turn) ? "Player 1 won " : "Player 2 won "));
                return;
            }
            player1Turn = !player1Turn;
            displayBoard(game, MAX_SIZE);
            printf("%s", ((player1Turn) ? "Player 1 turn: " : "Player 2 turn: "));
        }
        else
        {
            printf("Illegal move, please try again: ");
        }
    }
}

int main()
{
    printf("Welcome to Gomoku!\n");
    printf("1. Play game\n");
    printf("2. History\n");
    printf("3. Exit\n");
    printf("Please select mode [1/2/3]: ");
    int mode;
    while (true)
    {
        scanf("%d", &mode);
        if (mode == 1)
        {
            startGame();
            return 0;
        }
        else if (mode == 2)
        {
            int numOfMoves;
            printf("Please enter number of moves: ");
            scanf("%d", &numOfMoves);
            char history[700];
            printf("Please enter history: ");
            scanf("%s", history);
            displayHistory(history, numOfMoves);
            return 0;
        }
        else if (mode == 3)
        {
            return 0;
        }
        else
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Invalid mode, please try again: ");
        }
    }

    return 0;
}