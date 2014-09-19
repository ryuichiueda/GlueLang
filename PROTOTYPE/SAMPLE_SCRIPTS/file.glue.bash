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

main(){
	f=/tmp/$$-f
	 echo "abc" > /tmp/$$-f
ERROR_CHECK
	rev $f
ERROR_CHECK
}

main
ERROR_CHECK

rm -f /tmp/$$-*
