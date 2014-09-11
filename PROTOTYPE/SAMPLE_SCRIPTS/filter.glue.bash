ERROR_EXIT(){
	rm -f /tmp/$$-*
	exit 1
}

ERROR_CHECK(){
	[ "$(tr -d ' 0' <<< ${PIPESTATUS[@]})" = "" ] && return
	ERROR_EXIT
}

trap ERROR_EXIT 2

mainfilter(){
	/bin/cat $1 | /usr/bin/tail -r
	ERROR_CHECK
}

main(){
	mainfilter $1
	ERROR_CHECK
}

main "$1" < /dev/stdin
