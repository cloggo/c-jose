var Jose = require('bindings')('c_jose');

Jose.jose_json_type = Object.freeze(Jose.jose_json_type);

module.exports = Jose;

