#!/bin/sh

if [ "$1" == "Release" ]; then
    # rm -f "$2/*.a" "$2/*.dylib" "$2/*.so"
    rm -rf "$2/obj.target"
fi
