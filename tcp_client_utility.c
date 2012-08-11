/*
 * =====================================================================================
 *
 *       Filename:  tcp_client_utility.c
 *
 *    Description:  An tcp client utility set 
 *
 *        Version:  1.0
 *        Created:  05/24/2011 11:44:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<string.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netdb.h>

#include	<zhao_include/myUtility.h>

#include	"tcp_client_utility.h"
#include	"practical.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setup_tcp_client_socket
 *  Description:  An generic TCP client. setup takes a host and service. specified 
 *				  in a string, and returns a connected socket, or -1 on failure.
 *        Input:  host and service string.It may be specified as NULL
 * =====================================================================================
 */
int setup_tcp_client_socket(const char *host, const char *service) 
{
	/* set the socket of address info we want */	
	struct addrinfo addr_criteria ;
	memset(&addr_criteria, 0, sizeof(addr_criteria)) ;
	addr_criteria.ai_family = AF_UNSPEC ;			/* v4 or v6 */
	addr_criteria.ai_socktype = SOCK_STREAM ;		/* Only streaming socket */
	addr_criteria.ai_protocol = IPPROTO_TCP ;		/* Only tcp protocol */

	/* Get address */
	struct addrinfo *addrinfo_list ;
	int rtn_val = getaddrinfo(host, service, &addr_criteria, &addrinfo_list) ;
	if (rtn_val != 0) {
		die_with_user_msg("getaddrinfo() failed", gai_strerror(rtn_val)) ;
	}

	int sock = -1 ;
	for (struct addrinfo *i = addrinfo_list; i != NULL; i = i->ai_next) {
		debug_msg(stdout, "client address info") ;
		print_socket_addr(i->ai_addr, stderr) ;

		sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol) ;
		if (sock < 0) {
			debug_msg(stderr, "socket failed") ;
			continue ;
		}
		
		/* Establish the conection to the echo server */
		if (0 == connect(sock, i->ai_addr, i->ai_addrlen)) {
			debug_msg(stdout, "connect successed.") ;
			break ; /* connect successed */
		}
		
		debug_msg(stderr, "the socket can't connect to server") ;
		S_TEST(0, close(sock))
		sock = -1 ;
	}

	freeaddrinfo(addrinfo_list) ;

	return sock ;
}
