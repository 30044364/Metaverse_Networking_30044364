#include <iostream>
#include <string>
#include <WS2tcpip.h>

//define a pause macro which we use later - not essential for running of program, just for demo.
#include <stdlib.h>
#ifdef _WIN32
#define WINPAUSE system("pause")
#endif

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main() {
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

    while (true) {
        // Send data to server
        std::string strMessage;

        if (!strMessage.empty())
            std::cin.ignore();

        std::getline(std::cin, strMessage);

        const char* message = strMessage.c_str();
        send(clientSocket, message, strlen(message), 0);

        // Checking if string is equal to end
        if (static_cast<std::string>(message) == "end")
            break;


        // Receive response from server
        char buffer[1024] = { 0 };
        int valread = recv(clientSocket, buffer, 1024, 0);
        std::cout << "Server: " << buffer << std::endl;

        // Checking if string is equal to end
        if (static_cast<std::string>(buffer) == "end")
            break;
    }
    /**************************************************************************************************************/
    // Close socket
    closesocket(clientSocket);

    //pause program for us to check output
    WINPAUSE;


    WSACleanup();

    return 0;
}
