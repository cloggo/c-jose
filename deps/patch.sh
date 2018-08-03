#!/bin/sh

dup_names="misc.c jwk.c"
src_dir=jose/lib/openssl

cp jose/jose/jose.h.in jose/jose/jose.h
sed -i -e 's/@VERSION@/10/g' jose/jose/jose.h

for name in $dup_names; do
    if [ -f "$src_dir/$name" ]; then
        mv -f $src_dir/$name $src_dir/_$name
    fi
done

