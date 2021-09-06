#!/usr/bin/env bash

sudo killall chibi-scheme > /dev/null 2>&1

sudo killall nginx > /dev/null 2>&1

sudo nginx/sbin/nginx -c /media/stor/gentoo-files/execs/compilers/programs/Chibi/facilio.ws/facil.io.ws/nginx.aws.conf &

chibi-scheme -r main.scm 


