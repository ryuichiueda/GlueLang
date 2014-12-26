#!/bin/sh -e

cd `dirname $0`

bash ./simple_command.test
bash ./comment.test
bash ./file.test
bash ./v_opt.test
bash ./pipeline.test
