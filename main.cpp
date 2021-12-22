#include <iostream>
#include <iomanip>
#include <random>

using namespace std;

// probability of randomly choosing 2 for a cell:
// PROB_2_NUM / PROB_2_DEN

const int
    PROB_2_NUM = 9,
    PROB_2_DEN = 10;

//==============================================================
// void displayBoard(int board[4][4],int score)
//  clear screen and display board and score
//
// Parameters
// board - the game board to be displayed
// score - current game score
//

void displayBoard(int board[4][4],int score) {
    // clear screen
    cout << "\033[2J\033[H";

    // display score
    cout << "Score: " << score << endl;

    // display board, one row at a time
    for (int row=0;row<4;row++) {
        // draw top of box and top row of space
        cout << "+--------+--------+--------+--------+\n"
                "|        |        |        |        |" << endl;

        // draw the row with numbers
        for (int col=0;col<4;col++) {
            cout << "| ";

            if (board[row][col] == 0)
                cout << "       ";
            else
                cout << setw(6) << board[row][col] << ' ';
        }

        // draw end of numbers row and bottom row of space
        cout << "|\n|        |        |        |        |"
            << endl;
    }

    // draw the bottom of game board
    cout << "+--------+--------+--------+--------+" << endl;
}

//==============================================================
// void fillCell(int board[4][4])
//  pick random empty cell in board and put 2 or 4 there
//
// Parameter
// board - game board to update
//

void fillCell(int board[4][4]) {
    int
        nEmpty = 0;
    static random_device
        rd;
    static mt19937
        mt(rd());

    // count number of empty cells (nEmpty)
    for (int row=0;row<4;row++)
        for (int col=0;col<4;col++)
            if (board[row][col] == 0)
                nEmpty++;

    // if nEmpty == 0, stop
    if (nEmpty == 0)
        return;

    // pick random number 1 -- nEmpty
    uniform_int_distribution<>
        dis(1,nEmpty);
    int
        pick = dis(mt);

    // scan board, stop when empty cell count == random pick
    int
        count = 0;
    for (int row=0;row<4;row++) {
        for (int col=0;col<4;col++) {
            if (board[row][col] == 0) {
                count++;
                if (count == pick) {
                    // randomly choose 2 or 4, place in that cell
                    uniform_int_distribution<>
                        dis2or4(1,PROB_2_DEN);

                    if (dis2or4(mt) <= PROB_2_NUM)
                        board[row][col] = 2;
                    else
                        board[row][col] = 4;
                    // stop
                    return;

                }
            }
        }
    }
}

bool canSlideLeft(int board[4][4]) {
    // check all rows
    for (int row=0; row<4; row++) {
        // cols get 3, 2, 1
        for (int col=3; col>0; col--) {
            // skip if empty
            if (board[row][col] == 0)
                continue;

            // can we merge?
            if (board[row][col] == board[row][col-1])
                return true;

            // can we slide?
            if (board[row][col-1] == 0)
                return true;
        }
    }

    // if we get here, we cannot slide
    return false;
}

bool canSlideRight(int board[4][4]) {
    // check all rows
    for (int row=0; row<4; row++) {
        // cols get 2, 1, 0
        for (int col=2; col>=0; col--) {
            // skip if empty
            if (board[row][col] == 0)
                continue;

            // can we merge?
            if (board[row][col] == board[row][col+1])
                return true;

            // can we slide?
            if (board[row][col+1] == 0)
                return true;
        }
    }

    // if we get here, we cannot slide
    return false;
}

bool canSlideUp(int board[4][4]) {
    // check all columns
    for (int col=0; col<4; col++) {
        // rows get 3, 2, 1
        for (int row=3; row>0; row--) {
            // skip if empty
            if (board[row][col] == 0)
                continue;

            // can we merge?
            if (board[row][col] == board[row-1][col])
                return true;

            // can we slide?
            if (board[row-1][col] == 0)
                return true;
        }
    }

    // if we get here, we cannot slide
    return false;
}

bool canSlideDown(int board[4][4]) {
    // check all columns
    for (int col=0; col<4; col++) {
        // rows get 2, 1, 0
        for (int row=2; row>=0; row--) {
            // skip if empty
            if (board[row][col] == 0)
                continue;

            // can we merge?
            if (board[row][col] == board[row+1][col])
                return true;

            // can we slide?
            if (board[row+1][col] == 0)
                return true;
        }
    }

    // if we get here, we cannot slide
    return false;
}

