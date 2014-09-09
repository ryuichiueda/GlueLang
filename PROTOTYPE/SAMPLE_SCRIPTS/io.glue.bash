function main(){
	f=/tmp/$$-f
	 cattac $1 > /tmp/$$-f
	/bin/cat $f
}

function cattac(){
	/bin/cat $1 | /usr/bin/tail -r
}

main "$1" < /dev/stdin
