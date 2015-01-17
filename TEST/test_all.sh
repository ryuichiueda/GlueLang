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
bash ./if.test
bash ./stdcom.test
bash ./argv.test
bash ./proc.test
bash ./blank.test
bash ./andline.test
bash ./where.test

echo OK
