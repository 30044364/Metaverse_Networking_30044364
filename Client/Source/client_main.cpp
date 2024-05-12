#include <iostream>
#include <string>
#include <conio.h>
#include <WS2tcpip.h>

//define a pause macro which we use later - not essential for running of program, just for demo.
#include <stdlib.h>
#ifdef _WIN32
#define WINPAUSE system("pause")
#endif

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main(void) {
    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsok = WSAStartup(ver, &wsData);
    if (wsok != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Connect to server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Server port number
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    /**************************************************************************************************************/

    std::cout << "*** You are the Client ***" << std::endl << std::endl;

    // Receive response from server
    char buffer[1024] = { 0 };
    int valread = recv(clientSocket, buffer, 1024, 0);
    std::cout << buffer << std::endl;

    // Message asking client to continue
	std::cout << "Continue? [Y/N]" << std::endl;
	
    // Single press enter, Y to continue
    char key;
	do {
		key = _getch();
	} while (key != 'y');
	system("cls");
    
    // Automated response to server
    const char* sendToContinue = "Your turn";
    send(clientSocket, sendToContinue, strlen(sendToContinue), 0);


    std::cout << "Waiting for the server to make a move" << std::endl;

    // Message to server loop
    while (true) {
        // Receive response from server
        char buffer[1024] = { 0 };
        int valread = recv(clientSocket, buffer, 1024, 0);
        system("cls");
        std::cout << "Server: " << buffer << std::endl;




        // Sending message to sevrer
        std::string message;
        if (!message.empty())
            std::cin.ignore();
        
        // looking for the first word in the buffer
        std::string firstWordBuffer = static_cast<std::string>(buffer).substr(0, static_cast<std::string>(buffer).find(" "));
        
        if (firstWordBuffer == "Congratulations!") {
            message = "Client matched a pair of cards\n";
            send(clientSocket, message.c_str(), strlen(message.c_str()), 0);
            std::cout << "Please wait for the server to make a move." << std::endl;
            continue;
        }
        if (firstWordBuffer == "Sorry,") {
            message = "Your turn\n";
            send(clientSocket, message.c_str(), strlen(message.c_str()), 0);
            std::cout << "Please wait for the server to make a move." << std::endl;
            continue;
        }

        std::cout << "You: ";
        std::getline(std::cin, message);
        send(clientSocket, message.c_str(), strlen(message.c_str()), 0);
    }
    /**************************************************************************************************************/
    // Close socket
    closesocket(clientSocket);

    //pause program for us to check output
    WINPAUSE;


    WSACleanup();

    return 0;
}
