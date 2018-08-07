"use strict";

//During the test the env variable is set to test
process.env.NODE_ENV = 'test';

// let assert = require('assert');
let fs = require('fs');
let path = require('path');
let JOSE = require('..');
let chai = require('chai');

// const kid = "6nLskcj2dYVXd7JUjFouB3Ne7So";
const _fp = path.join(__dirname, './vector/test.luks.b64');
let tv = fs.readFileSync(_fp);
tv = tv.toString();
let hdr_b64 = tv.match(/^(.*?)\./);
hdr_b64 = hdr_b64[1];

describe('JWK', function() {
    chai.should();
    let expect = chai.expect;

    // let hdr = Buffer.from(hdr_b64, 'base64').toString('ascii');
    let hdrRaw = JOSE.jose_b64_dec_buf(hdr_b64);
    // console.log(hdr);
    let hdr = JSON.parse(hdrRaw);
    let url = hdr.clevis.tang.url;
    describe('base64 decode', function() {
        it('clevis.pin should equal to "tang"', function() {
            hdr.clevis.pin.should.equal("tang");
        });
    });

    describe('base64 encode', function() {
        it('encoded string should recover b64', function() {
            JOSE.jose_b64_enc_sbuf(hdrRaw).should.equal(hdr_b64);
        });
    });

    const clt = hdr.epk;

    describe('epk exists', function() {
        it('hdr.epk should not equal to undefined', function() {
            clt.should.not.be.undefined;
        });
    });

    const kid = hdr.kid;
    describe('kid exists', function() {
        it('hdr.kid should not equal to undefined', function() {
            kid.should.not.be.undefined;
        });
    });

    const keys = hdr.clevis.tang.adv.keys;

    describe('keys array', function() {
        it('must be an array', function() {
            Array.isArray(keys).should.be.true;
        });
    });

    const keysJSONArray = JSON.stringify(keys);
    const decodedKeys = JOSE.jose_json_loads(keysJSONArray);
    const encodedKeys = JOSE.jose_json_dumps(decodedKeys);

    describe('jansson json_dumps ', function() {
        it('should recover the stringify json', function() {
            JSON.parse(encodedKeys).should.deep.equal(keys);
        });
    });

    describe('json array', function() {
        it('foreach element operation', function() {
            let arr = [];
            JOSE.jose_json_foreach(decodedKeys, function(index, value) {
                arr.push(JOSE.jose_json_dumps(value));
            });

            arr = ['[', arr.join(","), ']'].join("");
            JSON.parse(arr).should.deep.equal(keys);
        });
    });

    const dlen = JOSE.jose_jwk_thp_buf(null, "S1", null, 0);

    describe('buffer size', function() {
         it('calculate size', function() {
            dlen.should.equal(20);
        });
    });


    let srv;

    describe('calculate thumbprint', function() {
        it('foreach element calculate thumbprint', function() {
            let buf = Buffer.allocUnsafe(dlen);
            let arr = [];

            JOSE.jose_json_foreach(decodedKeys, function(index, value) {
                JOSE.jose_jwk_thp_buf(value, "S1", buf);
                let tmp = JOSE.jose_b64_enc_bbuf(buf);

                if(tmp === kid) {
                    srv = value;
                }

                arr.push(tmp);
            });

            // console.log("value:", JOSE.jose_json_dumps(srv));
            arr.indexOf(kid).should.not.equal(-1);
        });
    });

    describe('json type enumeration', function() {
        it("JSON type", function() {
            JOSE.jose_json_type.JSON_OBJECT.should.equal(0);
            JOSE.jose_json_type.JSON_ARRAY.should.equal(1);
            JOSE.jose_json_type.JSON_STRING.should.equal(2);
        });
    });

    describe('json type of', function() {
        it("array type", function() {
            JOSE.jose_json_typeof(decodedKeys).should.equal(JOSE.jose_json_type.JSON_ARRAY);
        });
    });

    describe('json get', function() {
        let hdr_cjson = JOSE.jose_json_loads(hdrRaw);
        it("get kid", function() {
            // console.log(JOSE.jose_json_dumps(hdr_cjson));
            let kid_json = JOSE.jose_json_get(hdr_cjson, "kid");
            JOSE.jose_json_typeof(kid_json).should.equal(JOSE.jose_json_type.JSON_STRING);
            JOSE.jose_json_value_get(kid_json).should.equal(kid);
            // JOSE.jose_json_dumps(kid_json).should.equal(kid);
        });
    });

});
