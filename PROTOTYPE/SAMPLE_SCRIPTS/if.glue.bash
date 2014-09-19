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

checkColnum(){
	c=$( /usr/local/bin/retu $1 )
ERROR_CHECK
	/bin/test "$c" = "$2"
ERROR_CHECK
}

function main(){
if checkColnum $1 $2 ; then
	/bin/echo "OK"
else
	/bin/false
fi
}

main "$1" "$2"
ERROR_CHECK

rm -f /tmp/$$-*
