/*
 * =====================================================================================
 *
 *       Filename:  tcp_echo_server-fork.c
 *
 *    Description:  An generic echo back server listen a message and receve the message back
 *                  per-clinet one process
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
#include	<sys/wait.h>
#include	<sys/types.h>
#include	<stdio.h>
#include	<stdlib.h>

#include	"zhao_include/myUtility.h"	
#include	"practical.h"
#include	"tcp_server_utility.h"


const char welcome_page[] =  
	"##########################################################################\n\r"
	"#\t\t\t Welcome to TCP Echo Server \t\t\t#\n\r"
	"##########################################################################\n\r"  ;



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
	
	unsigned int child_proc_count = 0 ; /* Number of child processes */	
	for (;;) {
		/* accept from client connect and print client address information */
		int clnt_sock = accept_tcp_connection(serv_sock) ;

		/* Fork child process and report any errors */
		pid_t process_id = fork() ;
		if (process_id < 0)
			die_with_sys_msg("fork() failed") ;
		else if (process_id == 0) {
			/* Child process */
			debug_msg(stdout, "#Create child process:%u successed", getpid()) ;
			S_TEST(0, close(serv_sock))  /* closes parent socket */

			/* Send welcome page */
			debug_msg(stdout, "#%u inter send welcome", getpid()) ;
			send_welcome_page(clnt_sock, welcome_page) ;
			debug_msg(stdout, "#%u out of send welcome", getpid()) ;

			debug_msg(stdout, "#%u enter handle_tcp_client", getpid()) ;
			handle_tcp_client(clnt_sock) ;
			debug_msg(stdout, "#%u out of handle_tcp_client", getpid()) ;
			
			debug_msg(stdout, "#%u child process terminaled", getpid()) ;
			S_TEST(0, close(clnt_sock))  /* closes connect descriptor */
			exit(0) ;
		}

		S_TEST(0, close(clnt_sock))  /* Parent closes child socket descriptor */
		child_proc_count++ ;
	
		while (child_proc_count) {
			/* Clean up all zombies */
			process_id = waitpid((pid_t)-1, NULL, WNOHANG) ; /* non-blocking wait */
			if (process_id < 0)
				die_with_sys_msg("waitpid() failed") ;
			else if (process_id == 0) {
				/* NO zombie to wait on */
				debug_msg(stdout, "No zombie to wait on") ;
				break ;
			} else {
				debug_msg(stdout, "%u child process will be clean", process_id) ;
				child_proc_count-- ;
			}
		}
	} /* for(;;) */	

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
