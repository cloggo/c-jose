#!/bin/sh

rm -rf ./build
node-gyp configure -- --no-duplicate-basename-check
node-gyp build