bool canUndo(int board[4][4], int temp[4][4], int count, char prevChoice, char choice) {
    // undo is not allowed to be done on the first move of the game
    if (count == 0)
        return false;

    // undo is not allowed if the previous move was undo
    if (prevChoice == 'z' || prevChoice == 'Z')
        return false;

    // undo is not allowed if the previous array = the current array
    // prevents user from continuously pressing undo to get around not being able to undo due to the above if statement
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if ((board[i][j] == temp[i][j]) && (choice == 'z' || choice == 'Z'))
                return false;

    return true;
}


void slideLeft(int board[4][4], int &score) {
    int
        dest,
        hold;

    // loop over all rows
    for (int row=0; row<4; row++) {

        // initialize hold and dest
        dest = 0;

        hold = -1;

        // loop over all columns, left to right
        for (int col=0; col<4; col++) {

            // if cell not empty, test...
            if (board[row][col] != 0) {

                // does cell match hold?
                if (board[row][col] == hold) {

                    // double the hold
                    hold = 2 * hold;

                    // update the score
                    score = score + hold;

                    // place hold in destination cell
                    board[row][dest] = hold;

                    // update destination
                    dest++;

                    // clear hold
                    hold = -1;

                // otherwise, is the hold not empty
                } else if (hold != -1) {

                    // place hold in destination cell
                    board[row][dest] = hold;

                    // update destination
                    dest++;

                    // replace hold with cell
                    hold = board[row][col];


                // otherwise, hold is empty
                } else {

                    // replace hold with cell
                    hold = board[row][col];
                }

            // end if != 0
            }

        // end col loop
        }

        // check to see if hold is empty
        if (hold != -1) {

            // place hold in destination cell
            board[row][dest] = hold;

            // update destination
            dest++;
        }

        // clear the rest of the row
        while (dest < 4) {

            // place hold in destination cell
            board[row][dest] = 0;

            // update destination
            dest++;
        }

    // end row loop
    }
}

void slideRight(int board[4][4], int &score) {
    int
            dest,
            hold;

    // loop over all rows
    for (int row=0; row<4; row++) {

        // initialize hold and dest
        dest = 3;

        hold = -1;

        // loop over all columns, right to left
        for (int col=3; col>=0; col--) {

            // if cell not empty, test...
            if (board[row][col] != 0) {

                // does cell match hold?
                if (board[row][col] == hold) {

                    // double the hold
                    hold = 2 * hold;

                    // update the score
                    score = score + hold;

                    // place hold in destination cell
                    board[row][dest] = hold;

                    // update destination
                    dest--;

                    // clear hold
                    hold = -1;

                    // otherwise, is the hold not empty
                } else if (hold != -1) {

                    // place hold in destination cell
                    board[row][dest] = hold;

                    // update destination
                    dest--;

                    // replace hold with cell
                    hold = board[row][col];


                    // otherwise, hold is empty
                } else {

                    // replace hold with cell
                    hold = board[row][col];
                }

                // end if != 0
            }

            // end col loop
        }

        // check to see if hold is empty
        if (hold != -1) {

            // place hold in destination cell
            board[row][dest] = hold;

            // update destination
            dest--;
        }

        // clear the rest of the row
        while (dest >= 0) {

            // place hold in destination cell
            board[row][dest] = 0;

            // update destination
            dest--;
        }

        // end row loop
    }
}

void slideUp(int board[4][4], int &score) {
    int
            dest,
            hold;

    // loop over all columns
    for (int col=0; col<4; col++) {

        // initialize hold and dest
        dest = 0;

        hold = -1;

        // loop over all rows, up to down
        for (int row=0; row<4; row++) {

            // if cell not empty, test...
            if (board[row][col] != 0) {

                // does cell match hold?
                if (board[row][col] == hold) {

                    // double the hold
                    hold = 2 * hold;

                    // update the score
                    score = score + hold;

                    // place hold in destination cell
                    board[dest][col] = hold;

                    // update destination
                    dest++;

                    // clear hold
                    hold = -1;

                    // otherwise, is the hold not empty
                } else if (hold != -1) {

                    // place hold in destination cell
                    board[dest][col] = hold;

                    // update destination
                    dest++;

                    // replace hold with cell
                    hold = board[row][col];


                    // otherwise, hold is empty
                } else {

                    // replace hold with cell
                    hold = board[row][col];
                }

                // end if != 0
            }

            // end col loop
        }

        // check to see if hold is empty
        if (hold != -1) {

            // place hold in destination cell
            board[dest][col] = hold;

            // update destination
            dest++;
        }

        // clear the rest of the row
        while (dest < 4) {

            // place hold in destination cell
            board[dest][col] = 0;

            // update destination
            dest++;
        }

        // end row loop
    }
}

