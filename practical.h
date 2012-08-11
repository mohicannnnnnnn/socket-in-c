/*
 * =====================================================================================
 *
 *       Filename:  practical.h
 *
 *    Description:  used to socket in c 
 *
 *        Version:  1.0
 *        Created:  05/21/2011 12:47:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  PRACTICAL_H_INC
#define  PRACTICAL_H_INC


#define SOCKTYPE_MAX_LEN  25       /* storage the socket type string buffer size */
#define FAMILY_MAX_LEN 25          /* storage net family type string buffer size */

#include	<stdbool.h>
#include	<stdio.h>
#include	<sys/socket.h>

//	Handle error with user msg
void die_with_user_msg(const char *msg, const char *detail) ;

// Handle error with sys msg
void die_with_sys_msg(const char *msg) ;

// Print socket adress Generic IPv4 and IPv6 
void print_socket_addr(const struct sockaddr *address, FILE *stream) ;

// translate from int to string name of socket  type, return the buffer
char *socktype_to_str(int socktype, char *buffer) ;

// print socket type of specify socket
void print_socket_type(int socktype, FILE *stream) ;

// translate form int to string name of net family, return the translate buffer
char *family_to_str(int family, char *buffer) ;

// print family name string 
void print_family_type(int family, FILE *stream) ;

// translate from int to protocol of string name
char *protocol_to_str(int protocol, char *buffer) ;

// print protocol type of specify socket
void print_protocol_type(int protocol, FILE *stream) ;

// print the socket family, address, port for IPv6
void print_socket_addr6(const struct sockaddr *address, FILE *stream) ;

// Test two socket address equality Only family is INET or INET6
bool sock_addrs_equal(const struct sockaddr *addr1, const struct sockaddr *addr2) ;

//Create, bind, and listen a new TCP server socket
int setup_tcp_server_socket(const char *service) ;

// Accept a new TCP connection on a server socket
int accept_tcp_connection(int servSock) ;

// Handle new TCP client
void handle_tcp_client(int clntSock) ;

// Create and connect a new TCP client socket
int setup_tcp_client_socket(const char *server, const char *service) ;

enum size_constants {
	MAX_STRING_LENGTH = 128, 
	BUFSIZE = 512, 
} ;
#endif   /* ----- #ifndef PRACTICAL_H__INC  ----- */

