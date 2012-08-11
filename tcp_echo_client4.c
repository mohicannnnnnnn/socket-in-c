/*
 * =====================================================================================
 *
 *       Filename:  tcp_echo_client4.c
 *
 *    Description:  An implementation of a TCP echo client for IPv4
 *
 *        Version:  1.0
 *        Created:  05/21/2011 12:41:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<unistd.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>


#include	<zhao_include/myUtility.h>

#include	"practical.h"

int main(int argc, char *argv[])
{
	if(argc < 3 || argc > 4)
		die_with_user_msg("Parameter(s)", 
				"<Server Address> <Echo Word> [<Server Post>]") ;

	/* First arg is server IP and second arg is echo string */
	const char *serv_ip = argv[1] ;
	const char *echo_string = argv[2] ;

	/* If have the third arg use it.else user 7 port  */
	in_port_t serv_port = (argc == 4) ? atoi(argv[3]) : 7 ;

	/* Create a reliable, stream socket using TCP */
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) ;
	if(sock < 0)
		die_with_sys_msg("socket() failed") ;

	/* bind client socket to a spiscfy addresss */
	struct sockaddr_in clnt_addr ;
	memset(&clnt_addr, 0, sizeof(clnt_addr)) ;
	clnt_addr.sin_family = AF_INET ;
	clnt_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
	clnt_addr.sin_port = htons(0) ;
	E_TEST(-1, bind(sock, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr)))

	/* Show client socket information */
	socklen_t clnt_sock_len = sizeof(clnt_addr) ;
	E_TEST(-1, getsockname(sock, (struct sockaddr *)&clnt_addr, &clnt_sock_len))
	printf("client address information:\n") ;
	print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;



	/* Construct the server address struct */
	struct sockaddr_in serv_addr ;
	memset(&serv_addr, 0, sizeof(serv_addr)) ;
	serv_addr.sin_family = AF_INET ;

	/* convert address */
	int rtn_val = inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr.s_addr) ;
	if(0 == rtn_val)
		die_with_user_msg("inet_pton() failed", "invalid address string") ;
	else if (rtn_val < 0)
		die_with_sys_msg("inet_pton() failed") ;
	serv_addr.sin_port = htons(serv_port) ;

	/* Establish the connection to the echo server */
	E_TEST(-1, connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	size_t echo_string_len = strlen(echo_string) ;

	/* Recive sever welcome page */
	printf("inter welcome\n") ;
	recv_welcome_page(sock) ;
	printf("outoff welcome\n") ;

	/* Send the string to the server */
	ssize_t num_bytes = send(sock, echo_string, echo_string_len, 0) ;
	if(num_bytes < 0)
		die_with_sys_msg("sned() failed") ;
	else if (num_bytes != echo_string_len)
		die_with_user_msg("send()", "sent unexpected unmber of bytes") ;

	/* Recive the same string back from the server */
	unsigned int total_recv_bytes= 0 ;
	fputs("Received: ", stdout) ;
	while(total_recv_bytes < echo_string_len) {
		char buffer[BUFSIZE] ;
		/* Recive up to the buffer size, 
		 * minus 1 to leave space for a null terminator, 
		 * bytes from sender */
		num_bytes = recv(sock, buffer, BUFSIZE - 1, 0) ;
		if (num_bytes < 0)
			die_with_sys_msg("recv() failed") ;
		else if (num_bytes == 0)
			die_with_user_msg("recv()", "connection closed prematurely") ;
		total_recv_bytes += num_bytes ;
		buffer[num_bytes] = '\0' ;
		fputs(buffer, stdout) ;
	}

	debug_msg(stdout, "recitive finished") ;
	fputc('\n', stdout) ;

	S_TEST(0, close(sock)) 
	return 0 ;
}
