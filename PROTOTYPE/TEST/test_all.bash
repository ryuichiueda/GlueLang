#!/bin/bash -e

dir=$(dirname $0)

$dir/filter.bash
$dir/io.bash
$dir/str.bash
$dir/if.bash
$dir/heredoc.bash
$dir/while.bash
