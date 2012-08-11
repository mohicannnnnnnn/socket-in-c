/*
 * =====================================================================================
 *
 *       Filename:  framer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/26/2011 01:13:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<stdint.h>
#include	<netinet/in.h>

#include	<zhao_include/myUtility.h>
#include	"framer.h"
#include	"practical.h"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  length_framer
 *  Description:  based lenght framing a message. lenght fialed use two bytes.
 *				  Then read the indicated number of bytes.
 *				  If the input buffer too small for the data, multip send untill send
 *				  over the message. Returns -1 on any error
 * =====================================================================================
 */
int put_msg(uint8_t *buf, size_t msg_size, FILE *out)
{
	if (NULL == buf || NULL == out || msg_size > UINT16_MAX) {
		debug_msg(stderr, "Error: graument invalid") ;
		return -1 ;
	}

	uint16_t payload_size = htons(msg_size) ;
	/* Write the message size to socket */
	if (fwrite(&payload_size, sizeof(uint16_t), 1, out) != 1) {
		debug_msg(stderr, "Error: fwrite failed") ;
		return -1 ;
	}
	
	/* Write the message to socket */
	if (fwrite(buf, sizeof(uint8_t), msg_size, out) != msg_size) {
		debug_msg(stderr, "Error: fwrite failed") ;
		return -1 ;
	}

	fflush(out) ;
	debug_msg(stdout, "put message successed. put message size:%d", msg_size) ;

	return msg_size ;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_next_msg
 *  Description:  based on lenght_framer get msg from channel. Read 2-bytes lenght and 
 *				  place in big-endian order.
 *				  Then read the indicated number of bytes.
 *				  If the input buffer is smaller then data-length, truncate to fit and
 *				  return .
 * =====================================================================================
 */

int get_next_msg(FILE *in, uint8_t *buf, size_t buf_size)
{
	uint16_t recv_size = 0 ;
	uint16_t extra_size = 0 ;

	if (fread(&recv_size, sizeof(uint16_t), 1, in) != 1) {
		if (feof(in))
			debug_msg(stdout, "fread the stream end-of-file") ;
		if (ferror(in))
			die_with_sys_msg("fread() failed") ;
		return -1 ;
	}
	recv_size = ntohs(recv_size) ;
	debug_msg(stdout, "get a message parcket size:%d", recv_size) ;

	if (recv_size > buf_size) {
		debug_msg(stderr, "recive message size more then buf_size") ;
		extra_size = recv_size - buf_size ;
		recv_size = buf_size ;
	}

	if (fread(buf, sizeof(uint8_t), recv_size, in) != recv_size) {
		debug_msg(stderr, "Framing error: expected %d, read less\n", recv_size) ;
		return -1 ;
	}
	if (extra_size > 0) { /* If has extra */
		uint8_t waste[BUFSIZE] ;
		fread(waste, sizeof(uint8_t), extra_size, in) ;
		return -(recv_size + extra_size) ;
	} else 
		return recv_size ;
}
