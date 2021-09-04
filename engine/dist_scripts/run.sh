#!/bin/sh

# Retrieve directory containing this script, solving symbolic links and relative paths
GAMEDIR=`dirname "$(readlink -f "$0")"`
cd "$GAMEDIR"

# Run game executable from its own directory to make sure assets and config copied
# in the same directory are accessible. Also set LD_LIBRARY_PATH to this directory
# so dynamic libraries are accessible.
# We only support Linux builds currently.
LD_LIBRARY_PATH=".:$LD_LIBRARY_PATH" ./Game_Linux
