#!/bin/sh -exv

cd `dirname $0`

OS="$(uname)"

bash ./simple_command.test
bash ./variable.test
bash ./comment.test
bash ./file.test
bash ./v_opt.test
bash ./varstring.test
bash ./import.test
bash ./pipeline.test
bash ./if.test
bash ./argv.test
bash ./proc.test
bash ./blank.test
bash ./job.test
bash ./where.test
bash ./signal.test
bash ./internal_commands.test
bash ./background_proc.test
bash ./while.test
bash ./foreach.test

echo OK
