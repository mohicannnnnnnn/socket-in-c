/*
 * =====================================================================================
 *
 *       Filename:  framer.h
 *
 *    Description:  about message framer fucntion
 *
 *        Version:  1.0
 *        Created:  05/26/2011 01:13:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#ifndef  FRAMER_H__INC
#define  FRAMER_H__INC

#include	<stdint.h>
#include	<stdio.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  length_framer
 *  Description:  based lenght framing a message. lenght fialed use two bytes.
 *				  Then read the indicated number of bytes.
 *				  If the input buffer too small for the data, multip send untill send
 *				  over the message.
 * =====================================================================================
 */
int put_msg(uint8_t *buf, size_t msg_size, FILE *out) ;


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
int get_next_msg(FILE *in, uint8_t *buf, size_t buf_size) ;

#endif   /* ----- #ifndef FRAMER_H__INC  ----- */
