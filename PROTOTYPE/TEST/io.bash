#!/bin/bash -e

tmp=/tmp/$$
dir=$(dirname $0)/../
PATH=$PATH:$dir

ERROR_CHECK(){
	[ "$(echo ${PIPESTATUS[@]} | tr -d ' 0')" = "" ] && return
	rm -f $tmp-*
	echo ERROR >&2
	exit 1
}


seq 1 10					|
glue $dir/SAMPLE_SCRIPTS/io.glue	> $tmp-out

seq 1 10				|
tail -r					> $tmp-ans

diff $tmp-out $tmp-ans ; ERROR_CHECK

echo OK
rm -f $tmp-*