void slideDown(int board[4][4], int &score) {
    int
            dest,
            hold;

    // loop over all columns
    for (int col=0; col<4; col++) {

        // initialize hold and dest
        dest = 3;

        hold = -1;

        // loop over all rows, down to up
        for (int row=3; row>=0; row--) {

            // if cell not empty, test...
            if (board[row][col] != 0) {

                // does cell match hold?
                if (board[row][col] == hold) {

                    // double the hold
                    hold = 2 * hold;

                    // update the score
                    score = score + hold;

                    // place hold in destination cell
                    board[dest][col] = hold;

                    // update destination
                    dest--;

                    // clear hold
                    hold = -1;

                    // otherwise, is the hold not empty
                } else if (hold != -1) {

                    // place hold in destination cell
                    board[dest][col] = hold;

                    // update destination
                    dest--;

                    // replace hold with cell
                    hold = board[row][col];


                    // otherwise, hold is empty
                } else {

                    // replace hold with cell
                    hold = board[row][col];
                }

                // end if != 0
            }

            // end col loop
        }

        // check to see if hold is empty
        if (hold != -1) {

            // place hold in destination cell
            board[dest][col] = hold;

            // update destination
            dest--;
        }

        // clear the rest of the row
        while (dest >= 0) {

            // place hold in destination cell
            board[dest][col] = 0;

            // update destination
            dest--;
        }

        // end row loop
    }
}

void undo(int board[4][4], int temp[4][4], int &score, int prevScore[2]) {
    // replace the current board with the board from one move earlier
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            board[i][j] = temp[i][j];

    // update the score to be the score from the previous turn
    score = prevScore[0];
}

int main() {
    int
        board[4][4] = {0},
        temp[4][4] = {0},
        score = 0,
        prevScore[2] = {0},
        count = 0;
    char
        choice = 'g',
        prevChoice = 'g';
    bool
        leftOkay, rightOkay, upOkay, downOkay, undoOkay;

    // initialize game board
    // a. clear all positions (done)
    // b. choose empty cell randomly
    // c. randomly put 2 or 4 in that cell
    // d. repeat b and c one time
    fillCell(board);
    fillCell(board);

    // start do loop
    do {
        // see if player can slide and if player can undo
        leftOkay = canSlideLeft(board);
        rightOkay = canSlideRight(board);
        upOkay = canSlideUp(board);
        downOkay = canSlideDown(board);
        undoOkay = canUndo(board, temp, count, prevChoice, choice);

        // if player cannot slide and player cannot undo,
        // game is over
        if (!leftOkay && !rightOkay && !upOkay && !downOkay && !undoOkay)
            break;

        // display board and score
        displayBoard(board,score);

        // store the previous choice before making new choice
        prevChoice = choice;

        // stores the previous score in the 1st element of the array and the current score in the 2nd element
        if (prevScore[1] != score)
            prevScore[0] = prevScore[1];
        else
            prevScore[0] = score;

        prevScore[1] = score;

        // get choice
        cout << "Enter move [lrudzq]: ";
        cin >> choice;


        // process choice
        switch (choice) {
            case 'l':
            case 'L':
                if (leftOkay) {
                    // copy the current board to the temp array right before sliding left
                    for (int i = 0; i < 4; i++)
                      for (int j = 0; j < 4; j++)
                        temp[i][j] = board[i][j];

                    // slide left and fill in a random, empty square
                    slideLeft(board, score);
                    fillCell(board);
                }
                break;
            case 'r':
            case 'R':
                if (rightOkay) {
                    // copy the current board to the temp array right before sliding right
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            temp[i][j] = board[i][j];

                    // slide left and fill in a random, empty square
                    slideRight(board, score);
                    fillCell(board);
                }
                break;
            case 'u':
            case 'U':
                if (upOkay) {
                    // copy the current board to the temp array right before sliding up
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            temp[i][j] = board[i][j];

                    // slide left and fill in a random, empty square
                    slideUp(board, score);
                    fillCell(board);
                }
                break;
            case 'd':
            case 'D':
                if (downOkay) {
                    // copy the current board to the temp array right before sliding down
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            temp[i][j] = board[i][j];

                    // slide left and fill in a random, empty square
                    slideDown(board, score);
                    fillCell(board);
                }
                break;
            case 'q':
            case 'Q':
                break;
            case 'z':
            case 'Z':
                if (undoOkay) {
                    undo(board, temp, score, prevScore);
                }
                else
                    cout << "You cannot undo during this turn\n";
                break;
            default:
                cout << "Invalid move '" << choice << "'" << endl;
        }

        // increment count when count = 0 and undo was not previously typed
        // this prevents undo from being used right at the start of the game
        if (count == 0 && choice != 'z' && choice != 'Z')
            count++;

    // end do loop
    } while (choice != 'q' && choice != 'Q');

    // display final board and score
    displayBoard(board,score);

    return 0;
}