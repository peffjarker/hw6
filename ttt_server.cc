//*****************************************************************
// File: rsp_server.cc
// AUthor: David W. Juedes
// Purpose: Rock-Scissors-Paper Server
// Accepts exactly two connections from two rsp clients.
// gets two strings from two different clients.
//  ---
// returns the clients whether they won or lost.
//
// Acknowledgement:
// Based on the approach found here:
// https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T
//***************************************************************************
#include <iostream>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <vector>
#include <boost/asio.hpp>

//using namespace std;

using namespace std;
using boost::asio::ip::tcp;

bool valid(int i, int j)
{
  if (i < 0)
    return false;
  if (j < 0)
    return false;
  if (i >= 4)
    return false;
  if (j >= 4)
    return false;
  return true;
}

//
// Check to see whether player X (1 or 2) won the game!
//
bool win(vector<vector<int>> &board, int X)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      for (int k = -1; k <= 1; k++)
      {
        for (int l = -1; l <= 1; l++)
        {
          if ((k != 0) || (l != 0))
          {
            bool match = true;
            for (int m = 0; m <= 2; m++)
            {
              int i1 = i + (m * k);
              int j1 = j + (m * l);
              if (!valid(i1, j1))
              {
                match = false; // Failed out of bounds
              }
              else
              {
                if (board[i1][j1] != X)
                {
                  match = false; // Failed because wrong character
                }
              }
            }
            if (match)
              return true;
          }
        }
      }
    }
  }
  return false;
}

void RSP()
{
  boost::asio::io_service my_service;

  // Accept connections on
  // Advertise a service on port 4700
  tcp::acceptor acceptor(my_service, tcp::endpoint(tcp::v4(), 4700));

  tcp::socket socket1(my_service);
  // Wait for a connection from
  acceptor.accept(socket1);
  cout << "Connection 1 Established!!!" << endl;

  //Wait for a connection on Socket #2
  tcp::socket socket2(my_service);
  acceptor.accept(socket2);
  cout << "Connection 2 Established!!!!" << endl;

  boost::asio::write(socket1, boost::asio::buffer("x\n"));
  boost::asio::write(socket2, boost::asio::buffer("o\n"));

  // Read from Socket 1 until newline
  boost::asio::streambuf buf;
  boost::asio::read_until(socket1, buf, "\n");
  string data = boost::asio::buffer_cast<const char *>(buf.data());
  cout << data << endl;
  boost::asio::write(socket2, boost::asio::buffer(data));
  // Read from Socket #2 until newline



  // Figure out who won!
}

int main()
{
  RSP();

  return 0;
}
