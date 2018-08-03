IMAGE_PATH := docker/lib/node/c-jose
DOCKER_REGISTRY := registry.delite.ca
IMAGE := $(DOCKER_REGISTRY)/$(IMAGE_PATH)

DOCKER_RUN := docker run -it \
  -e "NODE_ENV=test" \
	-v "$(CURDIR)/tmp/node-gyp:/home/node/.node-gyp" \
	-v "$(CURDIR)/src:/deploy/src" \
	-v "$(CURDIR)/deps:/deploy/deps" \
	-v "$(CURDIR)/test:/deploy/test" \
	-v "$(CURDIR)/tmp/build:/deploy/build" \
	-v "$(CURDIR)/binding.gyp:/deploy/binding.gyp" \
	$(IMAGE)

MOCHA_BIN := node_modules/.bin/mocha
NODEMON_BIN := node_modules/.bin/nodemon
NODEGYP_BIN := node_modules/.bin/node-gyp
NODE_PRE_GYP_BIN := node_modules/.bin/node-pre-gyp

WATCH_EXTS := js,json

.PHONY: test watch pull build debug config clean

pull:
	docker pull $(IMAGE)

config: pull
	$(DOCKER_RUN) $(NODEGYP_BIN) configure -- --no-duplicate-basename-check

debug: config
	$(DOCKER_RUN) $(NODEGYP_BIN) build --debug

build: config
	$(DOCKER_RUN) $(NODEGYP_BIN) build

watch: pull
	$(DOCKER_RUN) $(NODEMON_BIN) -e $(WATCH_EXTS) --watch app --watch test \
	$(MOCHA_BIN) --watch --recursive \
	--watch-extensions $(WATCH_EXTS) "test/**/test-*.js"

test: pull
	$(DOCKER_RUN) $(MOCHA_BIN) --timeout 10000 "test/**/test-*.js"

clean:
	@RM -rf tmp/*
