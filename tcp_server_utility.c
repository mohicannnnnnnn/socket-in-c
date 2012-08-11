/*
 * =====================================================================================
 *
 *       Filename:  tcp_server_utility.c
 *
 *    Description:  use to handle information bewteen server and client
 *
 *        Version:  1.0
 *        Created:  05/22/2011 08:14:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<unistd.h>
#include	<string.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<errno.h>
#include	<netdb.h>

#include	"practical.h"
#include	"tcp_server_utility.h"
#include	"zhao_include/myUtility.h"

/* send welcome page to client when connected */
void send_welcome_page(int clnt_sock, const char *welcome_page) 
{
	debug_msg(stdout, "sendding welcome paga...") ;
	/* it can be read from file */
	/* const char welcome_page[] =  
		"##########################################################################\n\r"
		"#\t\t\t Welcome to TCP Echo Server \t\t\t#\n\r"
		"##########################################################################\n\r"  ;
	 */

	if(clnt_sock < 0)
		err_msg("invalid client socket:%d\n", clnt_sock) ;
	int page_len = strlen(welcome_page) ;
	int num_bytes_send= send(clnt_sock, welcome_page, page_len, 0) ;
	if(num_bytes_send < 0)
		die_with_sys_msg("send()") ;
	else if(num_bytes_send != page_len)
		die_with_user_msg("send()", "not send all of message") ;

	debug_msg(stdout,  "send welcome page successed") ;
}

/* Recive welcome page from server */
void recv_welcome_page(int clnt_sock) 
{
	debug_msg(stdout, "reciveing welcome page...") ;
	if(clnt_sock < 0)
		err_msg("invalid client socket:%d\n", clnt_sock) ;

	char welcome_page[WEL_PAGE_MAXLEN] ;
	/* non block recv */
	int num_bytes_recv = recv(clnt_sock, welcome_page, WEL_PAGE_MAXLEN, 0);
	//	printf("num_butes=%d\n", num_bytes_recv) ;
	while(num_bytes_recv > 0) {
		welcome_page[num_bytes_recv] = '\0' ;
		fputs(welcome_page, stdout) ;

    	num_bytes_recv = recv(clnt_sock, welcome_page, WEL_PAGE_MAXLEN, MSG_DONTWAIT);
		/*  上面的也可以用fcnt(df, F_SETFL, O_NONBLOCK) 来设置非阻塞recv */
		if (num_bytes_recv < 0) {
			if (errno != EWOULDBLOCK)
				die_with_sys_msg("recv failed") ;
			debug_msg(stdout, "nonblocking") ;
		}
	}
	debug_msg(stdout, "recive welcome page successed") ;
}

/* handle tcp clinet recive messsage from client and send to client */
void handle_tcp_client(int clnt_socket)
{
	char buffer[BUFSIZE] ;
	
	if(clnt_socket < 0)
		die_with_user_msg("handle_tcp_client()", "bad socket") ;

	printf("degue msg:recv\n") ;
	/* Recive message from client */
	ssize_t num_bytes_rcvd = recv(clnt_socket, buffer, BUFSIZE, 0) ;
	if(num_bytes_rcvd < 0)
		die_with_sys_msg("recv() failed") ;

	printf("degue msg:while\n") ;
	while(num_bytes_rcvd > 0) {
		ssize_t num_bytes_sent = send(clnt_socket, buffer, num_bytes_rcvd, 0) ;
		if(num_bytes_sent < 0) 
			die_with_sys_msg("snad() failed") ;
		else if (num_bytes_sent != num_bytes_rcvd)
			die_with_user_msg("send()", "sent unexpected number of bytes") ;

    	printf("degue msg:rerecv\n") ;
		/* See if there is more data to recive */
		num_bytes_rcvd = recv(clnt_socket, buffer, BUFSIZE, 0) ;
		if(num_bytes_rcvd < 0)
			die_with_sys_msg("recv() failed") ;
		printf("debug msg:while over num_bytes_rcvd=%d\n", num_bytes_rcvd) ;
	}
	
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setup_tcp_server_socket
 *  Description:  An generic create tcp socket function, and bind list.passing service
 *				  it return a binded to and listen sepcified address(port) of socket.
 *				  if cant' create, bind, listen It return -1.
 * =====================================================================================
 */
int setup_tcp_server_socket(const char *service)
{
	debug_msg(stdout, "beginning setup tcp server socket...") ;
	if (NULL == service) 
		debug_msg(stderr, "An invaild service argument") ;

	/* construct the server address structure you want*/
	struct addrinfo addr_criteria ;
	memset(&addr_criteria, 0, sizeof(addr_criteria)) ;
	addr_criteria.ai_family = AF_UNSPEC ; /* Any address family */
	addr_criteria.ai_flags = AI_PASSIVE ; /* Accept on any address/port */
	addr_criteria.ai_socktype = SOCK_STREAM ; /* Only stream sockets */
	addr_criteria.ai_protocol = IPPROTO_TCP ; /* Only TCP protocol */

	debug_msg(stdout, "get address information...") ;
	struct addrinfo *addrinfo_list = NULL ;
	int rtn_val = getaddrinfo(NULL, service, &addr_criteria, &addrinfo_list) ;
	if (0 != rtn_val)
		err_msg("getaddrinfo calling failed:%s", gai_strerror(rtn_val)) ;
	debug_msg(stdout, "get address information successed") ;
	
	int serv_sock = -1 ;
	for (struct addrinfo *i = addrinfo_list; i != NULL; i = i->ai_next) {
		/* Create a TCP server accpect socket */
		serv_sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol) ;

		/* print socket address info */
		debug_msg(stdout, "server address info:") ;
		print_socket_addr(i->ai_addr, stdout) ;

		if (serv_sock < 0) {
			debug_msg(stdout, "this server address create server socket failed") ;
			continue ;
		}

		/* Bind to the local address and set socket to list */
		debug_msg(stdout, "beginning bindding server socket...") ;
		if (0 == bind(serv_sock, i->ai_addr, i->ai_addrlen)) {
			debug_msg(stdout, "bind server socket successed") ;
		
			if ( 0 == listen(serv_sock, MAXPENDING)) {
				debug_msg(stdout, "listen successed") ;
				
				break ;
			}
		}	

		perror("bind or list failed") ;
		S_TEST(0, close(serv_sock))
		serv_sock = -1 ;
	}
	
	/* free the addrinfo list */
	freeaddrinfo(addrinfo_list) ;
	return serv_sock ;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  accept_tcp_connection
 *  Description:  accepting client connections a generic address. return the client socket
 * =====================================================================================
 */
int accept_tcp_connection(int serv_sock)
{
	if (serv_sock <= 0) 
		err_msg("An invalid servser socket") ;

	debug_msg(stdout, "acceptting a client connect...") ;
	/* A generic address */
	struct sockaddr_storage clnt_addr ;
	socklen_t clnt_addr_len = sizeof(clnt_addr) ;

	/* Wait for a client to connect */
	int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len) ;
	if(clnt_sock < 0) 
		err_msg("accept() failed") ;

	debug_msg(stdout, "Have a client connect accepted") ;
	print_socket_addr((struct sockaddr *)&clnt_addr, stdout) ;

	return clnt_sock ;
}
