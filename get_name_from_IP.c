/*
 * =====================================================================================
 *
 *       Filename:  get_name_from_IP.c
 *
 *    Description:  An tools get you want translate name(web name) that from Ip address
 *
 *        Version:  1.0
 *        Created:  05/25/2011 06:43:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<unistd.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>
#include	<netdb.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>


#include	<zhao_include/myUtility.h>
#include	"practical.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	if (argc < 3 || argc > 4)
		die_with_user_msg("Usage:Parameters", "<i4|i6> <IP Address> [<Service Port>]") ;	

	const char *addr_family = argv[1] ;
	const char *addr = argv[2] ;
	const in_port_t port = (argc == 4) ? atoi(argv[3]) : 0 ;
	socklen_t address_len = 0 ;
	struct sockaddr_storage address ; /* an generic address */

	memset(&address, 0, sizeof(address)) ;

	/* Create the sepcified address */
	if ((0 == strcmp(addr_family, "i4"))
			|| (0 == strcmp(addr_family, "I4"))) {
		/* Ipv4 address */
		address_len = sizeof(struct sockaddr_in) ;

		struct sockaddr_in *sock_addr = (struct sockaddr_in *)&address ;
		sock_addr->sin_family = AF_INET ;
		int rtn_val = inet_pton(AF_INET, addr, &sock_addr->sin_addr.s_addr) ;
		if (rtn_val == 0) {
			die_with_user_msg("inet_pton() failed", "invalid address string") ;
		} else if (rtn_val < 0) {
			die_with_sys_msg("inet_pton() failed") ;
		}	
		sock_addr->sin_port = htons(port)  ;
	} else if ((0 == strcmp(addr_family, "i6"))
			|| (0 == strcmp(addr_family, "I6"))) {
		/* Ipv6 address */
		address_len = sizeof(struct sockaddr_in6) ;

		struct sockaddr_in6 *sock_addr = (struct sockaddr_in6 *)&address ;
		sock_addr->sin6_family = AF_INET6 ;
		int rtn_val = inet_pton(AF_INET6, addr, &sock_addr->sin6_addr.s6_addr) ;
		if (rtn_val == 0) {
			die_with_user_msg("inet_pton() failed", "invalid address string") ;
		} else if (rtn_val < 0) {
			die_with_sys_msg("inet_pton() failed") ;
		}	
		sock_addr->sin6_port = htons(port) ;
	} else {
		die_with_user_msg("invalid paramaer"
				,"The firest pramaer muest is i4 or i6") ;
	}
	debug_msg(stdout, "set sepecified address successed") ;

	char node[NI_MAXHOST] ;
	char service[NI_MAXSERV] ;

	int rtn_val = getnameinfo((struct sockaddr *)&address, address_len, 
			node, sizeof(node), service, sizeof(service), 
			NI_NAMEREQD ) ; /* if cannot be ound for the gice address, return error */
	if (rtn_val == 0) {
		/* getnameinfo successed */
		debug_msg(stdout, "getnameinfo calling successed") ;
		printf("IP name:%s\tService name:%s\n", node, service) ;
	} else if(EAI_AGAIN == rtn_val) {
		/* the name could not be resolved at this time */
		debug_msg(stdout, "getnameinfo() failed: Retuen EAI_AGAIN%s\n", gai_strerror(rtn_val)) ;
		fprintf(stdout, "The name coulde not be resolved at this time,  try again\n") ;
	} else {
		err_msg("getnameinfo failed: return value is:%d %s\n"
				, rtn_val, gai_strerror(rtn_val)) ;
	}

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
