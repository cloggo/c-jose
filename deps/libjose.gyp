{
    'includes': [ 'common-libjose.gypi' ],
    'targets': [
        {
            'target_name': 'action_before_build',
            'type': 'none',
            'actions': [
                {
                    'action_name': 'patch jose file names',
                    'inputs': [''],
                    'outputs': [''],
                    'action': ['sh', '-c', './patch.sh']
                }
            ]
            
        },
        {
            'target_name': 'jose',
            'type': 'static_library',
            'product_prefix': 'lib',
            'include_dirs': [
                'jose',
                'jose/lib',
                'jose/lib/openssl'
            ],
            'dependencies': [
                'action_before_build'
            ],
            'sources': [
                'jose/lib/misc.c',
                'jose/lib/cfg.c',
                'jose/lib/io.c',
                'jose/lib/b64.c',
                'jose/lib/hsh.c',
                'jose/lib/hooks.c',
                'jose/lib/jwk.c',
                'jose/lib/jws.c',
                'jose/lib/jwe.c',
                'jose/lib/zlib/deflate.c',
                'jose/lib/openssl/aescbch.c',
                'jose/lib/openssl/aesgcm.c',
                'jose/lib/openssl/aesgcmkw.c',
                'jose/lib/openssl/aeskw.c',
                'jose/lib/openssl/compat.c',
                'jose/lib/openssl/dir.c',
                'jose/lib/openssl/ec.c',
                'jose/lib/openssl/ecdh.c',
                'jose/lib/openssl/ecdhes.c',
                'jose/lib/openssl/ecmr.c',
                'jose/lib/openssl/ecdsa.c',
                'jose/lib/openssl/hash.c',
                'jose/lib/openssl/hmac.c',
                'jose/lib/openssl/_jwk.c',
                'jose/lib/openssl/lock.c',
                'jose/lib/openssl/_misc.c',
                'jose/lib/openssl/oct.c',
                'jose/lib/openssl/pbes2.c',
                'jose/lib/openssl/rsa.c',
                'jose/lib/openssl/rsaes.c',
            ]
        }
    ]
}
