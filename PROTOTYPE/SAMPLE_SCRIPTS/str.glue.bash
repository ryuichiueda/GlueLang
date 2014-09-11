function main(){
	s=$( cattac )
	echo $s
}

function cattac(){
	/bin/cat $1 | /usr/bin/tail -r
}

main < /dev/stdin
