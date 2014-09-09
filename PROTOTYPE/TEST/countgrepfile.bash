#!/bin/bash -e

tmp=/tmp/$$
dir=$(dirname $0)/..

ERROR_CHECK(){
	[ "$(echo ${PIPESTATUS[@]} | tr -d ' 0')" = "" ] && return
	rm -f $tmp-*
	echo ERROR >&2
	exit 1
}

$dir/gluelang $dir/SAMPLE_SCRIPTS/countgrepfile.glue 'countgrepfile\.glue' $dir/SAMPLE_SCRIPTS |
sort > $tmp-out

#bash -vxe $dir/SAMPLE_SCRIPTS/countgrepfile.glue.bash 'countgrepfile\.glue'	$dir/SAMPLE_SCRIPTS	|
#sort > $tmp-out

cat <<FIN > $tmp-ans
2
FIN

diff $tmp-out $tmp-ans ; ERROR_CHECK

echo OK
rm -f $tmp-*
