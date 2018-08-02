#!/bin/sh
pip install nodeenv
nodeenv -p -n 10.8.0
npm install -g node-gyp
node-gyp configure
