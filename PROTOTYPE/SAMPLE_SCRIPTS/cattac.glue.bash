function main(){
	mainfilter $1
}

function mainfilter(){
	/bin/cat $1 | /usr/bin/tail -r
}

main "$1" < /dev/stdin
