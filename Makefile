IMAGE_PATH := docker/lib/node/c-jose
DOCKER_REGISTRY := registry.delite.ca
IMAGE := $(DOCKER_REGISTRY)/$(IMAGE_PATH)

DOCKER_RUN := docker run --user node -it \
  -e "NODE_ENV=test" \
	-v "$(CURDIR)/tmp/node-gyp:/home/node/.node-gyp" \
	-v "$(CURDIR):/deploy" \
	$(IMAGE)

NODE_MODULE_PATH := node_modules/.bin
MOCHA_BIN := $(NODE_MODULE_PATH)/mocha
NODEMON_BIN := $(NODE_MODULE_PATH)/nodemon
NODEGYP_BIN := $(NODE_MODULE_PATH)/node-gyp
NODE_PRE_GYP_BIN := $(NODE_MODULE_PATH)/node-pre-gyp

WATCH_EXTS := js,json

.PHONY: test watch pull build debug config install clean

pull:
	docker pull $(IMAGE)

config: binding.gyp deps/libjose.gyp
	$(DOCKER_RUN) $(NODEGYP_BIN) configure -- --no-duplicate-basename-check 
	touch tmp/config.done

debug: pull tmp/config.done
	$(DOCKER_RUN) $(NODEGYP_BIN) build --debug

build: pull tmp/config.done
	$(DOCKER_RUN) $(NODEGYP_BIN) build

install:
	$(DOCKER_RUN) npm install

watch: pull
	$(DOCKER_RUN) $(NODEMON_BIN) -e $(WATCH_EXTS) --watch app --watch test \
	$(MOCHA_BIN) --watch --recursive \
	--watch-extensions $(WATCH_EXTS) "test/**/*.test.js"

test: debug
	$(DOCKER_RUN) $(MOCHA_BIN) --timeout 10000 "test/**/*.test.js"

clean:
	@RM -rf tmp
	@RM -rf node_modules
	@RM -rf build
