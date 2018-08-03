{
    'includes': [ 'deps/common-libjose.gypi' ],
    "targets": [
        {
            "target_name": "c_jose",
            "defines": [
                'MODULE_NAME=c_jose'
            ],
            "sources": [ "./src/module.c" ],
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
            "target_name": "action_after_build",
            "type": "none",
            "dependencies": [ "c_jose" ],
            "copies": [
                {
                    "files": [ "<(PRODUCT_DIR)/c_jose.node" ],
                    "destination": "./lib/module/"
                }
            ]
        },
        {
            "target_name": "clean_up",
            "type": "none",
            "dependencies": [ "action_after_build" ],
            "actions": [
                {
                    'action_name' : 'cleanup',
                    'inputs': [''],
                    'outputs': [''],
                    'action': ['rm', '-rf', "<(PRODUCT_DIR)/obj.target"]
                }
            ]
        }
    ]
}
