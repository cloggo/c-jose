#!/bin/sh
ln -sf /usr/lib/libcrypto.so.1.0.0 /usr/lib/libcrypto.so

BUILD_MODE=--release

if [ "$NODE_ENV" != "production" ]; then
    BUILD_MODE=--debug
fi

NODEGYP_BIN=node_modules/.bin/node-gyp
NODE_PRE_GYP_BIN=node_modules/.bin/node-pre-gyp

$NODEGYP_BIN configure -- --no-duplicate-basename-check
# $NODE_PRE_GYP_BIN build $BUILD_MODE
$NODEGYP_BIN build $BUILD_MODE
