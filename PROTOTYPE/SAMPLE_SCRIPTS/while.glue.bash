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
}

plus(){
echo $1 $2 | /usr/local/bin/awk '{print $1+$2+1}'
	ERROR_CHECK
}

main(){
	f=$(mktemp /tmp/$$-f)
ERROR_CHECK
 foreach plus < /dev/stdin > $f
ERROR_CHECK

	/bin/cat $f
ERROR_CHECK

}

main 
ERROR_CHECK

rm -f /tmp/$$-*
