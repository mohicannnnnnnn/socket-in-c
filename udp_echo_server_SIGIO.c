/*
 * =====================================================================================
 *
 *       Filename:  udp_echo_server.c
 *
 *    Description:  An udp echo program server use to SIGIO
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
#include	<unistd.h>
#include	<sys/file.h>
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/socket.h>
#include	<fcntl.h>
#include	<netdb.h>
#include	<signal.h>

#include	<zhao_include/myUtility.h>

#include	"practical.h"



void use_idle_time(void) ;
void sigio_handler(int signal_type) ;

static int sock ;
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

	/* Set signal handler for SIGIO */
	struct sigaction handler ;
	handler.sa_handler = sigio_handler ;
	/* Create mask that mask all signals */
	if (sigfillset(&handler.sa_mask) < 0)
		die_with_sys_msg("sigfillset() failed") ;

	handler.sa_flags = 0 ;

	if (sigaction(SIGIO, &handler, 0) < 0)
		die_with_sys_msg("sigcation() failed") ;

	/* We must own the socket recive the SIGIO message */
	if (fcntl(sock, F_SETOWN, getpid()) < 0)
		die_with_sys_msg("Uable to set process owner to us") ;

	/* arrange for nonblocking I/O and GISIO delivery */
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
		die_with_sys_msg("Unable to put client sock into non_blocking/async mode") ;


	for (;;) {
		use_idle_time() ;
	}
	
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

void use_idle_time()
{
	int i = 0 ;
	while (i++ < 6) {
		printf("idle time%d\n", i) ;
		sleep(1) ;
	}
}

void sigio_handler(int signal_type) 
{
	ssize_t num_bytes_recv ;
	sleep(3) ;
	
	do {
		struct sockaddr_storage clnt_addr ;
		socklen_t clnt_addr_len = sizeof(clnt_addr) ;

		/* Block until receive message from a client */
		char buf[MAX_STRING_LENGTH] ;
		num_bytes_recv = recvfrom(sock, buf, MAX_STRING_LENGTH, 0,  
				(struct sockaddr *)&clnt_addr, &clnt_addr_len) ;
		if (num_bytes_recv < 0) {
			/* Only acceptable error:recvfrom woulde have block */
			if (errno != EWOULDBLOCK)
			die_with_sys_msg("recvfrom() failed") ;
			debug_msg(stdout, "block") ;
		} else {
			fputs("Handling client", stdout) ;
			print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
			fputc('\n', stdout) ;

			/* Send received datagram back to the client */
			ssize_t num_bytes_sent = sendto(sock, buf, num_bytes_recv, 0, 
					(struct sockaddr *)&clnt_addr, clnt_addr_len) ;
			if (num_bytes_sent < 0)
				die_with_sys_msg("sendto() failed") ;
			else if (num_bytes_sent != num_bytes_recv)
				die_with_user_msg("sendto()", "sent unexpected number of bytes") ;
			debug_msg(stdout, "send successed size:%d", num_bytes_sent) ;
		}
	} while( num_bytes_recv >= 0) ;
}
