/*
 * =====================================================================================
 *
 *       Filename:  vote_client_tcp.c
 *
 *    Description:  An voto client progream, This can inquiry a sepcified candidate
 *					tikete count, and voto responce(by default)
 *
 *        Version:  1.0
 *        Created:  05/26/2011 10:54:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<stdint.h>
#include	<unistd.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<arpa/inet.h>

#include	<zhao_include/myUtility.h>

#include	"practical.h"
#include	"tcp_client_utility.h"
#include	"tcp_server_utility.h"
#include	"vote_encoding_bin.h"
#include	"vote_protocol.h"
#include	"framer.h"



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	if (argc < 4 || argc > 5) 
		die_with_user_msg("Usage:Parameter(s)", "<Server> <Port/Service> <Candidate> [I]\n"
				          "Server is you want connect voto host address or name; " 
						  "I specify this is a inquiry(default voto request)\n") ;


	char *server = argv[1] ;
	char *service = argv[2] ;
	int candi = atoi(argv[3]) ;
	if (candi < 0 || candi > MAX_CANDIDATE)  /* test candidate ID */
		die_with_user_msg("Candidate # not valid", argv[3]) ;

	bool inq = argc > 4 && strcmp(argv[4], "I") == 0 ;

	/* Create a connect TCP socket */
	int serv_sock = setup_tcp_client_socket(server, service) ;
	if (serv_sock < 0) 
		die_with_user_msg("setup_tcp_client_socket() failed", "unable to connect") ;

	/* recive welcome page from vote server */
	recv_welcome_page(serv_sock) ;
	
	/* bind sock to FILE */
	FILE *serv_str = fdopen(serv_sock, "r+") ;
	if (NULL == serv_str)
		die_with_sys_msg("fdopen()failed") ;

	/* set up info for a request */
	vote_info vi ;
	memset(&vi, 0, sizeof(vi)) ;

	vi.is_inquiry = inq ;
	vi.candidate = candi ;

	/* Encode for transmission */
	uint8_t outbuf[MAX_WIRE_SIZE] ;
	uint8_t req_size = encode(&vi, outbuf, MAX_WIRE_SIZE) ;

	/* print info */
	printf("Sending %d-byte %s for candidate %d...\n", req_size, 
			(inq ? "inquiry" : "voto"), candi) ;

	/* Fram and end */
	debug_msg(stdout, "putting vote message from client to server") ;
	if (put_msg(outbuf, req_size, serv_str) < 0)
		die_with_sys_msg("put_msg() falied") ;

	/* Recive and print response */
	uint8_t inbuf[MAX_WIRE_SIZE] ;
	size_t resp_size = get_next_msg(serv_str, inbuf, MAX_WIRE_SIZE) ;

	if (decode(inbuf, resp_size, &vi)) {
		printf("Received:\n") ;
		if (vi.is_response)
			printf("\tResponse to ") ;
		if (vi.is_inquiry)
			printf("inquiry ") ;
		else
			printf("vote") ;
		printf("for candidate %d\n", vi.candidate) ;
		if (vi.is_response)
			printf("\tCount = %llu\n", vi.count) ;
	}

	/* Close server stream and server socket */
	/* 对于套接子fclose同时关闭流和套接字 */
	S_TEST(0, fclose(serv_str))


	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
