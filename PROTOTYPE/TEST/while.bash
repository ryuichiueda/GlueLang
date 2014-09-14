#!/bin/bash -e

tmp=/tmp/$$
dir=$(dirname $0)/../

ERROR_CHECK(){
	[ "$(echo ${PIPESTATUS[@]} | tr -d ' 0')" = "" ] && return
	rm -f $tmp-*
	echo ERROR >&2
	exit 1
}


seq 1 10		|
self 1 1 		|
glue $dir/SAMPLE_SCRIPTS/while.glue	> $tmp-out
ERROR_CHECK

cat << FIN > $tmp-ans
3
5
7
9
11
13
15
17
19
21
FIN

diff $tmp-out $tmp-ans ; ERROR_CHECK

echo OK
rm -f $tmp-*
