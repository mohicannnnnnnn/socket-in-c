#ifndef  VOTE_ENCODING_BIN_H__INC
#define  VOTE_ENCODING_BIN_H__INC

/*
 * =====================================================================================
 *
 *       Filename:  vote_encoding_bin.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/26/2011 08:29:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<stdint.h>
#include	"vote_protocol.h"

enum {
	REQUEST_SIZE = 4,
	RESPONSE_SIZE = 12, 
	COUNT_SHIFT =32, 
	INQUIRE_FLAG = 0x0100, 
	RESPONSE_FLAG = 0x0200, 
	MAGIC = 0x5400,		 /* 0101 0100 0000 0000 */
	MAGIC_MASK =  0xfc00,  /* 1111 1100 0000 0000 */
};

/* be transimssion vote msg form */
typedef struct vote_msg {
	uint16_t header;
	uint16_t candidate_id;
	uint32_t count_high;
	uint32_t count_low;
}vote_msg_bin, *ptr_vote_msg_bin ;

size_t encode(vote_info *v, uint8_t *outbuf, size_t buf_size) ;

bool decode(uint8_t *inbuf, size_t recv_size, vote_info *v) ;
#endif   /* ----- #ifndef VOTE_ENCODING_BIN_H__INC  ----- */
