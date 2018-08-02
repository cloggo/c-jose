{
  'targets': [
    {
      'target_name': 'jose',
      'type': 'static_library',
      'libraries': [
        '-lpthread',
        '-lz',
        '-ljansson',
        '-lcrypto',
      ],
      'include_dirs': [
        'jose',
        'jose/openssl'
      ],
      'sources': [
        'jose/misc.c',
        'jose/cfg.c',
        'jose/io.c',
        'jose/b64.c',
        'jose/hsh.c',
        'jose/hooks.c',
        'jose/jwk.c',
        'jose/jws.c',
        'jose/jwe.c',
        'jose/zlib/deflate.c',
        'jose/openssl/aescbch.c',
        'jose/openssl/aesgcm.c',
        'jose/openssl/aesgcmkw.c',
        'jose/openssl/aeskw.c',
        'jose/openssl/compat.c',
        'jose/openssl/dir.c',
        'jose/openssl/ec.c',
        'jose/openssl/ecdh.c',
        'jose/openssl/ecdhes.c',
        'jose/openssl/ecmr.c',
        'jose/openssl/ecdsa.c',
        'jose/openssl/hash.c',
        'jose/openssl/hmac.c',
        'jose/openssl/jwk.c',
        'jose/openssl/lock.c',
        'jose/openssl/misc.c',
        'jose/openssl/oct.c',
        'jose/openssl/pbes2.c',
        'jose/openssl/rsa.c',
        'jose/openssl/rsaes.c',
      ]
    }
  ]
}
