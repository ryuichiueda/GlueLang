#!/bin/bash -exv

dir=$(dirname $0)

$dir/filter.bash
$dir/io.bash
$dir/str.bash
$dir/if.bash
$dir/heredoc.bash
