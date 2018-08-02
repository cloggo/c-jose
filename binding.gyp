{
  'includes': [ 'deps/common-libjose.gypi' ],
  "targets": [
    {
      "target_name": "c_jose",
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ],
      'defines': [
        'MODULE_NAME=c_jose'
      ],
      "sources": [ "./src/module.c" ],
      'dependencies': [
        'deps/libjose.gyp:jose',
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
