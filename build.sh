#!/bin/bash

CMAKE=cmake
PROFILE=Release

[ ! -z "$1" ] && PROFILE=$1

BLDDIR=.build/example/$PROFILE

$CMAKE --build $BLDDIR -- -j8
