/*
 * =====================================================================================
 *
 *       Filename:  tcp_echo_client.c
 *
 *    Description:  An generic TCP echo client 
 *
 *        Version:  1.0
 *        Created:  05/25/2011 09:58:37 AM
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

#include	"tcp_client_utility.h"
#include	"tcp_server_utility.h"
#include	"practical.h"

int main(int argc, char *argv[])
{
	if(argc < 3 || argc > 4)
		die_with_user_msg("Parameter(s)", 
				"<Server Address/Name> <Echo Word> [<Server Post/Service>]") ;

	/* First arg is server IP and second arg is echo string */
	const char *serv_ip = argv[1] ;
	const char *echo_string = argv[2] ;

	/* If have the third arg use it.else user 7 port  */
	const char *serv_port = (argc == 4) ? argv[3] : "echo" ;

	/* get a connect to server of socket */
	int sock = setup_tcp_client_socket(serv_ip, serv_port) ;
	if(-1 == sock)
		die_with_user_msg("setup_tcp_client_socket() failed", "unable to connect") ;


	/* Recive sever welcome page */
	recv_welcome_page(sock) ;

	size_t echo_string_len = strlen(echo_string) ;

	/* mutlip send-recv loop */
	for (int i=0; i < 5; i++) {
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
		sleep(2) ;
		printf("\t%d\tsleep 2 sec.................\n", i) ;
	} /* for end */

	fputc('\n', stdout) ;

	S_TEST(0, close(sock)) 
	return 0 ;
}
