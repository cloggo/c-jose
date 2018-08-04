"use strict";

//During the test the env variable is set to test
process.env.NODE_ENV = 'test';

var assert = require('assert');
var Jose = require('..');

describe('Array', function() {
    describe('#indexOf()', function() {
        it('should return -1 when the value is not present', function() {
            assert.equal([1,2,3].indexOf(4), -1);
        });
    });
});
