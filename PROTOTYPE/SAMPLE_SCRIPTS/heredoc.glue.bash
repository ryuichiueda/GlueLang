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
	awk=$(mktemp /tmp/$$-awk)
ERROR_CHECK
cat << 'EOF' >> $awk
{print $2,$1}
END{print "aho"}
EOF
ERROR_CHECK

	data=$(mktemp /tmp/$$-data)
ERROR_CHECK
cat << 'EOF' >> $data
1 2 3
あいう えお
EOF
ERROR_CHECK

	/usr/local/bin/awk -f $awk $data
ERROR_CHECK

}

main 
ERROR_CHECK

rm -f /tmp/$$-*
