ERROR_EXIT(){
	rm -f /tmp/$$-*
	exit 1
}

ERROR_CHECK(){
	[ "$(tr -d ' 0' <<< ${PIPESTATUS[@]})" = "" ] && return
	ERROR_EXIT
}

trap ERROR_EXIT 1 2 3 15

foreach(){

	while read line ; do
		"$1" $line
		ERROR_CHECK
	done
	ERROR_CHECK
}

cattac(){
/bin/cat $1 | /usr/bin/tail -r
	ERROR_CHECK
}

main(){
	f=$(mktemp /tmp/$$-f)
ERROR_CHECK
 cattac $1 > $f
ERROR_CHECK

	/bin/cat $f
ERROR_CHECK

}

main "$1"
ERROR_CHECK

rm -f /tmp/$$-*
