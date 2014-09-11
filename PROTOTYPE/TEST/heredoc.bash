#!/bin/bash -e

tmp=/tmp/$$
dir=$(dirname $0)/../

ERROR_CHECK(){
	[ "$(echo ${PIPESTATUS[@]} | tr -d ' 0')" = "" ] && return
	rm -f $tmp-*
	echo ERROR >&2
	exit 1
}


seq 1 10					|
$dir/gluelang $dir/SAMPLE_SCRIPTS/heredoc.glue	> $tmp-out
ERROR_CHECK

cat << EOF > $tmp-ans
2 1
えお あいう
aho
EOF

diff $tmp-out $tmp-ans ; ERROR_CHECK

echo OK
rm -f $tmp-*
