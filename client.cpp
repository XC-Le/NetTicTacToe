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

using namespace std;

#define PORT 12345
#define BUFFER_SIZE 1024

char board[9];
char mySymbol = '?';

void printBoard() {
    cout << "\n";
    for (int i = 0; i < 9; i++) {
        char c = board[i];
        if (c == '1') cout << " X ";
        else if (c == '2') cout << " O ";
        else cout << " " << i << " "; 

        if (i % 3 != 2) cout << "|";
        else if (i < 8) cout << "\n---+---+---\n";
    }
    cout << "\n\n";
}

// sends a message to client based off sock
void sendMsg(SOCKET sock, const string& msg){
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

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed.\n";
        return 1;
    }
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed.\n";
        WSACleanup();
        return 1;
    }
 
    std::cout << "Connected to server!\n";

    
    while(true){
        string msg = receiveMsg(sock);
        if(msg.empty()){
            cout<<"Server disconected"<<endl;
            break;
        }

        if(msg.substr(0,7)=="Welcome"){
            mySymbol = msg[15]=='1'?'X':'O';
            cout<<"You are player "<<msg[15];
            cout<<" ("<<mySymbol<<")"<<endl;
        }

        
    }

    return 0;   
}