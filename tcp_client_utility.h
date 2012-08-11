/*
 * =====================================================================================
 *
 *       Filename:  tcp_client_utility.h
 *
 *    Description:  a utility set for client end.
 *
 *        Version:  1.0
 *        Created:  05/24/2011 11:36:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setup_tcp_client_socket
 *  Description:  setup takes a host and service. specified in a string, and
 *				  returns a connected socket, or -1 on failure.
 *        Input:  host and service string.It may be specified as NULL
 * =====================================================================================
 */
int setup_tcp_client_socket(const char *host, const char *service) ;
