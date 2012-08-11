/*
 * =====================================================================================
 *
 *       Filename:  timeout.c
 *
 *    Description:  use alarm to test timeout
 *
 *        Version:  1.0
 *        Created:  05/30/2011 11:03:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<unistd.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<zhao_include/myUtility.h>

#include	"practical.h"
/* SIGALRM handler function */
void timeout(int sig)
{
	printf("timeout\n") ;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	/* Set signal handler for alarm signal */
	struct sigaction handler ;
	handler.sa_handler = timeout ;

	if (sigfillset(&handler.sa_mask) < 0)
		die_with_sys_msg("sigfillset() failed") ;
	handler.sa_flags = 0 ;

	/* Register signal handler */
	if (sigaction(SIGALRM, &handler, 0))
		die_with_sys_msg("sigaction() failed") ;
	debug_msg(stdout, "SIGALRM signal set readly") ;

	unsigned int sec = 5 ;
	unsigned int rec = 9 ;
	rec = alarm(sec) ;
	debug_msg(stdout, "beginning alarm look for 5 seconds... rec:%u", rec) ;
	for (int i = 1; i < 4; i++) {
		printf("sec:%d\trec:%u...\n", i, rec) ;
		sleep(1) ;
	}

	/* cancel alarm clock */
	rec = alarm(0) ; /* rec为上面设定alarm剩余的秒数 */
	printf("alarm(0) rec:%u\n", rec) ;
	sleep(3) ;

	
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
