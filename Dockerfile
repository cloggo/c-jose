FROM node:10-alpine

LABEL maintainer="Aaron Diep <aaron@delite.ca>"

EXPOSE 8080

ENV USER='node' \
    APP_DIR='src' \
    DEPLOY_PATH='/deploy' \
    NODE_ENV='production'

WORKDIR $DEPLOY_PATH

ADD ./package.json $DEPLOY_PATH/package.json

RUN DEV_PACKAGES="make gcc g++ python git" \
    NODE_PACKAGES="node-gyp" \
    TEST_PACKAGES="mocha chai supertest" \
    && apk add --no-cache meson jansson tini $DEV_PACKAGES \
    && npm install --production --save $NODE_PACKAGES \
    && npm install --save-dev $TEST_PACKAGES

ADD ./docker-entrypoint.sh /docker-entrypoint.sh
ADD ./src $DEPLOY_PATH/$APP_DIR

RUN chmod +x /docker-entrypoint.sh \
  && chown -R $USER $DEPLOY_PATH

USER $USER

ENTRYPOINT ["/sbin/tini", "--", "/docker-entrypoint.sh"]
