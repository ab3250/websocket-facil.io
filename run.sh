#!/usr/bin/env bash

sudo killall chibi-scheme > /dev/null 2>&1

sudo killall nginx > /dev/null 2>&1

chibi-scheme -r main.scm

#http://localhost:8080/index.html
