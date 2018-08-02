IMAGE_PATH := docker/lib/node/c-jose
DOCKER_REGISTRY := registry.delite.ca
IMAGE := $(DOCKER_REGISTRY)/$(IMAGE_PATH)

DOCKER_RUN := docker run --user root -it \
  -e "NODE_ENV=test" \
	-v "$(CURDIR)/node_modules:/deploy/node_modules" \
	-v "$(CURDIR)/src:/deploy/src" \
	-v "$(CURDIR)/test:/deploy/test" \
	$(IMAGE)

MOCHA_BIN := node_modules/.bin/mocha
NODEMON_BIN := node_modules/.bin/nodemon

WATCH_EXTS := js,json

.PHONY: test pull

pull:
	docker pull $(IMAGE)

watch: pull
	$(DOCKER_RUN) $(NODEMON_BIN) -e $(WATCH_EXTS) --watch app --watch test \
	$(MOCHA_BIN) --watch --recursive \
	--watch-extensions $(WATCH_EXTS) "test/**/test-*.js"

test: pull
	$(DOCKER_RUN) $(MOCHA_BIN) --timeout 10000 "test/**/test-*.js"
