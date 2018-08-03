#!/bin/sh


if [ $(uname -s) == "Darwin" ]; then
  echo "fixes duplicate"

    dup_names="misc.c jwk.c"
    src_dir=deps/jose/lib/openssl

    for name in $dup_names; do
        if [ -f "$src_dir/$name" ]; then
            mv $src_dir/$name $src_dir/_$name
        fi
    done
fi

rm -rf ./build
node-gyp configure -- --no-duplicate-basename-check
node-gyp build
