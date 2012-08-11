#!/bin/sh
gcc -Wall -std=gnu99 -g  vote_server_tcp.c framer.c vote_encoding_bin.c tcp_server_utility.c  practical.c  -o vote_server_tcp
