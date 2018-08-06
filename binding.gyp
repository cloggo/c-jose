{
    'includes': [ 'deps/common-libjose.gypi' ],
    "targets": [
        {
            "target_name": "c_jose",
            "defines": [
                'MODULE_NAME=c_jose'
            ],
            "sources": [
                "./src/module.c",
                "./src/json.c",
                "./src/b64.c",
                "./src/jwk.c"
            ],
            'dependencies': [
                'deps/libjose.gyp:jose'
            ],
            'include_dirs': [
                'deps/jose',
            ],
            'libraries': [
                '-lpthread',
                '-lz',
                '-ljansson',
                '-lcrypto',
            ],
        },
        {
            "target_name": "clean_up",
            "type": "none",
            "dependencies": [ "c_jose" ],
            "actions": [
                {
                    'action_name' : 'cleanup',
                    'inputs': [''],
                    'outputs': [''],
                    'action': ['sh', '-c', "./cleanup.sh <(CONFIGURATION_NAME) <(PRODUCT_DIR)"]
                }
            ]
        }
    ]
}
