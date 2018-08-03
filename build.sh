#!/bin/sh

git submodule update --init

./patch.sh

rm -rf ./build
node-gyp configure -- --no-duplicate-basename-check
node-gyp build
