/*---------------------------------------------------------------------------------------
# Server code for Socket Communication
#
# Name: server.c
#
# Written By: Saalar Faisal - 2021-04-04
#
# Purpose:  This is client-server communication processes.
#			we(server) are receiveing a file from the client and then writing it to 
#			another text file.
#		   
# To compile: gcc -o server server.c
#
# Usage 1 >> ./server
# 
# Usage 2 >> ./server & (to run in background)
#
#----------------------------------------------------------------------------------------*/

#include <stdio.h>  // Input/output library
#include <stdlib.h> // C standard library
#include <unistd.h> // required for file open, close
#include <string.h> // String library
#include <arpa/inet.h> // IP address library

#define BufferSize 1024 // our fixed block BufferSize

// This method/subroutine will receive information of file from the client
// it will then write out those contents to a new file as specified below
void writeToFile(int socketd){
  // save the name of the cloned file that we will write to
  char *filename = "sourcefile_local_clone.txt";
  // flag for loop counter completion
  int done = 0;
  // our file pointer
  FILE *fp;
  // pointer of array of chars where the string read is stored
  char fileStr[BufferSize];

  // open the file and write to it
  fp = fopen(filename, "w");
  // as long as we don't reset our flag
  while (!done) {
	 // return if we have reached the end
    if (recv(socketd, fileStr, BufferSize, 0) <= 0){
	  done = 1; // end while loop
      return; // end method
    }
	// send formated output to stream in string from fileStr
    fprintf(fp, "%s", fileStr);
	// erases the data in the n bytes of the memory starting at the location we pointed
	// cleans the fileStr, so we do not carry from previous
    bzero(fileStr, BufferSize);
  }
  return; // end method
}

int main(){
  // this is our system local host IP
  char *ip = "127.0.0.1"; 
  int port = 50714;
  // socket descriptors
  int socketd, newfd;
  
  //struct type of sockaddr_in
  struct sockaddr_in srv, cli; //used by bind(), //used by accept() 
  socklen_t cli_len = sizeof(cli);  //used by accept() 
  char fileStr[BufferSize]; // pointer of array of chars where the string read is stored

  // 1) create the socket
  socketd = socket(AF_INET, SOCK_STREAM, 0);
  if(socketd < 0) {
    perror("socket call failed");
    exit(1); // exit with error
  }
  // inform user
  printf("---> Successfully created server socket.\n");

  // 2) bind the socket to a port
  srv.sin_family = AF_INET; // use the Internet addr family
  srv.sin_port = port; // use our unique port
  srv.sin_addr.s_addr = inet_addr(ip); // convert the string ip into an integer value suitable for use as an Internet address

  // bind cont...
  if(bind(socketd, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
    perror("bind call failed");
    exit(1); // exit with error
  }
  // inform user 
  printf("---> Successfully binded.\n");

  // 3) listen on the socket
  if(listen(socketd, 5) == 0){
 printf("---> Listening....\n"); //wait for connection
 }else{
 perror("listen call failed");
    exit(1); // exit with error
 }
 
  // 4) accept the incoming connection
  newfd = accept(socketd, (struct sockaddr*)&cli, &cli_len);
  // run our method to write the file
  writeToFile(newfd);
  // inform user we have written file
  printf("---> File successfully written.\n");

  return 0; // successfully exit
}