#include <iostream>
#include <WS2tcpip.h>
#include <string>

#include "../Game/match_game.h"

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib


#if(false)
int main() {
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

    while (true) {
        // Receive data from client
        char buffer[1024] = { 0 };
        int valread = recv(clientSocket, buffer, 1024, 0);
        std::cout << "Client: " << buffer << std::endl;

        // Checking if string is equal to end
        if (static_cast<std::string>(buffer) == "end")
            break;

        // Send response to client
        std::string strResponse;

        // Checking if string is empty
        if (!strResponse.empty())
            std::cin.ignore();

        std::getline(std::cin, strResponse);

        const char* response = strResponse.c_str();
        send(clientSocket, response, strlen(response), 0);

        // Checking if string is equal to end
        if (static_cast<std::string>(response) == "end")
            break;
    }
    /**************************************************************************************************************/
    // Close sockets
    closesocket(clientSocket); //this one was set up once a client connected
    closesocket(serverSocket); //this one is out listening socket for new clients
    WSACleanup();

    return 0;
}

#else

/******************************************************************************************************************************/// Testing
int main(void) {
    play_game();
    play_again();
    return 0;
}
#endif