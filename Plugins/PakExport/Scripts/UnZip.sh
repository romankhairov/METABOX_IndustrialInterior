#!/bin/bash

SourceName=$1
DestDir=$2

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
UtilsDir="$SCRIPTPATH/../Utils"

"$UtilsDir/7-Zip-Mac/7zz" x -tzip "$SourceName.zip" -o"$DestDir"
