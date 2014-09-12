ERROR_EXIT(){
	rm -f /tmp/$$-*
	exit 1
}

ERROR_CHECK(){
	[ "$(tr -d ' 0' <<< ${PIPESTATUS[@]})" = "" ] && return
	ERROR_EXIT
}

trap ERROR_EXIT 2

plus(){
	echo $1 | /usr/local/bin/awk '{print $1+1}'
	ERROR_CHECK
}

main(){
f=/tmp/$$-f
	 eachline plus < $1 > /tmp/$$-f
}

main "$1" < /dev/stdin
