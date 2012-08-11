#ifndef  VOTE_PROTOCOL_H__INC
#define  VOTE_PROTOCOL_H__INC
/*
 * =====================================================================================
 *
 *       Filename:  vote_protocol.h
 *
 *    Description:  An vote program protocol hand filed
 *
 *        Version:  1.0
 *        Created:  05/26/2011 10:46:28 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<stdint.h>
#include	<stdbool.h>

/* the send, recive informaton form */
typedef struct voto_info {
	uint64_t count ; /* the candidate tikite count, invariant: */
	int candidate ;  /* the candidate ID invariant: 0<= cnadidate <= MAX_CONDIDATE */
	bool is_inquiry ;
	bool is_response ; /* It specified is a response or request (set is response)*/
} vote_info, *ptr_vote_info ;


enum {
	MAX_CANDIDATE = 1000, 
	MAX_WIRE_SIZE = 500, 
} ;

#endif   /* ----- #ifndef VOTE_PROTOCOL_H__INC  ----- */
