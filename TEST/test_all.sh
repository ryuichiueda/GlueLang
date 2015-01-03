#!/bin/sh -exv

cd `dirname $0`

OS="$(uname)"

bash ./simple_command.test
bash ./comment.test
bash ./file.test
bash ./v_opt.test
bash ./string.test
bash ./import.test
bash ./pipeline.test

#if [ "$OS" = Linux ] ; then
#	bash ./pipeline.test
#else
#	bash ./pipeline_mac.test
#fi

echo OK
