#include<iostream>
#include<fstream>
#include<stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

class Client_socket
{
    fstream file;

    int PORT;

    int current_socket_descriptor;

    struct sockaddr_in address;
    int address_length;

public:
    Client_socket()
    {
        create_socket();
        PORT = 8050;

        address.sin_family = AF_INET;
        address.sin_port = htons( PORT );
        address_length = sizeof(address);
        if(inet_pton(AF_INET, "46.105.28.205", &address.sin_addr)<=0)
        {
            cout<<"[ERROR] : Invalid address\n";
        }

        create_connection();

        file.open("android-chrome-512x512.png", ios::out | ios::trunc | ios::binary);
        if(file.is_open())
        {
            cout<<"[LOG] : File opened!\n";
        }
        else
        {
            cout<<"[ERROR] : File creation failed, Exititng.\n";
            exit(EXIT_FAILURE);
        }
    }

    void create_socket()
    {
        if ((current_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("[ERROR] : Socket failed.\n");
            exit(EXIT_FAILURE);
        }
        cout<<"[LOG] : Socket Created Successfully.\n";
    }

    void create_connection()
    {
        if (connect(current_socket_descriptor, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("[ERROR] : connection attempt failed.\n");
            exit(EXIT_FAILURE);
        }
        cout<<"[LOG] : Connection Successfull.\n";
    }

    void receive_file()
    {
        char buffer[1024] = {};
        int valread = read(current_socket_descriptor, buffer, 1024);
        cout<<"[LOG] : Data received "<<valread<<" bytes\n";
        cout<<"[LOG] : Saving data to file.\n";

        file<<buffer;
        cout<<"[LOG] : File Saved.\n";
    }
    void transmit_file()
    {
        std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        cout<<"[LOG] : Transmission Data Size "<<contents.length()<<" Bytes.\n";

        cout<<"[LOG] : Sending...\n";
        cout<<contents.c_str()<<endl;
        int bytes_sent = send(current_socket_descriptor, contents.c_str(), contents.length(), 0 );
        cout<<"[LOG] : Transmitted Data Size "<<bytes_sent<<" Bytes.\n";

        cout<<"[LOG] : File Transfer Complete.\n";
    }
};

int main()
{
    Client_socket C;
    //C.receive_file();
    C.transmit_file();
    return 0;
}