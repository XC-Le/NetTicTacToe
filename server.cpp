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

using namespace std;

#define PORT 12345
#define BUFFER_SIZE 1024

// game board
char board[9];

// initialized the board
void initBoard() {
    for (int i = 0; i < 9; i++) board[i] = ' ';
}

// returns board in a string
string boardToString(){
    return string(board, 9);
}

// checks if move is valid
bool isValidMove(int pos){
    return pos>=0 && pos<9 && board[pos] == ' ';
}

// applies move to board
void applyMove(int pos, char symbol){
    board[pos] = symbol;
}

// sends a message to client based off sock
void sendMsg(SOCKET sock, string& msg){
    send(sock, msg.c_str(), (int) msg.size(), 0);
}

// receives a message from sock
string receiveMsg(SOCKET sock){
    char buf[BUFFER_SIZE] = {0};
    int bytes = recv(sock, buf, BUFFER_SIZE-1, 0);
    if(bytes <= 0) return "";
    return string(buf, bytes);
}


int main(){
    // initializes WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed.\n";
        return 1;
    }

    // creates a socket to listen on
    SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ListenSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
    }

    // creates address and binds to the socket and port
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(ListenSocket, (sockaddr*)&addr, sizeof(addr));
    listen(ListenSocket, 2);


    SOCKET ClientSocket1 = accept(ListenSocket, nullptr, nullptr);

    SOCKET ClientSocket2 = accept(ListenSocket, nullptr, nullptr);


    return 0;
}

// determine player 1

// determine player 2