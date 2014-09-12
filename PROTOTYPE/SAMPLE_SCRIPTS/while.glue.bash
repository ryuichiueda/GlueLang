ERROR_EXIT(){
	rm -f /tmp/$$-*
	exit 1
}

ERROR_CHECK(){
	[ "$(tr -d ' 0' <<< ${PIPESTATUS[@]})" = "" ] && return
	ERROR_EXIT
}

trap ERROR_EXIT 2

foreach(){

	while read line ; do
		"$1" $line
		ERROR_CHECK
	done
	ERROR_CHECK
}

plus(){
	echo $1 $2 | /usr/local/bin/awk '{print $1+$2+1}'
	ERROR_CHECK
}

main(){
f=/tmp/$$-f
	 foreach plus < /dev/stdin > /tmp/$$-f
/bin/cat $f
}

main
