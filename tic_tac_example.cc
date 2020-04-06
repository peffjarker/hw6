//***************************************************************************
// File: tic_tac_example.cc
//
// Author: David W. Juedes
// Date: March 31st, 2020
//
// The following example shows how to use the curses
// library to display a 4x4 boards for "4x4" Tic Tac Toe, where you win if you
// get 3 X's or O's in a row.  (A bit more challenging than regular tic-tac-toe.)
//
//
// Hit a space sets the next character.
// Plays until someone wins (alternates between X's and O's).
//
// The program exists when the q key is hit.
//
//*************************************************************************** 
#include <ncurses.h>
#include <iostream>
#include <vector>

using namespace std;

bool valid(int i, int j) {
  if (i<0) return false;
  if (j<0) return false;
  if (i>=4) return false;
  if (j>=4) return false;
  return true;
}


//
// Check to see whether player X (1 or 2) won the game!
//
bool win(vector<vector<int> >&board, int X) {

  for (int i=0;i<4; i++) {
    for (int j=0;j<4;j++) {

      for (int k=-1;k<=1; k++) {
	for (int l = -1; l<=1;l++) {
	  if ((k!=0) || (l!=0)) {
	    bool match=true;
	    for (int m=0;m<=2;m++) {
	      int i1 = i+(m*k);
	      int j1 = j+(m*l);
	      if (!valid(i1,j1)){
		match =false; // Failed out of bounds
		} else {
		if (board[i1][j1]!=X) {
		  match = false; // Failed because wrong character
		}
	      }
	      
	    }
	    if (match) return true;
	  }
	}
      }
    }
  }
  return false;
}
		  


void draw_top_matrix(vector<vector<int> > &board,
		     int cur_row,
		     int cur_col) {

  for (int j=0;j<4;j++) {
      move(0,2*j);
      printw("+-");
    }
    move(0,2*4);
    printw("+");
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      move(2*i+1,2*j);
      printw("|");
      move(2*i+1,2*j+1);
      switch (board[i][j]) {
      case 0: printw(" ");  break;
      case 1: printw("X");  break;
      case 2: printw("O"); break;
      }
      
      
    }
    move(2*i+1,2*4);
    printw("|");
    for (int j=0;j<4;j++) {
      move(2*i+2,2*j);
      printw("+-");
    }
    move(2*i+2,2*4);
    printw("+");
  }
  move(2*cur_row+1,2*cur_col+1);
}
      
      


main() {
  int rows;
  int cols;
  int cur_row=0;
  int cur_col=0;
  int ch;

  vector<vector<int> > board;
  for (int i=0;i<4;i++) {
    vector<int> t;
    for (int j=0;j<4;j++) {
      t.push_back(0);
    }
    board.push_back(t);
  }
  
  // Screen initialization

  initscr();
  // Clear the screen

  clear();

  // Get the size of the window!
  getmaxyx(stdscr,rows,cols);

  
  cbreak();  // Pass all characters to this program!

  keypad(stdscr, TRUE); // Grab the special keys, arrow keys, etc.

  // Paint the row and column markers.
  //paint_markers(rows,cols,10,0,0);
  // Redraw the screen.
  refresh();


  draw_top_matrix(board,0,0);
  

  // I/O Loop....
  // Stop when the q Key is hit.
  //
  bool is_X= false;
  
  while ((ch = getch())!='q') {
    switch (ch) {
    case ' ':  if (board[cur_row][cur_col]==0) {
	if (is_X) board[cur_row][cur_col]=1;
	else  board[cur_row][cur_col]=2;
	is_X = !is_X;
      }
      // Redraw the screen.
	draw_top_matrix(board,cur_row,cur_col);
	refresh();
      

      break;
    case KEY_RIGHT:
      cur_col++;
      cur_col%=4;
      draw_top_matrix(board,cur_row,cur_col);
      // Redraw the screen.
      refresh();
      break;
    case KEY_LEFT:
      cur_col--;
      cur_col = (4+cur_col)%4;
      draw_top_matrix(board,cur_row,cur_col);
      // Redraw the screen.
      refresh();
      break;
    case KEY_UP:
      cur_row--;
      cur_row=(4+cur_row) % 4;
      draw_top_matrix(board,cur_row,cur_col);
      
      //      paint_markers(rows,cols,10,cur_row,cur_col);
      // Redraw the screen.
      refresh();
      break;
    case KEY_DOWN:
      cur_row++;
      cur_row%=4;
      draw_top_matrix(board,cur_row,cur_col);
            //paint_markers(rows,cols,10,cur_row,cur_col);
      // Redraw the screen.
      refresh();
      break;


    }
    if (win(board,1)) {
      break;
    }
    if (win(board,2)) {
      break;
    }
  }
  

  


  endwin();
  if (win(board,1)) {
    cout << "X won!" << endl;
  }
  if (win(board,2)) {
    cout << "O won!" << endl;
  }
}
