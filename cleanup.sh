#!/bin/sh

if [ "$1" == "Release" ]; then
    rm -f "$2/*.a"
    rm -rf "$2/obj.target"
fi
