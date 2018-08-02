{
  'includes': [ 'deps/common-libjose.gypi' ],
  "targets": [
    {
      "target_name": "<(module_name)",
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ],
      'defines': [
        'MODULE_NAME=<(module_name)'
      ],
      "sources": [ "./src/module.c" ],
      'dependencies': [
        'deps/jose/jose.gyp:jose',
      ],
    }
  ]
}
