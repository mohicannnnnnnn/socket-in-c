#!/bin/bash - 
#===============================================================================
#
#          FILE:  link-tcp-echo-server.sh
# 
#         USAGE:  ./link-tcp-echo-server.sh 
# 
#   DESCRIPTION:  
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: lili, zhao (), lilijreeyGamil.com
#       COMPANY: 
#       CREATED: 12/06/2011 11:32:50 PM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

i=10 ;
while (( i > 0 )) 
do
	./tcp_echo_client 127.1.1.1 $i 5555 
	echo $i;
	(( i-- ));
done


