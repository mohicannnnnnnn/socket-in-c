/*
 * =====================================================================================
 *
 *       Filename:  tcp_echo_server4.c
 *
 *    Description:  An echo back server listen a message and receve the message back
 *
 *        Version:  1.0
 *        Created:  05/21/2011 04:18:04 PM
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

#include	"zhao_include/myUtility.h"	
#include	"practical.h"
#include	"tcp_server_utility.h"

static const int MAXPENDING = 5 ; /* Maximum outstanding conection request */


#include	<stdlib.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	if (2 != argc) 
		die_with_user_msg("Parameter(s)", "<Server Pot>") ;

	in_port_t serv_port = atoi(argv[1]) ;

	/* Create socket for incoming connections */
	int serv_sock ;
	EV_TEST(-1, serv_sock, socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
	
	/* Construct local address structre */
	struct sockaddr_in serv_addr ;
	memset(&serv_addr, 0, sizeof(serv_addr)) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ; /* any incoming interface */
	serv_addr.sin_port = htons(serv_port) ;
	
	/* print server socket address information */
	printf("server addr informeation\n") ;
	print_socket_addr((struct sockaddr *)&serv_addr, stdout) ;

	/* Bind to the local address */
	E_TEST(-1, bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	
	/* Make the sockte so it will listen for incoming connections */
	E_TEST(-1, listen(serv_sock, MAXPENDING))

	for (;;) {
		struct sockaddr_in clnt_addr ;
		socklen_t clnt_addr_len = sizeof(clnt_addr) ;

		/* Wait for a clinet to connect */
		int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) ;
		if(clnt_sock < 0)
			die_with_sys_msg("accept() failed") ;

		/* show clinet socket information */
		printf("client addr informeation\n") ;
		print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;

		/* Send welcome page */
		printf("inter send welcome\n") ;
		send_welcome_page(clnt_sock) ;
		printf("outoff send welcome\n") ;

		//printf("inter handle\n") ;
		handle_tcp_client(clnt_sock) ;
		//printf("out handle\n") ;
	}
		

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
