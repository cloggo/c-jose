{
  'includes': [ 'deps/common-libjose.gypi' ],
  "targets": [
    {
      "target_name": "c-jose",
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ],
      'defines': [
        'MODULE_NAME=c-jose'
      ],
      "sources": [ "./src/module.c" ],
      'dependencies': [
        'deps/jose.gyp:jose',
      ],
    }
  ]
}
