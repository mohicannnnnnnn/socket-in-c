/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/2011 08:30:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
/* test err_msg */
#if 0
#include	<zhao_include/myUtility.h>

#include	<stdio.h>
int main()
{
	const char welcome_page[] = "################################################################\n\r"
		                        "\t\t\tWelcome to TCP Echo Server\n\r"
								"################################################################\n\r" ;
	fputs(welcome_page, stdout) ;
	return 0 ;
}
#endif

// about const int and #define
#if 0
#include	<stdio.h>
#include	<stdlib.h>
#define A  10 


static int a = 10 ;
int main()
{
	int buf[A] ;
	buf[1] = 1 ;

	int bufa[a] ;
	bufa[2] = a ;

	printf("long long int size:%d", sizeof(long long int)) ;

	/* types conversion  int to usineg long long*/

	int inta = -1 ;
	unsigned long long lla = (unsigned long long)inta ;
	printf("lla:%lld\n", lla) ; /* lla=FFFFFFFF...FF */

	long long llb = (long long)inta ;
	printf("lla:%lld\n", llb) ; /* lla=FFFFFFFF...FF */

	unsigned int una = inta ;
	printf("una:%d\n", una) ;
	return 0 ;

}
#endif

//fd bind to FILE, then the close FILE stream if the fd was closed
#if 0
#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<sys/stat.h>
#include	<fcntl.h>

int main()
{
	int fd ;
	FILE *stream ; 
	if ((fd = open("aa.txt", O_RDWR)) != -1)
		printf("create aa.txt successed, fd=%d\n", fd) ;
	else
		perror("create:") ;

	if ((stream = fdopen(fd, "w")) != NULL)
			printf("fdopen successed\n") ;
	else 
		perror("fdopen:") ;

	if(0 == fclose(stream)) 
		printf("close stream successed\n") ;
	else 
		perror("close(stream)") ;

	if(0 == close(fd)) 
		printf("close fd successed\n") ;
	else 
		perror("close(fd)") ;
	return 0 ;
}
#endif


#include	<stdio.h>
#include	<limits.h>
#include	<stdint.h>
//implement a htonll() conversion a 64 bit int from host-endian to net-endian
uint64_t htonll(uint64_t num) 
{
	printf("orignal:%llx\n", num) ;
	uint64_t retn_val = 0 ;
	for (int i=0; i < sizeof(uint64_t); i++) {
		retn_val <<= CHAR_BIT ;
		retn_val |= (uint8_t)(num >> i * CHAR_BIT) ;
	}
	return retn_val ;
}
int main() {
	/* test host mechinc endian */
	union endian {
		char end[4] ;
		char answer ;
	} ;
	
	union endian machine ={.end = {'s', '0', '0', 'b'}} ;
	if (machine.answer == 's') {
		/* local mechine endia is small endian conversion it */
		printf("DebugMsg:small endian") ;
		uint64_t num = 0x123456700aabbcc ;
		num = htonll(num) ;
		printf("num:%llx\n", num) ;

	} else {
		/* local mechina endia is big endian just return  */
		printf("DebugMsg:endian") ;
	}

	return 0 ;
}

