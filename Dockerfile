FROM registry.delite.ca/docker/base/node/dev:latest

ENV APP_DIR='src'

ADD ./package.json $DEPLOY_PATH/package.json

RUN APP_PACKAGES="jansson-dev libcrypto1.0 zlib-dev" \
    && apk add --no-cache $APP_PACKAGES \
    && npm install --production --save $NODE_PACKAGES \
    && npm install --save-dev $TEST_PACKAGES \
    && ln -sf /usr/lib/libcrypto.so.1.0.0 /usr/lib/libcrypto.so

# USER $USER
