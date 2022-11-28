#!/bin/bash

CMAKE=cmake
PROFILE=Release

[ ! -z "$1" ] && PROFILE=$1

BLDDIR=.build/example/$PROFILE
[ ! -d "$BLDDIR" ] && mkdir -p $BLDDIR

cd $BLDDIR
$CMAKE -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=$PROFILE $OLDPWD
cd $OLDPWD
