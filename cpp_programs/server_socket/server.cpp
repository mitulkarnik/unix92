#include "server.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

SocketServer::SocketServer()
{
	std::cout << "Creating socket server instance..." << std::endl;
	decoder.reset(new Decoder);
	file_manager.reset(new FileManager);
	
	socket_path = "/home/mkarnik/Documents/programs/cpp/sock_server/ser_socket";

}

SocketServer::~SocketServer()
{
	
}

int SocketServer::bootstrap_server()
{
	std::cout << "Bootstrapping socket server..." << std::endl;
	server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(server_fd <= 0) {
		std::cout << "Unable to get server socket "<< std::endl;
		return -1;
	}

	sock_addr = static_cast<sockaddr_un *>(std::malloc(sizeof(sockaddr_un)));
	std::memset(sock_addr, '\0', sizeof(sockaddr_un));

	sock_addr->sun_family = AF_UNIX;
	//sock_addr->sun_path = (char *) std::malloc(socket_path.size());
	std::memset(sock_addr->sun_path, '\0', socket_path.size());

	std::strncpy(sock_addr->sun_path, socket_path.c_str(), socket_path.size());
	
	unlink(socket_path.c_str());

	int status = bind(server_fd, reinterpret_cast<sockaddr *>(sock_addr), sizeof(sockaddr_un));

	if(!status) {
		std::cout << "Socket bind failed \n";
		return -1;
	}
	
	status = listen(server_fd, 100);
	if(!status) {
		std::cout << "Socket listen failed \n";
		return -1;
	}	
	std::cout << "Server socket listening on fd: " << server_fd << std::endl;
	
	return 0;
}

int SocketServer::start_server()
{
	std::cout << "Starting the server instance ..." << std::endl;

	int client_fd;
	sockaddr_un client_addr;
	auto addrlen = sizeof(client_addr);
	
	while(true) {
		client_fd = accept(server_fd, reinterpret_cast<sockaddr *>(&client_addr), reinterpret_cast<socklen_t *>(&addrlen));

		Operations client_operations;	
		decoder->decode_operation(client_fd, client_operations);
		switch(client_operations) {
			case READ_FILE: {
				char *buffer = new char[1024]();
				read(client_fd, buffer, 1024);
				std::string file_path(buffer);
			
				delete[] buffer;
				buffer = nullptr;

				FILE* fptr = file_manager->open_file_for_read(file_path);
				if(!fd_manager->transfer_fd_to_client(client_fd, fileno(fptr)))
				{
					std::cout << "Failed to transfer the fd to client process\n";
				}
			}		
				break;
			case WRITE_FILE:
				break;

			default:
				std::cout << "Default case: Invalid operation invalid \n";
		}
	}	

	return 0;
}

bool Decoder::decode_operation(int client_fd, Operations &operation_code)
{

	return false;
}

FILE* FileManager::open_file_for_read(const std::string &file_path)
{
	FILE* fptr = std::fopen(file_path.c_str(), "r");
	if(!fptr) {
		std::cout << "Cannot open the requested file: " << file_path << std::endl;
		return NULL;
	}

	return fptr;
}

FILE* FileManager::open_file_for_write(const std::string &file_path)
{
	return NULL;
}

bool FDManager::transfer_fd_to_client(int client_fd, int fd_to_send)
{
	return false;
}

int main(int argc, char*argv[])
{
	std::cout << "Starting the socket server " << std::endl;

	std::unique_ptr<SocketServer> socket_server(new SocketServer);

	socket_server->bootstrap_server();
	socket_server->start_server();


	std::cout << "Code should not reach here " << std::endl;

	return 0; 
}
