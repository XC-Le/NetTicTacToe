// ============================================
// Names:           Xavier Le, Gabe Gordon
// Course:          CS370-01Y | Spring 2026
// Project:         NetTicTacToe - Client
// Description:     Connects to the server, displays the board, and 
//                  handles player input for a networked Tic-Tac-Toe game.
//
// Instructions:    Compile file with 'g++ -o client client.cpp -lws2_32'
//                  and then run './client' after compiling running the server.
// Due Date:        April 7, 2026
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
        if (c == 'X') cout << " X ";
        else if (c == 'O') cout << " O ";
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

        if(msg.substr(0,7) == "WELCOME"){
            mySymbol = msg[8];
            cout<<"You are player "<<(msg[8]=='X'?'1':'2');
            cout<<" ("<<mySymbol<<")"<<endl;
        }

        else if(msg.substr(0, 5) == "BOARD"){
            string state = msg.substr(6,15);
            for(int i=0;i<9;i++){
                board[i]=state[i];
            }
            printBoard();
        }

        else if(msg.substr(0, 9) == "YOUR_TURN"){
            int pos = -1;
            while(true){
                cout<<"Your move (0-8): ";
                cin>>pos;
                if(pos>=0 || pos<=8)break; 
                cout<<"Invalid input. Enter a number 0-8.\n";
            }
            sendMsg(sock, "MOVE " + to_string(pos));
        }

        else if(msg == "INVALID"){
            cout<<"That square is taken! Try again."<<endl;
        }

        else if(msg == "WAITING"){
            cout<<"Waiting for opponent's turn."<<endl;
        }

        else if(msg.substr(0,6) == "RESULT"){
            if(msg=="RESULT DRAW") cout<<"Draw!"<<endl; 
            else{
                char winner = msg[11];
                if(winner == mySymbol)cout<<"YOU WON!"<<endl;
                else cout<<"YOU LOST!"<<endl;
            }
            break;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;   
}