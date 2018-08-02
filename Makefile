IMAGE_PATH := docker/lib/node/c-jose
DOCKER_REGISTRY := registry.delite.ca
IMAGE := $(DOCKER_REGISTRY)/$(IMAGE_PATH)

DOCKER_RUN := docker run --user root -it \
  -e "NODE_ENV=test" \
	-v "$(CURDIR)/src:/deploy/src" \
	-v "$(CURDIR)/deps:/deploy/deps" \
	-v "$(CURDIR)/test:/deploy/test" \
	$(IMAGE)

MOCHA_BIN := node_modules/.bin/mocha
NODEMON_BIN := node_modules/.bin/nodemon
NODEGYP_BIN := node_modules/.bin/node-pre-gyp

WATCH_EXTS := js,json

.PHONY: test pull

pull:
	docker pull $(IMAGE)

debug: pull
	$(DOCKER_RUN) $(NODEGYP_BIN) rebuild --debug

watch: pull
	$(DOCKER_RUN) $(NODEMON_BIN) -e $(WATCH_EXTS) --watch app --watch test \
	$(MOCHA_BIN) --watch --recursive \
	--watch-extensions $(WATCH_EXTS) "test/**/test-*.js"

test: pull
	$(DOCKER_RUN) $(MOCHA_BIN) --timeout 10000 "test/**/test-*.js"
