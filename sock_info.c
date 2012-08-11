/*
 * =====================================================================================
 *
 *       Filename:  sock_info.c
 *
 *    Description:  use to getsockopt print socket info
 *
 *        Version:  1.0
 *        Created:  05/27/2011 10:07:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netdb.h>
#include	<zhao_include/myUtility.h>

#include	<stdio.h>
int main()
{
	int sock ;
	EV_TEST(-1, sock, socket(AF_INET, SOCK_STREAM, 0))
	
	printf("SOL_SOCKET:\n") ;
	int opt = 0 ;
	socklen_t optlen = sizeof(int) ;
	E_TEST(-1, getsockopt(sock, SOL_SOCKET, SO_BROADCAST, 
				&opt, &optlen))
	printf("SO_BROADCAST:%d.\n", opt) ;
	E_TEST(-1, getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, 
				&opt, &optlen))
	printf("SO_KEEPALIVE:%d.\n", opt) ;
	E_TEST(-1, getsockopt(sock, SOL_SOCKET, SO_RCVBUF, 
				&opt, &optlen))
	printf("SO_RCVBUF:%d.\n", opt) ;
	E_TEST(-1, getsockopt(sock, SOL_SOCKET, SO_RCVLOWAT, 
				&opt, &optlen))
	printf("SO_RCVLOWAT:%d.\n", opt) ;
	E_TEST(-1, getsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
				&opt, &optlen))
	printf("SO_REUSEADDR:%d.\n", opt) ;
	E_TEST(-1, getsockopt(sock, SOL_SOCKET, SO_SNDLOWAT, 
				&opt, &optlen))
	printf("SO_SNDLOWAT:%d.\n", opt) ;
	E_TEST(-1, getsockopt(sock, SOL_SOCKET, SO_SNDBUF, 
				&opt, &optlen))
	printf("SO_SNDBUF:%d.\n", opt) ;

	printf("IPRPOTO_IP:\n") ;
	E_TEST(-1, getsockopt(sock, IPPROTO_IP, IP_TTL, 
				&opt, &optlen))
	printf("IP_TTL:%d.\n", opt) ;
	E_TEST(-1, getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, 
				&opt, &optlen))
	printf("IP_MULTICAST_TTL:%d.\n", opt) ;
	E_TEST(-1, getsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, 
				&opt, &optlen))
	printf("IP_MULTICAST_LOOP:%d.\n", opt) ;

	return 0 ;
}
