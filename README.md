# c-jose

[![npm](https://img.shields.io/npm/v/c-jose.svg)](https://www.npmjs.com/package/c-jose)

Node.JS javascript binding for [JOSE](https://github.com/latchset/jose.git) using N-API

## Installation 

### prerequisites
* jansson
* openssl-dev
* zlib-dev

```
npm install
```

darwin (from source):

```bash
## assumed dependencies were installed with darwin ports 
CFLAGS=-I/opt/local/include LDFLAGS=-L/opt/local/lib npm install -g
```

