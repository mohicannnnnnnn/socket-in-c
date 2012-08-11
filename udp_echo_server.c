/*
 * =====================================================================================
 *
 *       Filename:  udp_echo_server.c
 *
 *    Description:  An udp echo program server
 *
 *        Version:  1.0
 *        Created:  05/28/2011 07:26:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<string.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netdb.h>

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
	if (2 != argc) 
		die_with_user_msg("Parameter(s)", "<Server Pot/Service>") ;

	const char *serv_port = argv[1] ;

	/* Tell the system what kind of address info we want */
	/* can be write a func
	 * "setup_udp_server_socket(const char *serv_ip, const char *service */
	struct addrinfo  addr_criteria ;
	memset(&addr_criteria, 0, sizeof(addr_criteria)) ;
	addr_criteria.ai_family = AF_UNSPEC ;			/* v4 or v6 */
	addr_criteria.ai_socktype = SOCK_DGRAM;		/* Only datagram socket */
	addr_criteria.ai_protocol = IPPROTO_UDP;		/* Only udp protocol */
	addr_criteria.ai_flags = AI_PASSIVE ;			/* Accept on any adrress/port */

	/* Get address */
	struct addrinfo *addrinfo_list ;
	int rtn_val = getaddrinfo(NULL, serv_port, &addr_criteria, &addrinfo_list) ;
	if (rtn_val != 0) {
		die_with_user_msg("getaddrinfo() failed", gai_strerror(rtn_val)) ;
	}

	int sock = -1 ;
	struct addrinfo *sock_addr ;
	for (sock_addr = addrinfo_list; sock_addr!= NULL; sock_addr = sock_addr->ai_next) {
		debug_msg(stdout, "client address info") ;
		print_socket_addr(sock_addr->ai_addr, stderr) ;

		sock = socket(sock_addr->ai_family, sock_addr->ai_socktype, sock_addr->ai_protocol) ;
		if (sock < 0) {
			debug_msg(stderr, "socket failed") ;
			continue ;
		}

		/* Bind to the local address */
		debug_msg(stdout, "bindding local address...") ;
		if (bind(sock, sock_addr->ai_addr, sock_addr->ai_addrlen) < 0)
			perror("bind() failed") ;
		else 
		{
			debug_msg(stdout, "bind successed") ;
			break ;
		}
	}
	freeaddrinfo(addrinfo_list) ;

	for (;;) {
		struct sockaddr_storage clnt_addr ;
		socklen_t clnt_addr_len = sizeof(clnt_addr) ;

		/* Block until receive message from a client */
		char buf[MAX_STRING_LENGTH] ;
		ssize_t num_bytes = recvfrom(sock, buf, MAX_STRING_LENGTH, 0,  
				(struct sockaddr *)&clnt_addr, &clnt_addr_len) ;
		if (num_bytes < 0)
			die_with_sys_msg("recvfrom() failed") ;
		
		fputs("Handling client", stdout) ;
		print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
		fputc('\n', stdout) ;

		/* Send received datagram back to the client */
		ssize_t num_bytes_sent = sendto(sock, buf, num_bytes, 0, 
				(struct sockaddr *)&clnt_addr, clnt_addr_len) ;
		if (num_bytes_sent < 0)
			die_with_sys_msg("sendto() failed") ;
		else if (num_bytes_sent != num_bytes)
			die_with_user_msg("sendto()", "sent unexpected number of bytes") ;
	}
	
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
