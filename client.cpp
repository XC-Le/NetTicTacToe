// ============================================
// Names:       Xavier Le, Gabe Gordon
// Course:      CS370-01Y | Spring 2026
// Project:     NetTicTacToe - Client
// Description: Connects to the server, displays the board, and 
//              handles player input for a networked Tic-Tac-Toe game.
// Due Date:    April 7, 2026
// ============================================

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 1024

char board[9];
char mySymbol = '?';

void printBoard() {
    std::cout << "\n";
    for (int i = 0; i < 9; i++) {
        char c = board[i];
        if (c == '1') std::cout << " X ";
        else if (c == '2') std::cout << " O ";
        else std::cout << " " << i << " "; 

        if (i % 3 != 2) std::cout << "|";
        else if (i < 8) std::cout << "\n---+---+---\n";
    }
    std::cout << "\n\n";
}