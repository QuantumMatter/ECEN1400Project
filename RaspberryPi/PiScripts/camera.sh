#!/bin/sh

raspivid -t 9999999 -w 960 -h 540 -fps 25 -b 500000 -vf -o - | ffmpeg -i - -vcodec copy -an -r 25 -f flv rtmp://davidkopala.com/live/test
