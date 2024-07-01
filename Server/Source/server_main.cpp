#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <Windows.h>
#include "../Game/match_game.h"
#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main(void) {
    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsok = WSAStartup(ver, &wsData);

    //Any problems, close down.
    if (wsok != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    //Any problems, close down.
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Bind socket to IP and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    serverAddr.sin_port = htons(12345);      // Port number

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        //Any problems, close down.
        std::cerr << "Bind failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        //Any problems, close down.
        std::cerr << "Listen failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    /**************************************************************************************************************/
    std::cout << "Server waiting for incoming connections..." << std::endl;

    // Accept connection
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    system("cls"); // clear screen

    std::cout << "*** You are the Server ***" << std::endl << std::endl;

    // Sending rules to client
    send(clientSocket, (get_rules().c_str()), strlen(get_rules().c_str()), 0);
    
    // Printing rules on server
    std::cout << get_rules() << std::endl;
    std::cout << "Waiting for client to start..." << std::endl;
    
    /**************************************************************************************************************/ // Printing board

    char grid[NUM_CARDS] = { 'a', 'a', 'b', 'b', 'c', 'c', 'd', 'd', 'e', 'e', 'f', 'f', 'g', 'g', 'h', 'h' };
    int high = NUM_CARDS - 1;
    int low = 0;

    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
        int random = rand() % (high - low + 1) + low;
        int random2 = rand() % (high - low + 1) + low;
        char temp = grid[random];
        grid[random] = grid[random2];
        grid[random2] = temp;
    }

    std::string board[NUM_ROWS][NUM_COLS];
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            board[row][col] = grid[row * NUM_COLS + col];
        }
    }

    // Receive data from client
    char buffer[1024] = { 0 };
    int valread = recv(clientSocket, buffer, 1024, 0);
    system("cls");
    std::cout << "System: " << buffer << std::endl;

    int num_turns = 0;
    int matched_pairs = 0;
    int selected1 = -1, selected2 = -1;

    char row1, col1, row2, col2;
    int index1, index2;

    // Printing the board
    std::cout << "Turn " << num_turns + 1 << ":" << std::endl;
    std::cout << get_board(board, selected1, selected2) << std::endl;

    while (matched_pairs < NUM_CARDS / 2) {

        /**********************************************************************************************************/
        while (true) {
            std::cout << "Enter the location of the first card: ";
            std::cin >> row1 >> col1;

            index1 = get_index(row1, col1);
            if (index1 == -1) {
                std::cout << "Invalid location. Please enter a valid location." << std::endl;
                continue;
            }

            std::cout << "Enter the location of the second card: ";
            std::cin >> row2 >> col2;

            index2 = get_index(row2, col2);
            if (index2 == -1) {
                std::cout << "Invalid location. Please enter a valid location." << std::endl;
                continue;
            }
            if (index1 == index2) {
                std::cout << "You cannot select the same card twice. Please select a different card." << std::endl;
                continue;
            }

            std::string card1 = board[index1 / NUM_COLS][index1 % NUM_COLS];
            std::string card2 = board[index2 / NUM_COLS][index2 % NUM_COLS];

            if (card1 == "x" || card2 == "x") {
                std::cout << "You have already matched one of these cards. Please select a different card." << std::endl;
                continue;
            }
            if (card1 == card2) {
                std::cout << "Congratulations! You have matched a pair." << std::endl;
                board[index1 / NUM_COLS][index1 % NUM_COLS] = 'x';
                board[index2 / NUM_COLS][index2 % NUM_COLS] = 'x';
                matched_pairs++;
            }
            else {
                std::cout << "Sorry, the selected cards do not match." << std::endl;
            }
            break;
        }

        selected1 = index1;
        selected2 = index2;
        num_turns++;

        Sleep(2000);
        system("cls");

        // Printing the board
        std::cout << "Turn " << num_turns + 1 << ":" << std::endl;
        std::cout << get_board(board, selected1, selected2) << std::endl;
        std::cout << "Please wait for the client to make a move." << std::endl;

        /**********************************************************************************************************/
        while (true) {
            // First question to client
            std::string gameReply;
            gameReply = "Enter the location of the first card: \nTurn " + std::to_string(num_turns + 1) + ":\n" + get_board(board, selected1, selected2);
            send(clientSocket, (gameReply.c_str()), strlen(gameReply.c_str()), 0);

            // Receive data from client
            char buffer[1024] = { 0 };
            int valread = recv(clientSocket, buffer, 1024, 0);

            row1 = buffer[0];
            col1 = buffer[1];

            index1 = get_index(row1, col1);
            if (index1 == -1) {
                gameReply = "Invalid location. Please enter a valid location.\n";
                send(clientSocket, (gameReply.c_str()), strlen(gameReply.c_str()), 0);
                continue;
            }

            // Second question to cleint
            gameReply = "Enter the location of the second card: \nTurn " + std::to_string(num_turns + 1) + ":\n" + get_board(board, selected1, selected2);
            send(clientSocket, (gameReply.c_str()), strlen(gameReply.c_str()), 0);

            // Receive data from client
            valread = recv(clientSocket, buffer, 1024, 0);

            row2 = buffer[0];
            col2 = buffer[1];

            index2 = get_index(row2, col2);
            if (index2 == -1) {
                gameReply = "Invalid location. Please enter a valid location.\n" + get_board(board, selected1, selected2);
                send(clientSocket, (gameReply.c_str()), strlen(gameReply.c_str()), 0);
                continue;
            }
            if (index1 == index2) {
                gameReply = "You cannot select the same card twice. Please select a different card.\n" + get_board(board, selected1, selected2);
                send(clientSocket, (gameReply.c_str()), strlen(gameReply.c_str()), 0);
                continue;
            }

            std::string card1 = board[index1 / NUM_COLS][index1 % NUM_COLS];
            std::string card2 = board[index2 / NUM_COLS][index2 % NUM_COLS];

            selected1 = index1;
            selected2 = index2;

            if (card1 == "x" || card2 == "x") {
                gameReply = "You have already matched one of these cards. Please select a different card.\n" + get_board(board, selected1, selected2);
                send(clientSocket, (gameReply.c_str()), strlen(gameReply.c_str()), 0);
                continue;
            }

            if (card1 == card2) {
                board[index1 / NUM_COLS][index1 % NUM_COLS] = 'x';
                board[index2 / NUM_COLS][index2 % NUM_COLS] = 'x';
                matched_pairs++;

                gameReply = "Congratulations! You have matched a pair.\n" + get_board(board, selected1, selected2);
                send(clientSocket, (gameReply.c_str()), strlen(gameReply.c_str()), 0);
            }
            else {
                gameReply = "Sorry, the selected cards do not match.\n" + get_board(board, selected1, selected2);
                send(clientSocket, (gameReply.c_str()), strlen(gameReply.c_str()), 0);

            }
            valread = recv(clientSocket, buffer, 1024, 0);
            break;
        }

        // Update client message & board
        system("cls");
        std::cout << "Client: " << buffer << std::endl;
        std::cout << get_board(board, selected1, selected2) << std::endl;
    }
    /**************************************************************************************************************/
    // Close sockets
    closesocket(clientSocket); //this one was set up once a client connected
    closesocket(serverSocket); //this one is out listening socket for new clients
    WSACleanup();
    return 0;
}