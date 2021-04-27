/*--------------------------------------------------------------------------------
# Client code for Socket Communication
#
# Name: client.c
#
# Written By: Saalar Faisal - 2021-04-04
#
# Purpose: This is client-server communication processes.
#		   We (client) are sending a file to the server to write to another file
#		   
# To compile: gcc -o client client.c
#
# Usage >> ./client <fileName>
#
#---------------------------------------------------------------------------------*/

#include <stdio.h>  // Input/output library
#include <stdlib.h> // C standard library
#include <unistd.h> // required for file open, close
#include <string.h> // String library
#include <arpa/inet.h> // IP address library

#define BufferSize 1024 // our fixed block size

// --deliverFile method--
// takes filePointer and socket descriptor as parameters
// this will send the file string based on our fixed block size of 1024 bytes
// it will also use the shortcut bzero to erase the data in the memory from the previous
// it will send the file in packets using 'send'

void deliverFile(FILE *fp, int socketd){
  char fileStr[BufferSize] = {0}; // pointer of array of chars where the string read is stored

// read a line from the stream of fp and store into fileStr
// read as per 1024 bytes
  while(fgets(fileStr, BufferSize, fp) != NULL) {
    if (send(socketd, fileStr, sizeof(fileStr), 0) == -1) {
      perror("Could not send file.");
      exit(1); // exit with error
    }
	// cleans the buffer, so we do not carry from previous
	// erases the data in the n bytes of the memory starting at the location we pointed
    bzero(fileStr, BufferSize);
  }
}

// our main program accepts command line parameter
// it will specify the file that is to be sent to be written
int main(int argc, char *argv[])
{
  // this is our system local host IP
  char *ip = "127.0.0.1";
  
  int port = 50714; 
  // socket descriptor
  int socketd;
  // structure for sockaddr
  struct sockaddr_in srv;
  // our file pointer
  FILE *fp;
  
  // Checking commandline
  // we will exit if file not provided  
  if(argc != 2) {
	  
	  printf("---> Incorrect Parameters: Enter as >> ./client <fileName> \n");
	  exit(1); // exit with error
  }
  
// 1) create the socket 
// it is IPV4 as specified with AF_INET
// it is a TCP connection as its a SOCK_STREAM
  if((socketd = socket(AF_INET, SOCK_STREAM, 0))< 0) {
    perror("Socket Call Failed");
    exit(1); // exit with error
  }
  // if it succeeds, inform the user
  printf("---> Server socket call was successful.\n");

  // 2) connect() to the server 
  srv.sin_family = AF_INET; // connect: use the Internet address family
  srv.sin_port = port;  // connect socket 'socketd' to our unique port
  srv.sin_addr.s_addr = inet_addr(ip); // connect to IP Address

// connect to server
  if(connect(socketd, (struct sockaddr*)&srv, sizeof(srv)) == -1) {
    perror("Socket Error");
    exit(1); // exit with error
  }
  // let user know if successful
 printf("---> We have successfully connected to the server.\n");

// open and read the file
// entered from command line
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror("Could not read file.");
    exit(1); // exit with error
  }
  
  // use our method and send the file to server
  deliverFile(fp, socketd);
  // let user know
  printf("---> Successfully sent file %s \n", argv[1] );

  // close the socket
  printf("---> We have now closed the connection.\n");
  close(socketd);

  return 0; // successfully exit
}