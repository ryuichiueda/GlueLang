#!/bin/bash -e

tmp=/tmp/$$
dir=$(dirname $0)/../

ERROR_CHECK(){
	[ "$(echo ${PIPESTATUS[@]} | tr -d ' 0')" = "" ] && return
	rm -f $tmp-*
	echo ERROR >&2
	exit 1
}

$dir/langToBash $dir/SAMPLE_SCRIPTS/cattac.glue

seq 1 10					|
bash -xve $dir/SAMPLE_SCRIPTS/cattac.glue.bash	> $tmp-out

seq 1 10				|
tail -r					> $tmp-ans

diff $tmp-out $tmp-ans ; ERROR_CHECK

echo OK
rm -f $tmp-*
