#!/bin/sh
NODEGYP_BIN=node_modules/.bin/node-gyp
NODE_PRE_GYP_BIN=node_modules/.bin/node-pre-gyp

$NODEGYP_BIN configure -- --no-duplicate-basename-check
$NODE_PRE_GYP_BIN build --debug


