#!/bin/bash -evx

function main(){
	find $2 | grep $1
}

main "$1" "$2"
