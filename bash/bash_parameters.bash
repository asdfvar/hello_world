#!/bin/bash

echo "# = $#"
echo "@ = $@"
echo "0 = $0"
echo "PWD = $PWD"
echo "BASH_SOURCE = $BASH_SOURCE"
echo "{BASH_SOURCE} = ${BASH_SOURCE}"
echo "SCRIPT_PATH = $SCRIPT_PATH"
echo "This script's directory location: $(dirname $(readlink -f $BASH_SOURCE))"
