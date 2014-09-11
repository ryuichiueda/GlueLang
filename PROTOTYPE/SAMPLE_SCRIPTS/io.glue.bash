ERROR_EXIT(){
	rm -f /tmp/$$-*
	exit 1
}

ERROR_CHECK(){
	[ "$(tr -d ' 0' <<< ${PIPESTATUS[@]})" = "" ] && return
	ERROR_EXIT
}

trap ERROR_EXIT 2

cattac(){
	/bin/cat $1 | /usr/bin/tail -r
	ERROR_CHECK
}

main(){
f=/tmp/$$-f
	 cattac $1 > /tmp/$$-f
/bin/cat $f
}

main "$1" < /dev/stdin
