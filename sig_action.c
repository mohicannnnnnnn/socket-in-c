/*
 * =====================================================================================
 *
 *       Filename:  sig_action.c
 *
 *    Description:  An signal test program
 *
 *        Version:  1.0
 *        Created:  05/27/2011 03:20:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<stdio.h>
#include	<signal.h>
#include	<unistd.h>
#include	<stdlib.h>


#include	"practical.h"

void interrupt_signal_handler(int signal_type) ;

int main()
{
	struct sigaction handler ;
	/* Set interrupt_signal_handler */
	handler.sa_handler = interrupt_signal_handler ;
	
	/* Create mask the blocks all signals */
	if (sigfillset(&handler.sa_mask) < 0) 
		die_with_sys_msg("sigfillset() failed\n") ;
	handler.sa_flags = 0 ; /* NO flags */
	printf("set sa_mask, sa_flags finnished\n") ;

	/* Set signal handling for interrupt signal */
	if (sigaction(SIGINT, &handler, 0) < 0)
		die_with_sys_msg("sigcation failed()\n") ;
	printf("regester sigcation finnished\n") ;

	for (;;) 
		pause() ; /*  Suspend program until signal received */

	exit(0) ;	
}


void interrupt_signal_handler(int signal_type) 
{
	puts("Interrupt received.pending 3s\n") ;
	sleep(3) ;
	puts("pending over, return back main") ;
}
