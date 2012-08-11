/*
 * =====================================================================================
 *
 *       Filename:  vote_encoding_bin.c
 *
 *    Description:  based-on binary encoding of vote message
 *
 *        Version:  1.0
 *        Created:  05/26/2011 04:56:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<string.h>
#include	<stdbool.h>
#include	<stdlib.h>
#include	<stdint.h>
#include	<netinet/in.h>

#include	<zhao_include/myUtility.h>
#include	"practical.h"	
#include	"vote_encoding_bin.h"
#include	"vote_protocol.h"

/* binary form of vote message
 *  0	1	2	3	4	5	6	7	8	9	11	12	13	14	15 
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * |	Magic              | Flags |       ZERO				   |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * |                    Candidate  ID                          |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * |       Vote Count (only in response)				       |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  encode
 *  Description:  The flags indecate if a request.
 * =====================================================================================
 */

size_t encode(vote_info *v, uint8_t *outbuf, size_t buf_size)
{
	debug_msg(stdout, "encoding message") ;
	if (NULL == v || NULL == outbuf) 
		die_with_user_msg("Error: ", "encode() argument invalid") ;

	size_t encode_size = REQUEST_SIZE ; /* be encodede msg size */

	if ((v->is_response && buf_size < sizeof(vote_msg_bin)) 
			|| buf_size < 2 * sizeof(uint16_t)) 
		die_with_user_msg("output buffe too small", "") ;

	ptr_vote_msg_bin vm = (ptr_vote_msg_bin)outbuf ;
	memset(outbuf, 0, sizeof(vote_msg_bin)) ;
	/* set outbuf info */
	vm->header = MAGIC ;
	if (v->is_inquiry) 
		vm->header |= INQUIRE_FLAG ;
	if (v->is_response) {
		vm->header |= RESPONSE_FLAG ;
		vm->count_high = htonl(v->count >> COUNT_SHIFT) ;
		vm->count_low = htonl((uint32_t)v->count) ;
		encode_size = RESPONSE_SIZE ;
	}

	/* conversion to net order */
	vm->header = htons(vm->header) ;
	vm->candidate_id = htons(v->candidate) ;

	debug_msg(stdout, "encoding message finished, encode message size:%d", encode_size) ;
	return encode_size ;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  decode
 *  Description:  Extract message info from given buffer,  beas-on binary encodeing.
 *				  inbuf recive to info from socket, recv_size:the inbuf data size
 * =====================================================================================
 */

bool decode(uint8_t *inbuf, size_t recv_size, vote_info *v) 
{
	if (NULL == inbuf || NULL == v)
		die_with_user_msg("decode ", "argument invaild") ;

	ptr_vote_msg_bin vm = (ptr_vote_msg_bin)inbuf ;

	/* Attend to byte order */
	uint16_t header = ntohs(vm->header) ;
	if (recv_size < REQUEST_SIZE || ((header & MAGIC_MASK) != MAGIC)) {
		debug_msg(stderr, "recv_size:%d < request_size or is a invaild message", recv_size) ;
		return false ;
	}

	/* Message is big enough and include correct magic number, set v */
	v->is_response = ((header & RESPONSE_FLAG) != 0) ;
	v->is_inquiry = ((header & INQUIRE_FLAG) != 0) ;
	v->candidate = ntohs(vm->candidate_id) ;
	if (v->is_response && recv_size >= RESPONSE_SIZE) {
		v->count = ((uint64_t)ntohl(vm->count_high) << COUNT_SHIFT)
			| (uint64_t)ntohl(vm->count_low) ;
	}
	return true ;
}
