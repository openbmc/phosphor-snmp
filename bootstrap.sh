#!/bin/sh

set -eu

AUTOCONF_FILES="Makefile.in aclocal.m4 ar-lib autom4te.cache compile \
    config.guess config.h.in config.sub configure depcomp install-sh \
    ltmain.sh missing *libtool test-driver"

BOOTSTRAP_MODE=""

if [ $# -gt 0 ];
then
    BOOTSTRAP_MODE="${1}"
    shift 1
fi

case ${BOOTSTRAP_MODE} in
    clean)
        test -f Makefile && make maintainer-clean
        for file in ${AUTOCONF_FILES}; do
            find . -name "$file" -print0 | xargs -0 -r rm -rf
        done
        exit 0
        ;;
esac

autoreconf -i

case ${BOOTSTRAP_MODE} in
    dev)
        # shellcheck disable=SC2086 # CONFIGURE_FLAGS is intentionally split.
        ./configure \
            ${CONFIGURE_FLAGS} \
            --enable-code-coverage \
            --enable-oe-sdk \
            "$@"
        ;;
    *)
        # shellcheck disable=SC2016
        echo 'Run "./configure ${CONFIGURE_FLAGS} && make"'
        ;;
esac

