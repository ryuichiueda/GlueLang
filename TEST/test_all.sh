#!/bin/sh -e

cd `dirname $0`

bash ./simple_command.test
bash ./invalid_command_name.test
