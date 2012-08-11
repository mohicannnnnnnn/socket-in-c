/*
 * =====================================================================================
 *
 *       Filename:  udp_echo_client.c
 *
 *    Description:  An base-on UDP protocol echo program client
 *
 *        Version:  1.0
 *        Created:  05/28/2011 05:37:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<stdio.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<string.h>
#include	<netdb.h>
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
	if(argc < 3 || argc > 4)
		die_with_user_msg("Parameter(s)", 
				"<Server Address/Name> <Echo Word> [<Server Post/Service>]") ;

	/* First arg is server IP and second arg is echo string */
	const char *serv_ip = argv[1] ;
	const char *echo_string = argv[2] ;

	size_t echo_string_len = strlen(echo_string) ;
	if (echo_string_len > MAX_STRING_LENGTH)
		die_with_user_msg(echo_string, "string too long") ;

	/* If have the third arg use it.else user 7 port  */
	const char *serv_port = (argc == 4) ? argv[3] : "echo" ;

	/* Tell the system what kind of address info we want */
	/* can be write a func
	 * "setup_udp_client_socket(const char *serv_ip, const char *service */
	struct addrinfo  addr_criteria ;
	memset(&addr_criteria, 0, sizeof(addr_criteria)) ;
	addr_criteria.ai_family = AF_UNSPEC ;			/* v4 or v6 */
	addr_criteria.ai_socktype = SOCK_DGRAM;		/* Only datagram socket */
	addr_criteria.ai_protocol = IPPROTO_UDP;		/* Only udp protocol */

	/* Get address */
	struct addrinfo *addrinfo_list ;
	int rtn_val = getaddrinfo(serv_ip, serv_port, &addr_criteria, &addrinfo_list) ;
	if (rtn_val != 0) {
		die_with_user_msg("getaddrinfo() failed", gai_strerror(rtn_val)) ;
	}

	int sock = -1 ;
	struct addrinfo *sock_addr ;
	for (sock_addr = addrinfo_list; sock_addr!= NULL; sock_addr = sock_addr->ai_next) {
		debug_msg(stdout, "server address info") ;
		print_socket_addr(sock_addr->ai_addr, stdout) ;

		sock = socket(sock_addr->ai_family, sock_addr->ai_socktype, sock_addr->ai_protocol) ;
		if (sock < 0) {
			perror("socket() failed") ;
			continue ;
		} else { 
			debug_msg(stdout, "create socket successed") ;
			break ;
		}
	}
	if (sock_addr == NULL)
		die_with_user_msg("getaddrinfo()", " addr_list no vaild address can be userd") ;


	/* print current client sock info */
	struct sockaddr_storage clnt_addr ;
	socklen_t clnt_addr_len = sizeof(clnt_addr) ;
#if 0
	if (getsockname(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
		perror("getsockname() failed") ;
	else {
		printf("before connect host addr info:\n") ;
		print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
	}
	if (getpeername(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
		perror("getpeerkname() failed") ;
	else {
		printf("before connect peer addr info:\n") ;
		print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
	}

	/* Connect UDP socket */
	if (connect(sock, sock_addr->ai_addr, sock_addr->ai_addrlen) < 0) 
		perror("connect() failed") ;
	else {
		debug_msg(stdout, "connect successed") ;

		if (getsockname(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
			perror("getsockname() failed") ;
		else {
			printf("after connect host addr info:\n") ;
			print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
		}
		if (getpeername(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
			perror("getpeerkname() failed") ;
		else {
			printf("after connect peer addr info:\n") ;
			print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
		}

		debug_msg(stdout, "sendto client ...") ;
		ssize_t num_bytes = send(sock, echo_string, echo_string_len, 0) ;
		if (num_bytes < 0)
			die_with_sys_msg("send() failed") ;
		else if (num_bytes != echo_string_len)
			die_with_user_msg("send() error", "send unexpected number of bytes") ;

		debug_msg(stdout, "send client successed") ;

		/* Receive a response */
		char buf[MAX_STRING_LENGTH + 1] ; /* the last char storage null */

		debug_msg(stdout, "recv server...") ;
		num_bytes = recv(sock, buf, MAX_STRING_LENGTH, 0) ;
		if (num_bytes < 0)
			die_with_sys_msg("recv() failed") ;
		else if (num_bytes != echo_string_len)
			die_with_user_msg("recv() error", "recived unexpected number of bytes") ;
		debug_msg(stdout, "recive successed, recived data size%d", num_bytes) ;
		/* print recived string */
		buf[echo_string_len] = '\0' ;
		printf("Connet() Received: %s\n", buf) ;
	}
	
	/* Disconnect  */
	struct sockaddr unspec_addr ;
	memset(&unspec_addr, 0, sizeof(unspec_addr)) ;
	unspec_addr.sa_family = AF_UNSPEC ;
	if (connect(sock, &unspec_addr, sizeof(unspec_addr)) < 0)
		perror("connect() failed") ;


	printf("after disconnect\n") ;
	if (getsockname(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
		perror("getsockname() failed") ;
	else {
		printf("after disconnect host addr info:\n") ;
		print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
	}
	if (getpeername(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
		perror("getpeerkname() failed") ;
	else {
		printf("after disconnect peer addr info:\n") ;
		print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
	}
	/* setup_udp_clinet finnished */
#endif 
	/* Send the string to server */
	debug_msg(stdout, "sendto client ...") ;
	ssize_t num_bytes = sendto(sock, echo_string, echo_string_len, 0, 
									sock_addr->ai_addr, sock_addr->ai_addrlen) ;
	if (num_bytes < 0)
		die_with_sys_msg("sendto() failed") ;
	else if (num_bytes != echo_string_len)
		die_with_user_msg("sendto() error", "send unexpected number of bytes") ;

	debug_msg(stdout, "sendto client successed") ;

	/* Receive a response */
	struct sockaddr_storage serv_addr ;
	socklen_t serv_addr_len = sizeof(serv_addr) ;
	char buf[MAX_STRING_LENGTH + 1] ; /* the last char storage null */

	debug_msg(stdout, "recvfrom server...") ;
		if (getsockname(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
			perror("getsockname() failed") ;
		else {
			printf("after sendto ost addr info:\n") ;
			print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
		}
		if (getpeername(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
			perror("getpeerkname() failed") ;
		else {
			printf("after sendto peer addr info:\n") ;
			print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
		}
	num_bytes = recvfrom(sock, buf, MAX_STRING_LENGTH, 0, 
							(struct sockaddr *)&serv_addr, &serv_addr_len) ;
		if (getsockname(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
			perror("getsockname() failed") ;
		else {
			printf("after asendto host addr info:\n") ;
			print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
		}
		if (getpeername(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) < 0)
			perror("getpeerkname() failed") ;
		else {
			printf("after asendto peer addr info:\n") ;
			print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;
		}
	if (num_bytes < 0)
		die_with_sys_msg("recvfrom() failed") ;
	else if (num_bytes != echo_string_len)
		die_with_user_msg("recvfrom() error", "recived unexpected number of bytes") ;
	debug_msg(stdout, "recive successed, recived data size%d", num_bytes) ;

	/* Verify reception from expected source */
	if (!sock_addrs_equal((struct sockaddr *)&serv_addr, sock_addr->ai_addr))
		die_with_user_msg("recvfrom()", "received a packet from unknow source") ;

	freeaddrinfo(addrinfo_list) ; 
	
	/* print recived string */
	buf[echo_string_len] = '\0' ;
	printf("Received: %s\n", buf) ;

	S_TEST(0, close(sock)) ;

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
