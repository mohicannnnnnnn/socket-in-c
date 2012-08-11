/*
 * =====================================================================================
 *
 *       Filename:  tcp_echo_server4.c
 *
 *    Description:  An generic echo back server listen a message and receve the message back
 *
 *        Version:  1.0
 *        Created:  05/25/2011 01:54:52 PM
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

#include	"zhao_include/myUtility.h"	
#include	"practical.h"
#include	"tcp_server_utility.h"




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
		die_with_user_msg("Parameter(s)", "<Server Pot/Service>") ;

	const char *serv_port = argv[1] ;

	/* Setpu socket for incoming connections, creak server socket, and
	 * bind, listen */
	int serv_sock = setup_tcp_server_socket(serv_port) ;
	if(-1 == serv_sock)
		err_msg("setup_tcp_socket() failed") ;
	
	
	for (;;) {
		/* accept from client connect and print client address information */
		int clnt_sock = accept_tcp_connection(serv_sock) ;

		/* Send welcome page */
		debug_msg(stdout, "inter send welcome") ;
		send_welcome_page(clnt_sock) ;
		debug_msg(stdout, "out of send welcome") ;

		debug_msg(stdout, "enter handle_tcp_client") ;
		handle_tcp_client(clnt_sock) ;
		debug_msg(stdout, "out of handle_tcp_client") ;

		printf("client connect closed\n") ;
		S_TEST(0, close(clnt_sock)) 
	}

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
