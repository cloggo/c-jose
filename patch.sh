#!/bin/sh

dup_names="misc.c jwk.c"
src_dir=deps/jose/lib/openssl

for name in $dup_names; do
    if [ -f "$src_dir/$name" ]; then
        mv $src_dir/$name $src_dir/_$name
    fi
done

