function main(){
	/usr/bin/find $2 | /usr/bin/grep $1 | /usr/local/bin/gyo
}

main "$1" "$2" < /dev/stdin
