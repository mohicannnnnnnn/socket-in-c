/*
 * =====================================================================================
 *
 *       Filename:  vote_server_tcp.c
 *
 *    Description:  An voto server progream  
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
#include	<stdbool.h>
#include	<unistd.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>

#include	<zhao_include/myUtility.h>

#include	"practical.h"
#include	"tcp_server_utility.h"
#include	"vote_protocol.h"
#include	"vote_encoding_bin.h"
#include	"framer.h"


/* Welcome page */
static const char welcome_page[] =  
	"##########################################################################\n\r"
	"#\t\t\t Welcome to Vote Server \t\t\t#\n\r"
	"##########################################################################\n\r"  ;

/* candidate data it can read from a file */
static uint64_t counts[MAX_CANDIDATE + 1 ] ;

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
	die_with_user_msg("Usage:Parameter(s)", "<Server Port/Service>") ;


/* Create a connect TCP socket */
	int serv_sock = setup_tcp_server_socket(argv[1]) ;
	if (serv_sock < 0) 
		die_with_user_msg("setup_tcp_server_socket() failed", "unable to connect") ;
	debug_msg(stdout, "setup server socket successed") ;


	for (;;) {
		/* wait for a client to connect */
		int clnt_sock = accept_tcp_connection(serv_sock) ;

		/* Send welcome page to vote client */
		send_welcome_page(clnt_sock, welcome_page) ;

		/* Create an input stream from the socket */
		FILE *channel = fdopen(clnt_sock, "r+") ;
		if (NULL == channel) 
			die_with_sys_msg("fdopen() failed") ;
		debug_msg(stdout, "bind client socket to file stream successed") ;

		/* Recive message untill connection closeed */
		int recv_size ;
		uint8_t inbuf[MAX_WIRE_SIZE] ;
		vote_info v ;
		while ((recv_size = get_next_msg(channel, inbuf, MAX_WIRE_SIZE)) > 0) {
			memset(&v, 0, sizeof(v)) ;
			printf("Rceived message (%d bytes)\n", recv_size) ;
			if (decode(inbuf, recv_size, &v)) {
				if (!v.is_response) { /* Ignore non-request */
					v.is_response = true ;
					if (v.candidate >= 0 && v.candidate <= MAX_CANDIDATE) {
						if (!v.is_inquiry)
							counts[v.candidate] += 1 ;

						v.count = counts[v.candidate] ;
					} /* Ignore invalid candidates */
				}
				uint8_t outbuf[MAX_WIRE_SIZE] ;
				recv_size = encode(&v, outbuf, MAX_WIRE_SIZE) ;
				if (put_msg(outbuf, recv_size, channel) < 0) {
					fputs("Error framing/outpoutting message\n", stderr) ;
					break ;
				} else {
					printf("Processed %s for candidate %d; current count is %llu.\n", 
							(v.is_inquiry ? "inquiry" : "vote"), v.candidate, v.count) ;
				}
				fflush(channel) ;
			} else { /* decode error */
				fputs("Decode error, closing connection.\n", stderr) ;
				break ;
			}
		}
	
		puts("Client connect closed") ;	
		/* Close server stream and server socket */
		S_TEST(0, fclose(channel))
	}

	S_TEST(0, close(serv_sock))
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
