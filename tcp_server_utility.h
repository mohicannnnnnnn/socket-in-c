/*
 * =====================================================================================
 *
 *       Filename:  tcp_server_utility.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/22/2011 08:16:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  _TCP_SERVER_UTILITY_H_INC
#define  _TCP_SERVER_UTILITY_H_INC

/* Mamimum outstanding connection requets */
static const int MAXPENDING = 5 ;
#define WEL_PAGE_MAXLEN  500 /* welcome page max length */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_welcome_page
 *  Description:  when the connect as well begin as send welcome page to client
 * =====================================================================================
 */
void send_welcome_page(int clnt_sock, const char *welcome_page) ;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  recv_welcome_page
 *  Description:  client recive welcome page from server
 * =====================================================================================
 */
void recv_welcome_page(int clnt_sock) ;

/* handle tcp clinet recive messsage from client and send to client */
void handle_tcp_client(int clet_socket) ;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setup_tcp_server_socket
 *  Description:  An generic create tcp socket function, and bind list.passing service
 *				  it return a binded to and listen sepcified address(port) of socket.
 *				  if cant' create, bind, listen It return -1.
 * =====================================================================================
 */
int setup_tcp_server_socket(const char *service) ;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  accept_tcp_connection
 *  Description:  accepting client connections a generic address
 * =====================================================================================
 */
int accept_tcp_connection(int serv_sock) ;
#endif   /* ----- #ifndef _TCP_SERVER_UTILITY_H_INC  ----- */
