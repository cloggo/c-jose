#!/bin/sh

if [ "$NODE_ENV" == "test" ] && [ ! -f "node_modules/.bin/mocha" ]; then
    DEV_PACKAGES="make gcc g++ python git"
    apk add --no-cache $DEV_PACKAGES
    npm install
    apk del $DEV_PACKAGES
fi

exec "$@"
