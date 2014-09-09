#!/bin/bash -evx

function main(){
	cat $1 | tail -r
}

main "$1"
