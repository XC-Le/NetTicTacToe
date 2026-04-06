// ============================================
// Names:           Xavier Le, Gabe Gordon
// Course:          CS370-01Y | Spring 2026
// Project:         NetTicTacToe - Server
// Description:     Accepts 2 client connections and manages game state, 
//                  turn order, and win detection for an online Tic-Tac-Toe game.
//
// Instructions:    Compile file with 'g++ -o server server.cpp -lws2_32' and 
//                  then run './server' before compiling and running the client
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
void sendMsg(SOCKET sock, const string& msg){
    if (send(sock, msg.c_str(), (int) msg.size(), 0) != msg.size()) {
        cout<<"Issue sending message."<<endl;
        exit(1);
    }
}

// receives a message from sock
string receiveMsg(SOCKET sock){
    char buf[BUFFER_SIZE] = {0};
    int bytes = recv(sock, buf, BUFFER_SIZE-1, 0);
    if(bytes <= 0) return "";
    return string(buf, bytes);
}

char checkGameWon(){
    int winConditions[8][3] = {{0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {6,4,2}};

    for(auto& w : winConditions){
        if(board[w[0]] != ' ' &&
        board[w[0]] == board[w[1]] &&
        board[w[1]] == board[w[2]])
        return board[w[0]];
    }

    for(int i=0;i<9;i++){
        if(board[i] == ' ')return '\0';
    }

    return 'D';
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

    if (bind(ListenSocket, (sockaddr*)&addr, sizeof(addr))) {
        cout<<"Failed to bind"<<endl;
        return 1;
    }
    if (listen(ListenSocket, 2)) {
        cout<<"Failed to listen"<<endl;
        return 1;
    }


    SOCKET client1 = accept(ListenSocket, nullptr, nullptr);
    if (client1 == INVALID_SOCKET) {
        cout<<"Failed to accept"<<endl;
        return 1;
    }
    cout<<"Player 1 connected"<<endl;
    sendMsg(client1, "WELCOME X");
    SOCKET client2 = accept(ListenSocket, nullptr, nullptr);
    if (client2 == INVALID_SOCKET) {
        cout<<"Failed to accept"<<endl;
        return 1;
    }
    cout<<"Player 2 connected"<<endl;
    sendMsg(client2, "WELCOME O");

    SOCKET clients[2] = {client1, client2};
    char symbols[2] = {'X', 'O'};

    initBoard();
    sendMsg(client1, "BOARD " + boardToString());
    sendMsg(client2, "BOARD " + boardToString());

    bool turn = true;
    while(true){
        SOCKET current = clients[turn?0:1];
        SOCKET waiting = clients[turn?1:0];

        sendMsg(current, "YOUR_TURN");
        sendMsg(waiting, "WAITING");

        string msg = receiveMsg(current);
        if(msg.empty()){
            cout<<"Player disconected"<<endl;
            break;
        }

        int pos = -1;
        if(msg.substr(0,4) == "MOVE"){
            pos=stoi(msg.substr(5));
        }

        if(!isValidMove(pos)){
            sendMsg(current, "INVALID");
            continue;
        }

        applyMove(pos, symbols[turn?0:1]);

        sendMsg(client1, "BOARD " + boardToString());
        sendMsg(client2, "BOARD " + boardToString());

        char gameState = checkGameWon();
        if (gameState != '\0') {
            if (gameState == 'D') {
                sendMsg(client1, "RESULT DRAW");
                sendMsg(client2, "RESULT DRAW");
            } else {
                sendMsg(client1, std::string("RESULT WIN ") + gameState);
                sendMsg(client2, std::string("RESULT WIN ") + gameState);
            }
            break;
        }

        
        turn=!turn;


    }

    closesocket(client1);
    closesocket(client2);
    closesocket(ListenSocket);
    WSACleanup();
    std::cout << "Game over. Server shutting down.\n";
    return 0;
}
