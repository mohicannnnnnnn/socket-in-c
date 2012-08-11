/*
 * =====================================================================================
 *
 *       Filename:  get_addr_info.c
 *
 *    Description:  use getaddrinfo(), freeaddrinfo(), gai_strerror()
 *					get the servers address inforament
 *
 *        Version:  1.0
 *        Created:  05/23/2011 02:02:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */



#define _XOPEN_SOURCE__

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netdb.h>


#include	"practical.h"



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
	if(3 != argc)
		die_with_user_msg("Parameter(s)", "<Address/Name> <Port/Service>") ;

	const char *addr_str = argv[1] ;
	const char *port_str = argv[2] ;

	/* Tell the system what kind of address wo want */
	struct addrinfo addr_criteria ;
	memset(&addr_criteria, 0, sizeof(addr_criteria)) ;

	addr_criteria.ai_family = AF_UNSPEC ;
	addr_criteria.ai_socktype = SOCK_STREAM ;
	addr_criteria.ai_protocol = IPPROTO_TCP ;

	/* Get address(es) associated with the specified name/service */
	struct addrinfo *addr_list_head ;
	int rtn_val = getaddrinfo(addr_str, port_str, NULL, &addr_list_head) ;
	if(0 != rtn_val)
		die_with_user_msg("getaddrinfo() failed", gai_strerror(rtn_val)) ;

	/* Display returned address */

	for (struct addrinfo *i=addr_list_head; i != NULL; i = i->ai_next) {
		print_socket_type(i->ai_socktype, stdout) ;
		print_socket_addr(i->ai_addr, stdout) ;
	}
	
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
