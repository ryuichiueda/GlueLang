ERROR_EXIT(){
	rm -f /tmp/$$-*
	exit 1
}

ERROR_CHECK(){
	[ "$(tr -d ' 0' <<< ${PIPESTATUS[@]})" = "" ] && return
	ERROR_EXIT
}

trap ERROR_EXIT 2

main(){
awk=/tmp/$$-awk
cat << 'EOF' >> /tmp/$$-awk
{print $2,$1}
END{print "aho"}
EOF
data=/tmp/$$-data
cat << 'EOF' >> /tmp/$$-data
1 2 3
あいう えお
EOF
/usr/local/bin/awk -f $awk $data
}

main < /dev/stdin
