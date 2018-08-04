FROM registry.delite.ca/docker/base/dev/node:latest

RUN APP_PACKAGES="jansson-dev libcrypto1.0 zlib-dev" \
    && apk add --no-cache $APP_PACKAGES \
    && ln -sf /usr/lib/libcrypto.so.1.0.0 /usr/lib/libcrypto.so

USER $USER
