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
            'libraries': [
                '-lpthread',
                '-lz',
                '-ljansson',
                '-lcrypto',
            ],
        }
    ]
}
