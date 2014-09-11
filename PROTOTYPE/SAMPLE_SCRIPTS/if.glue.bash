ERROR_EXIT(){
	rm -f /tmp/$$-*
	exit 1
}

ERROR_CHECK(){
	[ "$(tr -d ' 0' <<< ${PIPESTATUS[@]})" = "" ] && return
	ERROR_EXIT
}

trap ERROR_EXIT 2

checkColnum(){
c=$( /usr/local/bin/retu $1 )
/bin/test "$c" = "$2"
}

function main(){
if checkColnum $1 $2 ; then
	/bin/echo "OK"
else
	/bin/false
fi
}

main "$1" "$2" < /dev/stdin
