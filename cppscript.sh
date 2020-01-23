#!/bin/bash
real_src=$(realpath "$1")
mangled=${real_src/\//}
mangled=${mangled//\//__}

mkdir -p "$HOME/.cppscript_cache"

checksum=$(md5sum "$1")
exist_checksum=$(cat "$HOME/.cppscript_cache/checksum.$mangled" 2>/dev/null)

bin_name="$HOME/.cppscript_cache/$mangled"

if [ "$checksum" != "$exist_checksum" ]; then
    tail -n +2 "$1" \
        | g++ \
            -Wall \
            -Wextra \
            -Werror \
            -O2 \
            -std=c++2a \
            -o "$bin_name" \
            -x c++ - \
            -lfmt
    if [ "$?" != "0" ]; then
        exit $?
    fi
    echo -n "$checksum" >"$HOME/.cppscript_cache/checksum.$mangled"
fi

exec $bin_name "${@:2}"
