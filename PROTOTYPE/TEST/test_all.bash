#!/bin/bash -e

dir=$(dirname $0)

$dir/filters.bash
$dir/countgrepfile.bash
