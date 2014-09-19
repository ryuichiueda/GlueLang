#!/bin/bash

tmp=/tmp/$$
dir=$(dirname $0)

ERROR_CHECK(){
	[ $(plus ${PIPESTATUS[@]} ) -eq 0 ] && return
	rm -f $tmp-*
	exit 1
}

cat << FIN > $tmp-list
filter
io
str
if
heredoc
while
FIN

cat $tmp-list 	|
while read i ; do
	echo -en $i "\t"
	$dir/$i.bash
	ERROR_CHECK
done
