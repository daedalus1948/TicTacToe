/* Automatic TicTacToe implementation in C
Minimax algorithm for enemy AI not implemented
Optional player input not implemented
Many functions contain a generic nested for loop iterating over the board data structure - Consider creating a separate function for this
KNOWN ISSUES - find_free_moves function could be simplified, temp_index is not necesarry, 77 array prefill aswell
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* board is a square matrix, therefore only one size information is neccesary - "LENGTH" */
/* BOARD DIMENSIONS CAN BE ALTERED!! */
#define LENGTH 3 /* for board length purposes */
#define PLAYER 4
#define ENEMY 7

int evaluate_board(int leng, int entity[][LENGTH], int player) {
    int n = leng;
    int x;
    int y;
    int row, column, diag1, diag2;
    for (x = 0; x<n;x++) {
        row = 0, column = 0, diag1 = 0, diag2 = 0;
        for (y = 0; y<n;y++) {
            if (entity[x][y] == player) {
              column++;}
            if (entity[y][x] == player) {
              row++;}
            if (entity[y][y] == player) {
              diag1++;}
            if (entity[y][(n-1)-y] == player) {
              diag2++;}
        }
        if (row == n || column == n || diag1 == n || diag2 == n) {
            printf("player %d won \n", player);
            return 0;
        }
    }
    return 1;
}

/* only true setter - updating board data */
void update_state(int entity[][LENGTH], int player, int move[]) {
    int* temp_move = move;
    entity[move[0]][move[1]] = player; /* update the game board with corresponding player data */
    free(temp_move); /* free the second malloc heap array returned from generate_move() */
}

/* or "int (*entity)[LENGTH]"*/
void show_board (int leng, int entity[][LENGTH]) {
  int row, i;
  for (row = 0; row < leng; row++) {
    for (i = 0; i < leng; i++) {
      printf("%d", entity[row][i]);
    }
    printf("\n");
  }
}

int get_free_move_count (int leng, int entity[][LENGTH]) {
  int row, i;
  int count = 0;
  for (row = 0; row < leng; row++) {
    for (i = 0; i < leng; i++) {
      if (entity[row][i] == 0) {
        count++;
      }
    }
  }
  return count;
}

int* find_free_moves (int leng, int entity[][LENGTH]) {
  int count;
  int row, i;
  int* temp;
  int temp_index = 0;
  int empty;
  /* calcuate the size of malloced array - *2 because storing x,y */
  count = ((get_free_move_count(leng, entity))*2);
  /* create a 1D contiguous array for pseudo 2D access */
  temp = malloc(sizeof(int) * count+1); /* CHECK IF TEMP NOT NULL!!!!!! */
  /*prefill the malloc array with arbitrary 77, later check if [0] is not 77 to determine whether array is "empty" (77 == EMPTY)*/
  for (empty = 0; empty<count;empty++) {
    temp[empty] = 77;
  }
  for (row = 0; row < leng; row++) {
    for (i = 0; i < leng; i++) {
      if (entity[row][i] == 0) {
        temp[temp_index] = row;
        temp_index++;
        temp[temp_index] = i;
        temp_index++;
      }
    }
  }
  /* every ODD index + 1 (eg 0-1,2-3,4-5,5-6 etc) storex x,y coordinates of a free move */
  return temp;
}

int* generate_move(int leng, int entity[][LENGTH]) {
  int* free_moves;
  int free_moves_half_length;
  int random_move;
  int* selected_move;
  free_moves_half_length = (get_free_move_count(leng, entity));
  free_moves = find_free_moves(leng, entity); /* returns an int* to malloc array */
  random_move = (rand() % free_moves_half_length)*2; /* modulo with RND_MAX does not allow for normal distribution!!! CAUTION */
  selected_move = malloc(sizeof(int)*2); /* for [x,y] move coordinates  */
  selected_move[0] = free_moves[random_move]; /* random_move selectes every possible random ODD array index */
  selected_move[1] = free_moves[random_move+1]; /* [x,y] cooridantes are stored as [ODD, ODD+1] in the array */
  free(free_moves); /* remove the malloc array from heap generated by find_free_moves() */
  return selected_move;
}

int game_running (int leng, int entity[][LENGTH]) {
  int row, i;
  for (row = 0; row < leng; row++) {
    for (i = 0; i < leng; i++) {
      if (entity[row][i] == 0) {
        return 1;
      }
    }
  }
  return 0;
}

void play_game (int friend, int enemy, int entity[][LENGTH]) {
    int turn;
    turn = enemy;
    while (game_running(LENGTH,entity)) {
        printf("------\n");
        update_state(entity, turn, generate_move(LENGTH,entity));
        show_board(LENGTH, entity);
        evaluate_board(LENGTH, entity, turn);
        turn = (turn == enemy) ? friend : enemy;
    }
    return 0;
  }

/* main could accept LENGTH and FRIEND/ENEMY arguments in the future */
int main(void) {
  srand(time(NULL)); /* generate a seed for pseudo-random calculations once a game */
  int board[LENGTH][LENGTH] = {0};
  play_game(PLAYER, ENEMY, board); /* start the game */
  return 0;
}
