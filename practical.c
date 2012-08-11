/*
 * =====================================================================================
 *
 *       Filename:  practical.c
 *
 *    Description:  an Implementation practical.h of Socket in C
 *
 *        Version:  1.0
 *        Created:  05/21/2011 02:16:18 PM
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
#include	<unistd.h>
#include	<arpa/inet.h>

#include "practical.h"
#include	<zhao_include/myUtility.h>

void die_with_user_msg(const char *msg, const char *detail) 
{
	fputs(msg, stderr) ;
	fputs(": ", stderr) ;
	fputs(detail, stderr) ;
	fputc('\n', stderr) ;
	exit(1) ;
}


void die_with_sys_msg(const char *msg)
{
	perror(msg) ;
	exit(1) ;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  socktype_to_str
 *  Description:  get spcifiy socket type string name 
 * =====================================================================================
 */
char *socktype_to_str(int socktype, char *buffer)
{
	if(socktype <0 || NULL == buffer)
		die_with_user_msg("socktype_to_str()", "arguments is inavialble") ;
	

	switch ( socktype ) {
		case SOCK_STREAM:	
			snprintf(buffer, SOCKTYPE_MAX_LEN,  "SOCK_STREAM") ;
			break;
		case SOCK_DGRAM:	
			snprintf(buffer, SOCKTYPE_MAX_LEN, "SOCK_DGRAM") ;
			break;
		case SOCK_SEQPACKET:	
			snprintf(buffer, SOCKTYPE_MAX_LEN, "SOCK_SEQPACKET") ;
			break;
		case SOCK_RAW:	
			snprintf(buffer, SOCKTYPE_MAX_LEN, "SOCK_RAW") ;
			break;
		case SOCK_RDM:	
			snprintf(buffer, SOCKTYPE_MAX_LEN, "SOCK_RDM") ;
			break;
		case SOCK_PACKET:	
			snprintf(buffer, SOCKTYPE_MAX_LEN, "SOCK_PACKET") ;
			break;

		default:	
			snprintf(buffer, SOCKTYPE_MAX_LEN, "unknown socket type") ;
			break;
	}				/* -----  end switch  ----- */
	return buffer ;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_socket_type
 *  Description:  print socket type 
 * =====================================================================================
 */
void print_socket_type(int socktype, FILE *stream) 
{
	if(socktype < 0 || NULL == stream)
		die_with_user_msg("print_socket_type()", "arguments is inavialble") ;
	char buffer[SOCKTYPE_MAX_LEN] ;
	
	fprintf(stream, "socktype:%-15s", socktype_to_str(socktype, buffer)) ;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_family_type
 *  Description:   print family name string 
 * =====================================================================================
 */
void print_family_type(int family, FILE *stream)
{
	if(family < 0 || NULL == stream)
		die_with_user_msg("print_family_type()", "arguments is inavialble") ;

	char buffer[FAMILY_MAX_LEN] ;
	
	fprintf(stream, "family:%-15s", family_to_str(family, buffer)) ;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  family_to_str
 *  Description:   translate form int to string name of net family, 
 *					return the translate buffer
 * =====================================================================================
 */
char *family_to_str(int family, char *buffer)
{
	
	if(family <0 || NULL == buffer)
		die_with_user_msg("family_to_str()", "arguments is inavialble") ;

	switch ( family ) {
		case AF_UNIX:	
			snprintf(buffer, FAMILY_MAX_LEN, "UNIX") ;
			break;

		case AF_INET:	
			snprintf(buffer, FAMILY_MAX_LEN, "INET") ;
			break;

		case AF_INET6:	
			snprintf(buffer, FAMILY_MAX_LEN, "INET6") ;
			break;

		case AF_IPX:	
			snprintf(buffer, FAMILY_MAX_LEN, "IPX") ;
			break;

		case AF_NETLINK:	
			snprintf(buffer, FAMILY_MAX_LEN, "NETLINK") ;
			break;

		case AF_X25:	
			snprintf(buffer, FAMILY_MAX_LEN, "X25") ;
			break;

		case AF_AX25:	
			snprintf(buffer, FAMILY_MAX_LEN, "AX25") ;
			break;

		case AF_ATMPVC:	
			snprintf(buffer, FAMILY_MAX_LEN, "ATMPVC") ;
			break;

		case AF_APPLETALK:	
			snprintf(buffer, FAMILY_MAX_LEN, "APPLETALK") ;
			break;

		case AF_PACKET:	
			snprintf(buffer, FAMILY_MAX_LEN, "PACKET") ;
			break;

		default:	
			snprintf(buffer, FAMILY_MAX_LEN, "family type unknown") ;
			break;
	}				/* -----  end switch  ----- */
	return buffer ;
}


/*
 * print the socket family, address, port
 */
void print_socket_addr(const struct sockaddr *address, FILE *stream) 
{
	if(NULL == address || NULL == stream)
		die_with_user_msg("print_socket_addr()", "arguments is inavialble") ;

	void *numeric_address ; /* pointer to binary address */
	char addr_buffer[INET6_ADDRSTRLEN] ;	/* hold string address */
	char family_buffer[FAMILY_MAX_LEN] ;
	in_port_t  sock_port ;

	/* cheek address family type */
	family_to_str(address->sa_family, family_buffer) ;
	fprintf(stream, "domain:%s ", family_buffer) ;

	switch (address->sa_family) {
		case AF_INET:
			numeric_address = &((struct sockaddr_in *)address)->sin_addr ;
			sock_port = ntohs(((struct sockaddr_in *)address)->sin_port) ;
			fprintf(stream, "inet addr:") ;
			break ;

		case AF_INET6:
			numeric_address = &((struct sockaddr_in6 *)address)->sin6_addr ;
			sock_port = ntohs(((struct sockaddr_in6 *)address)->sin6_port) ;
			fprintf(stream, "inet6 addr:") ;
			break ;

		default:
			return ;
	}

	/* print address port */
	if(NULL == inet_ntop(address->sa_family, numeric_address, 
							addr_buffer, sizeof(addr_buffer))) {
		fputs("[invalid address]", stream) ;
	} else {
		fprintf(stream, "%s ", addr_buffer) ;
		fprintf(stream, "port:%u\n", sock_port) ;
	}
}

/*
 * print the socket family, address, port for IPv6
 */
void print_socket_addr6(const struct sockaddr *address, FILE *stream) 
{
	print_socket_addr(address, stream) ;
}

/*   Test two socket info equality that family, port address
 *   Only use family of INET v4 v6*/
bool sock_addrs_equal(const struct sockaddr *addr1, const struct sockaddr *addr2)
{
	if (addr1 == NULL || addr2 == NULL)
		return addr1 == addr2 ;
	else if (addr1->sa_family != addr2->sa_family)
		return false ;
	else if (addr1->sa_family == AF_INET) {
		const struct sockaddr_in *i4_addr1 = (const struct sockaddr_in *)addr1 ;
		const struct sockaddr_in *i4_addr2 = (const struct sockaddr_in *)addr2 ;
		return (i4_addr1->sin_addr.s_addr == i4_addr2->sin_addr.s_addr 
				&& i4_addr1->sin_port == i4_addr2->sin_port);
	} else if (addr1->sa_family == AF_INET6) {
		const struct sockaddr_in6 *i6_addr1 = (const struct sockaddr_in6 *)addr1 ;
		const struct sockaddr_in6 *i6_addr2 = (const struct sockaddr_in6 *)addr2 ;
		return memcmp(&i6_addr1->sin6_addr, &i6_addr2->sin6_addr, sizeof(struct in6_addr)) == 0
			&& i6_addr1->sin6_port == i6_addr2->sin6_port ;
	} else 
		return false ;
}
