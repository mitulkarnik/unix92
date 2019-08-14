#pragma once 

#include <string>
#include <cstdio>
#include <fstream>
#include <memory>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

class Decoder;
class FileManager;
class FDManager;

enum Operations
{
	READ_FILE,
	WRITE_FILE
};

/**
 * @brief
 *
 * This Socket server will mainly create the unix domain socket and 
 * will wait for client to connect to it for operations
 *
 **/
class SocketServer 
{
public:
	SocketServer();
	~SocketServer();

	int bootstrap_server();
	int start_server();	
private:
	int server_fd;
	std::unique_ptr<Decoder> decoder;
	std::unique_ptr<FileManager> file_manager;
	std::unique_ptr<FDManager> fd_manager;
	
	sockaddr_un *sock_addr;
	std::string socket_path;	
};

class Decoder
{
public:
	bool decode_operation(int client_fd, Operations &operation_code);	
private:
};

class FileManager
{
public:
	FILE* open_file_for_read(const std::string &file_path);
	FILE* open_file_for_write(const std::string &file_path);
};

/**
 * @brief 
 * This class will invoke low level syscalls like 
 * sendmsg which will transfer the file descriptor from server process to client 
 * process 
 **/
class FDManager
{
public:
	bool transfer_fd_to_client(int client_fd, int fd_to_send);
};

