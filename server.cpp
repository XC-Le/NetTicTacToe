// ============================================
// Names:       Xavier Le, Gabe Gordon
// Course:      CS370-01Y | Spring 2026
// Project:     NetTicTacToe - Server
// Description: Accepts 2 client connections and manages game state, 
//              turn order, and win detection for an online Tic-Tac-Toe game.
// Due Date:    April 7, 2026
// ============================================

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 1024

// ---- Game State ----
char board[9];

void initBoard() {
    for (int i = 0; i < 9; i++) board[i] = ' ';
}