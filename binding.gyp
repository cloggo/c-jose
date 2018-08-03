{
    'includes': [ 'deps/common-libjose.gypi' ],
    "targets": [
        {
            'target_name': 'action_before_build',
            'type': 'none',
            'actions': [
                {
                    'action_name': 'update submodules',
                    'inputs': [''],
                    'outputs': [''],
                    'action': ['git', 'submodule', 'update', '--init']
                    
                }
            ]
            
        },
        {
            "target_name": "c_jose",
            "defines": [
                'MODULE_NAME=c_jose'
            ],
            "sources": [ "./src/module.c" ],
            'dependencies': [
                'action_before_build',
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
