//****************************************************
// File:rsp_client.cc
// Author: David W. Juedes
// Purpose: play rock-scissors-paper over the network.
// Connects to rsp server.
// rsp server acts as referee.
// returns whether you won-lost or tied.
// Client write "rock/scissors/paper"
// Server return "win/lose/tie"
//
//**************************************************************************
#include <iostream>
#include <string>
#include <ncurses.h>
#include <vector>
#include "boost/asio.hpp"
using namespace std;
using boost::asio::ip::tcp;

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

int main(int argc, char *argv[]) {

  int rows;
  int cols;
  int cur_row=0;
  int cur_col=0;
  int ch;

  vector<vector<int>> board;
  for (int i=0;i<4;i++) {
    vector<int> t;
    for (int j=0;j<4;j++) {
      t.push_back(0);
    }
    board.push_back(t);
  }

  cout << "X args" << argc << endl;
  cout << "server" << argv[1] << endl;
  cout << "port" << argv[2] << endl;

  int portno = atoi(argv[2]);
  // Standard boost code to connect to a server.
  // Comes from the boost tutorial
  boost::asio::io_service my_service;

  tcp::resolver resolver(my_service);
  // Find the server/port number.
  //  tcp::resolver::results_type endpoints = resolver.resolve(argv[2], argv[3]);

  tcp::socket socket(my_service);
  
  socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(argv[1]),portno));
  
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
    case ' ':  
      if (board[cur_row][cur_col]==0) {
	      if (is_X) 
          board[cur_row][cur_col]=1;
	      else  
          board[cur_row][cur_col]=2;
	      is_X = !is_X;
      }
      // Redraw the screen.
	    draw_top_matrix(board,cur_row,cur_col);
      string msg = to_string(cur_row) + to_string(cur_col);
      msg += '\n';
      boost::asio::write( socket, boost::asio::buffer(msg));
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
      // paint_markers(rows,cols,10,cur_row,cur_col);
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
}
  
