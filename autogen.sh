#!/bin/sh

if [[ "$OSTYPE" == "darwin"* ]]; then
	echo "OS X detected. Generating Xcode project generator instead"
	clang++ -std=c++11 Setup/Xcode/configure.cpp -o configure
	exit
fi

echo "Generating ./configure script, this may take a while..."

# this is specificly for mandrake 8.2
WANT_AUTOCONF_2_5=1
export WANT_AUTOCONF_2_5

AUTOMAKE_VERSION=`automake --version | head -n 1`

case $AUTOMAKE_VERSION in
    *1.5b* | *1.6* | *1.7* | *1.8* | *1.9* | *1.10* | *1.11* | *1.12* | *1.13* | *1.14* | *1.15* | *1.16* | *1.17* )
	echo "Running aclocal, automake and autoconf..."
        AUTOMAKE="automake"
        ACLOCAL="aclocal"
	;;
    *)
        # Debian specific checks for other automake binaries
        if [ `which automake-1.7` ]; then
            AUTOMAKE="automake-1.7"
            ACLOCAL="aclocal-1.7"
        elif [ `which automake-1.6` ]; then
            AUTOMAKE="automake-1.6"
            ACLOCAL="aclocal-1.6"
        else
            echo "Your version of automake is:"
            echo ""
            echo "   $AUTOMAKE_VERSION"
            echo ""
            echo "But automake >= 1.5b is required, you can find it at:"
            echo ""
            echo " - http://sources.redhat.com/automake/"
            echo ""
            exit 0
        fi
	;;
esac

$ACLOCAL -I Setup/Unix/m4

libtoolize --force --copy

$AUTOMAKE --add-missing --copy --force

# autoconf >= 2.50 is required here, earlier versions might fail at
# detecting some libraries
autoconf

# EOF #
