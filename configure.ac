dnl
dnl Process this file with autoconf to produce a configure script.
dnl

dnl Ensure that a recent enough version of Autoconv is being used
AC_PREREQ(2.50)

dnl Set the name of the package and its version. (This must be called first)
AC_INIT(ClanLib, 2.4.0)

AC_CONFIG_MACRO_DIR(m4)

dnl Ensure that the source code exists (by specifying a known source code file)
AC_CONFIG_SRCDIR(Sources/API/core.h)

dnl Compute the canonical target-system type variable (target_cpu, target_vendor, target_os)
AC_CANONICAL_TARGET

dnl Space-separated list of Automake options to be applied to every Makefile.am in the tree
dnl foreign -> Make automake less strict about GNU policy (as ClanLib is missing ChangeLog)
dnl dist-bzip2 -> Hook dist-bzip2 to dist
dnl dist-zip -> Hook dist-zip to dist
dnl 1.5b -> Minimum Automake version
dnl subdir-objects -> Objects places into the subdirectory of the build directory (eg subdir/file.c would be subdir/file.o)
AM_INIT_AUTOMAKE( foreign dist-bzip2 dist-zip 1.5b subdir-objects )

CLANLIB_MAJOR_VERSION=2
CLANLIB_MINOR_VERSION=4
CLANLIB_MICRO_VERSION=0

CLANLIB_VERSION=$CLANLIB_MAJOR_VERSION.$CLANLIB_MINOR_VERSION.$CLANLIB_MICRO_VERSION
CLANLIB_RELEASE=$CLANLIB_MAJOR_VERSION$CLANLIB_MINOR_VERSION

AC_SUBST(LDFLAGS_LT_RELEASE)
AC_SUBST(CLANLIB_MAJOR_VERSION)
AC_SUBST(CLANLIB_MINOR_VERSION)
AC_SUBST(CLANLIB_MICRO_VERSION)
AC_SUBST(CLANLIB_VERSION)
AC_SUBST(CLANLIB_RELEASE)

dnl Set this to: MAJOR.MINOR, ie 0.7, 0.8, 0.9 it gets append as:
dnl  libclanDisplay-$LT_RELEASE

LT_RELEASE=$CLANLIB_MAJOR_VERSION.$CLANLIB_MINOR_VERSION
if test -z "$LDFLAGS_LT_RELEASE"; then
	LDFLAGS_LT_RELEASE="-release $LT_RELEASE"
fi

dnl If any features got added to the API since last release incremeant
dnl this by 1 (ie increment this for each develpment release, and only
dnl if features have been added to a stable release)
LT_CURRENT=1

dnl If any interfaces have been added since the last public release,
dnl then increment AGE. If any interfaces have been removed since the
dnl last public release, then set AGE to 0. So when backward compability
dnl is broken, set this to 0, else stay in sinc with LT_CURRENT
dnl (development releases should stay 0, stable be in sync with
dnl LT_CURRENT)
LT_AGE=0

dnl If LT_CURRENT has changed, set this to 0, if LT_CURRENT stayed the
dnl same increment by 1 (so stable releases should increment, while
dnl development releases stay at 0)
LT_REVISION=0

dnl libclanFoobar-$LT_RELEASE.so.[$LT_CURRENT-$LT_AGE].$LT_AGE.$LT_REVISION

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

dnl Checks for C++ compiler
AC_PROG_CXX

dnl Check for a C compiler 
AM_PROG_CC_C_O
dnl AC_PROC_CC

dnl Check for a C preprocessor
AC_PROG_CPP

dnl Check for a BSD-compatible install program
AC_PROG_INSTALL

dnl Check for libtool (enabled shared libraries)
AC_PROG_LIBTOOL

dnl Using C++
AC_LANG(C++)
dnl AC_LANG(C)

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_STDBOOL

AC_PROG_GCC_TRADITIONAL

dnl Find absolute name to pkg-config
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

dnl -----------------------------------------------------------------------
dnl TODO: Is this the correct way to enable c++0x?
dnl -----------------------------------------------------------------------
CXXFLAGS="$CXXFLAGS -std=c++0x"

case $target in
	*cygwin* )
		X11="no"
		WIN32="yes"
		CXXFLAGS="$CXXFLAGS -mno-cygwin -DWIN32"
		;;
	*mingw*)
		X11="no"
		WIN32="yes"
		CXXFLAGS="$CXXFLAGS -DWIN32 -DUNICODE"
		;;
	*)
		X11="yes"
		WIN32="no"
		;;
esac

dnl Define CL_ARM_PLATFORM if building for an ARM target
case $target in
	arm* )
		CXXFLAGS="$CXXFLAGS -DCL_ARM_PLATFORM"
		CFLAGS="$CXXFLAGS -DCL_ARM_PLATFORM"
		;;
esac

dnl Extra flags that are needed to compile (for pkg-config)
if test "$WIN32" = "yes"; then
	case $target in
	*mingw*)
		extra_CFLAGS_clanApp="-mwindows"
		extra_LIBS_clanApp="-gdi32 -luser32 -lkernel32 -lmsvcrt"
		extra_LIBS_clanCore="-lz -lmsvcrt -lshell32 -lshlwapi -limagehlp -lmingwex"
		extra_LIBS_clanDisplay="-lwinmm -lmsvcrt -lmingwex -lddraw -ldinput8 -ldxguid -lmapi32 -lgdi32 -lole32 -ljpeg -lpng -lfreetype"
		extra_LIBS_clanGL="-lopengl32"
		extra_LIBS_clanGL1="-lopengl32"
		extra_LIBS_clanSound="-ldsound -ldxguid -lwinmm -lmsvcrt -lmingwex"
		extra_LIBS_clanSqlite="-lsqlite3"
		extra_LIBS_clanVorbis="-lvorbis -logg"
		extra_LIBS_clanNetwork="-lws2_32"
		;;
	*)
	extra_CFLAGS_clanApp="-mwindows"
	extra_LIBS_clanCore="-lz"
	extra_LIBS_clanDisplay="-lwinmm -lddraw -ldinput8 -ldxguid -lgdi32 -lole32 -ljpeg -lpng"
	extra_LIBS_clanGL="-lopengl32"
	extra_LIBS_clanGL1="-lopengl32"
	extra_LIBS_clanSound="-ldsound -ldxguid -lwinmm"
	extra_LIBS_clanVorbis="-lvorbis -logg"
	extra_LIBS_clanNetwork="-lws2_32"
	extra_LIBS_clanSqlite="-lsqlite3"
	;;
esac
fi

dnl ----------------------------------------
dnl Additional include and library paths:
dnl ----------------------------------------

AC_ARG_WITH([extra-includes], AS_HELP_STRING([--with-extra-includes=DIR],[Add extra include paths]),
	use_extra_includes="$withval",
	use_extra_includes=NO
)
if test -n "$use_extra_includes" && test "$use_extra_includes" != "NO"; then
	ac_save_ifs=$IFS
	IFS=':'
	for dir in $use_extra_includes; do
		extra_includes="$extra_includes -I$dir"
	done
	IFS=$ac_save_ifs
fi

CXXFLAGS="$CXXFLAGS $extra_includes"

AC_ARG_WITH(extra-libs, AS_HELP_STRING([--with-extra-libs=DIR],[Add extra library paths]), 
	use_extra_libs=$withval,
	use_extra_libs=NO
)
if test -n "$use_extra_libs" && test "$use_extra_libs" != "NO"; then
	ac_save_ifs=$IFS
	IFS=':'
	for dir in $use_extra_libs; do
		extra_libraries="$extra_libraries -L$dir"
	done
	IFS=$ac_save_ifs
fi

AC_CHECK_HEADERS(unistd.h fcntl.h sys/kd.h sys/vt.h sys/sysctl.h)
AC_EGREP_HEADER(fcvt, stdlib.h,,AC_DEFINE(NEED_FCVT))

dnl -----------------------------------------------------
dnl Check commandline parameters to the configure script:
dnl -----------------------------------------------------

AC_CHECK_HEADER(libgen.h)
AC_CHECK_HEADER(sys/types.h)
AC_CHECK_HEADER(fstab.h, AC_DEFINE([HAVE_FSTAB_H]))

AC_MSG_CHECKING([for extern *__progname])
AC_TRY_COMPILE([],
[extern const char *__progname;],
AC_MSG_RESULT(yes);AC_DEFINE(EXTERN___PROGNAME),
AC_MSG_RESULT(no))

dnl Check for GNU extensions
AC_CHECK_FUNCS(wcscasecmp)


dnl -----------------------------------------------------
dnl Check for optional SSE2 support:
dnl -----------------------------------------------------
AC_MSG_CHECKING(for SSE2 support)
use_sse2=no
AC_ARG_ENABLE(sse2, AC_HELP_STRING([--enable-sse2], [Enable SSE2 support]),
[
	if test "$enable_sse2" != "no"; then
		CLANLIB_CHECK_CPP([`cat $srcdir/Setup/Tests/sse2.cpp`], [ ], [ -msse2 ] )
		if test "$CL_RESULT" != "yes"; then
			 AC_MSG_ERROR([ *** Compliler does not support SSE2, run configure with the --disable-sse2 option ])
		fi
		AC_MSG_RESULT([enabled])
		use_sse2=yes
	else
		AC_DEFINE(CL_DISABLE_SSE2)
		AC_MSG_RESULT([disabled])
	fi

],
[
	CLANLIB_CHECK_CPP([`cat $srcdir/Setup/Tests/sse2.cpp`], [ ], [ -msse2 ] )
	if test "$CL_RESULT" != "no"; then
		AC_MSG_RESULT([enabled])
		use_sse2=yes
	else
		AC_MSG_WARN( [ *** Compliler does not support SSE2 ])
		AC_DEFINE(CL_DISABLE_SSE2)
		AC_MSG_RESULT([disabled])
	fi


  ])


if test "$use_sse2" != "no"; then
	CXXFLAGS="$CXXFLAGS -msse2"

	extra_CFLAGS_clanCore="$extra_CFLAGS_clanCore -msse2"
else
	extra_CFLAGS_clanCore="$extra_CFLAGS_clanCore -DCL_DISABLE_SSE2"
fi
extra_CFLAGS_clanCore="$extra_CFLAGS_clanCore -pthread -std=c++0x"

dnl -----------------------------------------------------------------------
dnl Check system endianess
dnl -----------------------------------------------------------------------

if test "$ac_cv_sizeof_int" = "8"; then
	AC_DEFINE(USE_64BIT)
fi

if test "$ac_cv_c_bigendian" = "yes"; then
	AC_DEFINE(USE_BIG_ENDIAN)
fi

dnl Set makefile condition when WIN32 should be compiled
AM_CONDITIONAL([WIN32], [test $WIN32 = "yes"])

dnl ----------------------------------------
dnl Search for libraries useful for ClanLib:
dnl ----------------------------------------

if test "$WIN32" = "no"; then
	dnl Check for prefered pthread flag
	has_pthreads=no;
	save_CFLAGS=$CFLAGS
	for _thread in "pthread" "pthreads" "lpthread"; do
		AC_MSG_CHECKING([pthread_create in -${_thread}])
		CFLAGS="$save_CFLAGS -${_thread}"
		AC_TRY_COMPILE(
			[#include <pthread.h>],
			[pthread_create(0,NULL,0,NULL);],
			AC_MSG_RESULT(yes);has_pthreads=yes && break, AC_MSG_RESULT(no);)
	done

	if test x$has_pthreads = "xyes"; then
		LIBPTHREAD="-${_thread}"
	else
		AC_MSG_ERROR([" *** ClanLib requires pthread to run."])
	fi

	AC_SUBST(LIBPTHREAD)
	CFLAGS=$save_CFLAGS

	dnl Check for thread-local storage support
	GCC_CHECK_TLS
fi

AC_CHECK_LIB(z, main, , AC_MSG_ERROR([" *** ClanLib requires zlib (try zlib1g-dev or better)."]))

dnl ----------------------------------------------------------
dnl Check for ClanLib modules that should be enabled/disabled
dnl ----------------------------------------------------------

echo ""
echo "Checking for ClanLib Modules to build..."
echo "========================================"

CLANLIB_ARG_ENABLE(docs,         auto, [Build Clanlib API documentation], [whether we should try to build API documentation])
CLANLIB_ARG_ENABLE(clanDisplay,  auto, [Build clanDisplay module],    [whether we should try to build clanDisplay])
CLANLIB_ARG_ENABLE(clanGL,       auto, [Build clanGL module],         [whether we should try to build clanGL])
CLANLIB_ARG_ENABLE(clanGL1,      auto, [Build clanGL1 module],        [whether we should try to build clanGL1])
CLANLIB_ARG_ENABLE(clanSound,    auto, [Build clanSound module],      [whether we should try to build clanSound])
CLANLIB_ARG_ENABLE(clanDatabase, auto, [Build clanDatabase module],   [whether we should try to build clanDatabase])
CLANLIB_ARG_ENABLE(clanSqlite,   auto, [Build clanSqlite module],     [whether we should try to build clanSqlite])
CLANLIB_ARG_ENABLE(clanNetwork,  auto, [Build clanNetwork module],    [whether we should try to build clanNetwork])
CLANLIB_ARG_ENABLE(clanGUI,      auto, [Build clanGUI module],        [whether we should try to build clanGUI])
CLANLIB_ARG_ENABLE(clanCSSLayout,      auto, [Build clanCSSLayout module],        [whether we should try to build clanCSSLayout])
CLANLIB_ARG_ENABLE(clanVorbis,   auto, [Build clanVorbis module],     [whether we should try to build clanVorbis])

linux_joystick=no
linux_input=no
echo ""
if test "$enable_clanDisplay" != "no"; then
	echo "Checking for clanDisplay stuff"
	echo "=============================="
	CLANLIB_CHECK_LIB(png,  [`cat $srcdir/Setup/Tests/png.cpp`],  clanDisplay, [ *** Cannot find png (See http://www.libpng.org ) (Try libpng12-dev or better) ], [-lz -lpng])
	CLANLIB_CHECK_LIB(jpeg, [`cat $srcdir/Setup/Tests/jpeg.cpp`], clanDisplay, [ *** Cannot find jpeg (See http://www.ijg.org ) (Try libjpeg62-dev or better) ], [ -ljpeg])
	CLANLIB_CHECK_LIB(ttf, [`cat $srcdir/Setup/Tests/ttf.cpp`], clanDisplay, [ *** Cannot find ttf (freetype) (See http://freetype.sourceforge.net )  (Try libfreetype6-dev or better) ], [`freetype-config --libs` ], [ `freetype-config --cflags`])

	dnl  Optional linux/joystick.h
	AC_CHECK_HEADERS(linux/joystick.h, linux_joystick=yes)
	AM_CONDITIONAL(LINUX_JOYSTICK, test x$linux_joystick = xyes)

	dnl Optional linux/input.h
	AC_CHECK_HEADERS(linux/input.h, linux_input=yes)
	AM_CONDITIONAL(LINUX_INPUT, test x$linux_input = xyes)

	if test "$WIN32" = "no" && test "$enable_clanDisplay" != "no"; then
	CLANLIB_CHECK_LIB(fontconfig,  [`cat $srcdir/Setup/Tests/fontconfig.cpp`],  clanDisplay, [ *** Cannot find fontconfig (See http://fontconfig.org/ ) (Try libfontconfig1-dev or better) ], [-lfontconfig])
	fi
	if test "$WIN32" = "yes" && test "$enable_clanDisplay" != "no"; then
		CLANLIB_CHECK_LIB(gdi32,[#include <windows.h>
			int main(){} void used_stuff(){ CreateCompatibleDC(NULL); }],
			clanDisplay, [ *** Cannot find gdi32], [-lgdi32])
		CLANLIB_CHECK_LIB(dinput8,[#include <dinput.h>
			#if DIRECTINPUT_HEADER_VERSION < 0x0800
				#error Need newer DirectInput header! (version 8)
			#endif
			int main(){}void used_stuff(){ DirectInput8Create(NULL, 0, IID_IDirectInput8, NULL, 0); } ],
			clanDisplay, [ *** Cannot find version 8 of DirectInput], [-ldxguid -lole32 -ldinput8])
	fi
fi

have_xrender=no

if test "$enable_clanDisplay" != "no"; then
	enable_clanDisplay=yes

	dnl Add any flags required to compiple clanDisplay
	CXXFLAGS="$CXXFLAGS $clanDisplay_CXXFLAGS"

	if test "$X11" = "yes"; then
		AC_PATH_X
		AC_PATH_XTRA

		dnl Add the flags we found to the default ones
		CXXFLAGS="$CXXFLAGS $X_CFLAGS"

		dnl X_LIBS are the X linker flags
		LDFLAGS="$LDFLAGS $X_LIBS"
		
		OLDLIBS="$LIBS"
		LIBS=""

		LIBS="$OLDLIBS"
		clanDisplay_CFLAGS="$clanDisplay_CFLAGS $X_CFLAGS"
		extra_LIBS_clanDisplay=" -lX11 $extra_LIBS_clanDisplay "


		echo ""
		fi

		if test "$enable_clanGL" != "no"; then
			echo "Checking for clanGL stuff"
			echo "=============================="
			if test "$WIN32" = "yes"; then
				CLANLIB_CHECK_LIB(opengl32, [`cat $srcdir/Setup/Tests/opengl.cpp`], clanGL, [ *** Cannot find opengl32], [-lopengl32])
			else
				CLANLIB_CHECK_LIB(GL, [`cat $srcdir/Setup/Tests/opengl.cpp`], clanGL, [ *** Cannot find GL], [ $LIBPTHREAD -lGL ])
				if test "$enable_clanGL" != "no" && test "$X11" = "yes"; then
			
					dnl Check for optional usage of /X11/extensions/Xrender.h
					AC_CHECK_HEADERS(X11/extensions/Xrender.h, have_xrender=yes)

					if test x"$have_xrender" = "xyes"; then
						CLANLIB_CHECK_LIB(Xrender, [`cat $srcdir/Setup/Tests/xrender.cpp`], clanGL, [ *** Cannot find Xrender. Try installing libxrender-dev], [ -lXrender])
					fi

					dnl Check that glXGetProcAddressARB is defined in glx.h
					AC_MSG_CHECKING([whether glx.h defines glXGetProcAddressARB]);
					AC_TRY_COMPILE([#define GLX_GLXEXT_PROTOTYPES
						#include <GL/glx.h>], [ char *foo = (char*) glXGetProcAddressARB; ],
						glx_has_getprocaddressarb="yes", glx_has_getprocaddressarb="no")
					AC_MSG_RESULT([$glx_has_getprocaddressarb])

					if test "x$glx_has_getprocaddressarb" = "xyes"; then
						AC_DEFINE(HAVE_GLX_GETPROCADDRESSARB, 1, [Define if the OpenGL library supports the glXGetProcAddressARB call])
					fi
				fi
			fi
			if test "$enable_clanGL" = "auto"; then enable_clanGL=yes; fi
			echo ""
		fi
	
		if test "$enable_clanGL1" != "no"; then
			echo "Checking for clanGL1 stuff"
			echo "=============================="
			if test "$WIN32" = "yes"; then
				CLANLIB_CHECK_LIB(opengl32, [`cat $srcdir/Setup/Tests/opengl.cpp`], clanGL1, [ *** Cannot find opengl32], [-lopengl32])
			else
				CLANLIB_CHECK_LIB(GL, [`cat $srcdir/Setup/Tests/opengl.cpp`], clanGL1, [ *** Cannot find GL], [ $LIBPTHREAD -lGL ])
				if test "$enable_clanGL1" != "no" && test "$X11" = "yes"; then
			
					dnl Check that glXGetProcAddressARB is defined in glx.h
					AC_MSG_CHECKING([whether glx.h defines glXGetProcAddressARB]);
					AC_TRY_COMPILE([#define GLX_GLXEXT_PROTOTYPES
						#include <GL/glx.h>], [ char *foo = (char*) glXGetProcAddressARB; ],
						glx_has_getprocaddressarb="yes", glx_has_getprocaddressarb="no")
					AC_MSG_RESULT([$glx_has_getprocaddressarb])

					if test "x$glx_has_getprocaddressarb" = "xyes"; then
						AC_DEFINE(HAVE_GLX_GETPROCADDRESSARB, 1, [Define if the OpenGL library supports the glXGetProcAddressARB call])
					fi
				fi
			fi
			if test "$enable_clanGL1" = "auto"; then enable_clanGL1=yes; fi
			echo ""
		fi
	
		if test "$enable_clanCSSLayout" != "no"; then
			echo "Checking for clanCSSLayout stuff"
			echo "=============================="

			if test "$enable_clanCSSLayout" = "auto"; then enable_clanCSSLayout=yes; fi

		fi

		if test "$enable_clanCSSLayout" != "no"; then
			if test "$enable_clanGUI" != "no"; then
				echo "Checking for clanGUI stuff"	
			echo "=============================="
				if test "$enable_clanGUI" = "auto"; then enable_clanGUI=yes; fi

			CLANLIB_CHECK_LIB(gtk, [`cat $srcdir/Setup/Tests/gtk.cpp`], clanGUI, [ *** Cannot find gtk 3 (Try libgtk-3-dev or better) ], [`pkg-config gtk+-3.0 --libs` ], [ `pkg-config gtk+-3.0 --cflags`])

			if test "$enable_clanGUI" = "auto"; then enable_clanGUI=yes; fi
			fi
		else
			CLANLIB_DISABLE_MODULE(clanGUI, [ *** clanGUI depends on clanCSSLayout])
		fi

		echo ""
	else
		CLANLIB_DISABLE_MODULE(clanGUI, [ *** clanGUI depends on clanDisplay])
		CLANLIB_DISABLE_MODULE(clanCSSLayout, [ *** clanCSSLayout depends on clanDisplay])
		CLANLIB_DISABLE_MODULE(clanGL,  [ *** clanGL  depends on clanDisplay])
		CLANLIB_DISABLE_MODULE(clanGL1, [ *** clanGL1 depends on clanDisplay])
	fi

have_alsa=no
if test "$enable_clanSound" != "no"; then
	echo "Checking for clanSound stuff"
	echo "============================"

	if test "$WIN32" = "yes"; then
		CLANLIB_CHECK_LIB(dsound,[#include <dsound.h>
			int main() { return 0; } void used_stuff() {DirectSoundCreate(NULL, NULL, NULL);}],
			clanSound, [ *** Cannot find directsound libs],[-ldxguid -lwinmm -ldsound])
	else
		dnl OSS header check
	soundcard_h=no
	AC_CHECK_HEADERS(machine/soundcard.h sys/soundcard.h soundcard.h, [soundcard_h=yes && break])
	if test "$soundcard_h" == "no"; then
		CLANLIB_DISABLE_MODULE(clanSound,[ *** Cannot find soundcard.h])
	fi
	sound_libs=""
	have_alsa=no
	AC_CHECK_LIB(ossaudio, _oss_ioctl, sound_libs=-lossaudio)
	AC_CHECK_HEADERS(asoundlib.h alsa/asoundlib.h, [have_alsa=yes && break])
	if test "$have_alsa" != "no"; then
		sound_libs="$sound_libs -lasound"
	fi

	extra_LIBS_clanSound="$extra_LIBS_clanSound $sound_libs"
	AM_CONDITIONAL(ALSA, test x$have_alsa = xyes)
	fi

	if test "$enable_clanSound" = "auto"; then enable_clanSound=yes; fi

fi

if test "$enable_clanDatabase" != "no"; then
	echo "Checking for clanDatabase stuff"
	echo "==============================="
	enable_clanDatabase=yes
	echo ""
fi

if test "$enable_clanSqlite" != "no"; then
	echo "Checking for clanSqlite stuff"
	echo "==============================="
	CLANLIB_CHECK_LIB(sqlite, [`cat $srcdir/Setup/Tests/sqlite.cpp`], clanSqlite, [ *** Cannot find sqlite (See: http://www.sqlite.org/ ) (Try  libsqlite3-dev or better) ], [-lsqlite3 -ldl])
	echo ""
fi

if test "$enable_clanSqlite" != "no"; then
	enable_clanDatabase=yes
	enable_clanSqlite=yes
fi


if test "$enable_clanNetwork" != "no"; then
	echo "Checking for clanNetwork stuff"
	echo "=============================="
	if test "$WIN32" = "yes"; then
		dnl make sure we have winsock2
		CLANLIB_CHECK_LIB(ws2_32,[
				#include <winsock2.h>
				int main(){ return 0;}
				void used_stuff() {WSAStartup(0, NULL);}
			], clanNetwork, [ *** Could not find ws2_32 (winsock2)], [-lws2_32])
	fi
fi

if test "$enable_clanNetwork" != "no"; then
	enable_clanNetwork=yes
	AC_CHECK_FUNC(getaddrinfo,getaddr=enabled,)

	AC_MSG_CHECKING(for GetAddr ipv6 support)
	CHECK_ENABLED(getaddr,
		AC_HELP_STRING([--enable-getaddr], [Enable getaddr(ipv6) functions in Network]),
		USE_GETADDR, $getaddr)

	echo ""
fi


dnl -----------------------------------------------------------------------
dnl Store cflags for Setup/pkgconfig/clan*.in pkg-config files
dnl -----------------------------------------------------------------------
AC_SUBST(extra_CFLAGS_clanApp)
AC_SUBST(extra_CFLAGS_clanCore)
AC_SUBST(extra_CFLAGS_clanDisplay)
AC_SUBST(extra_CFLAGS_clanGL)
AC_SUBST(extra_CFLAGS_clanGL1)
AC_SUBST(extra_CFLAGS_clanGUI)
AC_SUBST(extra_CFLAGS_clanCSSLayout)
AC_SUBST(extra_CFLAGS_clanSound)
AC_SUBST(extra_CFLAGS_clanDatabase)
AC_SUBST(extra_CFLAGS_clanSqlite)
AC_SUBST(extra_CFLAGS_clanNetwork)
AC_SUBST(extra_CFLAGS_clanVorbis)

AC_SUBST(clanGUI_CXXFLAGS)

dnl -----------------------------------------------------------------------
dnl Store libs for pkg-config Metadata - for files in Setup/pkgconfig/*.in
dnl -----------------------------------------------------------------------

AC_SUBST(extra_LIBS_clanApp)
AC_SUBST(extra_LIBS_clanCore)
AC_SUBST(extra_LIBS_clanDisplay)
AC_SUBST(extra_LIBS_clanGL)
AC_SUBST(extra_LIBS_clanGL1)
AC_SUBST(extra_LIBS_clanGUI)
AC_SUBST(extra_LIBS_clanCSSLayout)
AC_SUBST(extra_LIBS_clanSound)
AC_SUBST(extra_LIBS_clanDatabase)
AC_SUBST(extra_LIBS_clanSqlite)
AC_SUBST(extra_LIBS_clanNetwork)
AC_SUBST(extra_LIBS_clanVorbis)

dnl ----------------------------------------------------
dnl Which clan subprojects should be compiled?
dnl ----------------------------------------------------

dnl Set the include path
CXXFLAGS="$CXXFLAGS -I\$(top_srcdir)/Sources"

dnl Modules which will be build
ClanLib_Modules="App"
ClanLib_pkgconfig="clanApp.pc"

dnl API header files for the Modules which will be installed
ClanLib_API_Modules="$ClanLib_API_Modules \$(clanApp_includes)"

dnl Default examples which will be build
dnl (Currently Disabled)
ClanLib_Examples=""
CLANLIB_ENABLE_MODULES(Core)

if test "$enable_clanDisplay" = "yes"; then
	CLANLIB_ENABLE_MODULES(Display)
fi
if test "$enable_clanSound" = "yes"; then
	CLANLIB_ENABLE_MODULES(Sound)
fi

if test "$enable_clanDatabase" = "yes"; then
	CLANLIB_ENABLE_MODULES(Database)
fi

if test "$enable_clanSqlite" = "yes"; then
	CLANLIB_ENABLE_MODULES(Sqlite)
fi

if test "$enable_clanGL" = "yes"; then
	CLANLIB_ENABLE_MODULES(GL)
fi

if test "$enable_clanGL1" = "yes"; then
	CLANLIB_ENABLE_MODULES(GL1)
fi

if test "$enable_clanGUI" = "yes"; then
	CLANLIB_ENABLE_MODULES(GUI)
fi

if test "$enable_clanCSSLayout" = "yes"; then
	CLANLIB_ENABLE_MODULES(CSSLayout)
fi

if test "$enable_clanNetwork" = "yes"; then
	CLANLIB_ENABLE_MODULES(Network)
fi

if test "$enable_clanVorbis" = "yes"; then
	CLANLIB_ENABLE_MODULES(Vorbis)
fi

dnl ----------------------------------------
dnl Check for documentation generation tools
dnl ----------------------------------------

if test "$enable_docs" != "yes"; then
	enable_docs=no
fi

if test "$enable_docs" != "no"; then
	echo "Checking for Documentation stuff"
	echo "================================"

	AC_PATH_PROG([perl_exec], [perl])
	if test -z "$perl_exec"; then
		if test "$enable_docs" = "yes"; then
			AC_MSG_ERROR([cannot find perl])
		else
			AC_MSG_WARN([cannot find perl])
			enable_docs=no
		fi
	fi
   AC_PATH_PROG([doxygen_exec], [doxygen])
	if test -z "$doxygen_exec"; then
		if test "$enable_docs" = "yes"; then
			AC_MSG_ERROR([cannot find doxygen])
		else
			AC_MSG_WARN([cannot find doxygen])
			enable_docs=no
		fi
	fi
fi

AC_MSG_CHECKING([if documentation should be generated]); AC_MSG_RESULT([$enable_docs])
if test "$enable_docs" != "no"; then
	AC_SUBST(ClanLib_docs,"Documentation")
	AC_SUBST(ClanLib_add_docs,"Reference Overview")
	enable_docs=yes
	AC_CONFIG_FILES([Documentation/Makefile Documentation/Overview/Makefile Documentation/Reference/Makefile])
fi

echo ""


dnl --------------------
dnl Check for debug mode
dnl --------------------

echo "Checking for debugging and profiling"
echo "===================================="
AC_MSG_CHECKING(for debug mode)
AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug], [Enable debugging support]),, enable_debug="no")

if test "$enable_debug" != "no"; then
	CXXFLAGS="$CXXFLAGS -DDEBUG=1 -g -O0 -Wall"
	AC_MSG_RESULT([enabled])
else
	AC_MSG_RESULT([disabled])
fi

dnl ----------------------
dnl Check for profile mode
dnl ----------------------

AC_MSG_CHECKING(for profile mode)
AC_ARG_ENABLE(profile, AC_HELP_STRING([--enable-profile], [Enable profiling  supprt]),, enable_profile="no")

if test "$enable_profile" != "no"; then
	CXXFLAGS="$CXXFLAGS -pg -g3 -ggdb3 -ftest-coverage -fprofile-arcs"
   dnl support for gcov and gprof
	AC_MSG_RESULT([enabled])
else
	AC_MSG_RESULT([disabled])
fi
echo ""

dnl -------------------------
dnl Check for Maintainer Mode
dnl -------------------------
AC_MSG_CHECKING(for maintainer mode)
AC_ARG_ENABLE(maintainer-mode, AC_HELP_STRING([--enable-maintainer-mode], [Enable maintainer mode]),, enable_maintainer_mode="no")

if test "$enable_maintainer_mode" != "no"; then
	CXXFLAGS="$CXXFLAGS -Werror"
	AC_MSG_RESULT([enabled])
else
	AC_MSG_RESULT([disabled])
fi

dnl ---------------------
dnl Finish configuration:
dnl ---------------------

dnl Set which parts of ClanLib should be built (could probally solved more nicely)
AC_SUBST(ClanLib_Modules)
AC_SUBST(ClanLib_pkgconfig)
AC_SUBST(ClanLib_API_Modules)
AC_SUBST(ClanLib_Examples)

AC_CONFIG_FILES([
		Setup/pkgconfig/clanApp.pc
		Setup/pkgconfig/Makefile
		Makefile
		Examples/Makefile
		Examples/Makefile.conf
		Setup/Makefile
		Sources/API/Makefile
		Sources/App/Makefile
		Sources/Makefile
		Tests/Makefile.conf
		])

AC_OUTPUT

echo "------------------------------------------------------------------------------"
echo "The following options are set: (use ./configure --help for more options)"
echo ""
eval clan_temp="${includedir}"
echo "   Installation Include Dir = ${clan_temp}/ClanLib-${LT_RELEASE}"
eval clan_temp="${libdir}"
eval clan_temp="${clan_temp}"
echo "   Installation Library Dir = ${clan_temp}"
eval clan_temp="${datadir}"
eval clan_temp="${clan_temp}"
echo "      Installation Docs Dir = ${clan_temp}/doc/${PACKAGE}-${LT_RELEASE}"

dnl Do a check to see if the user has forgotton to set the prefix directory
if test "$prefix" == "/usr/local"; then
	echo " * Warning: installation directory is /usr/local. This may cause problems on some linux distros. Use --prefix=/usr if required *"
fi

dnl echo ""
dnl echo "                   CXXFLAGS = $CXXFLAGS"
dnl echo "                   CPPFLAGS = $CPPFLAGS"
dnl echo "                       LIBS = $LIBS"
dnl echo "                    LDFLAGS = $LDFLAGS"
echo ""
echo "The following modules will be built:"
echo ""
gl_options=""
if test "$enable_clanGL" != "no"; then
	if test "$have_xrender" != "no"; then
		gl_options="$display_options (XRender Enabled)"
	else
		gl_options="$display_options (XRender Disabled)"
	fi
fi

echo "                     clanGL = $enable_clanGL$gl_options"
echo "                    clanGL1 = $enable_clanGL1"
echo "                    clanApp = yes"
echo "               clanDatabase = $enable_clanDatabase"
echo "                 clanSqlite = $enable_clanSqlite"
echo "                    clanGUI = $enable_clanGUI"
echo "              clanCSSLayout = $enable_clanCSSLayout"

core_options=""
if test "$use_sse2" != "no"; then
	core_options="$core_options (SSE2 Enabled)"
else
	core_options="$core_options (SSE2 Disabled)"
fi

if test "$gcc_cv_have_tls" != "yes"; then
	core_options="$core_options * Warning Thread Local Storage is Disabled *"
fi

echo "                   clanCore = yes$core_options"

sound_options=""
if test "$enable_clanSound" != "no"; then
	if test "$have_alsa" != "no"; then
		sound_options="$sound_options (ALSA Enabled)"
	else
		sound_options="$sound_options (ALSA Disabled)"
	fi
fi

echo "                  clanSound = $enable_clanSound$sound_options"
echo "                clanNetwork = $enable_clanNetwork"

display_options=""
if test "$enable_clanDisplay" != "no"; then
	if test "$linux_joystick" != "no"; then
		display_options="$display_options (Linux Joystick Enabled)"
	else
		display_options="$display_options (Linux Joystick Disabled)"
	fi

	if test "$linux_input" != "no"; then
		display_options="$display_options (Linux Input Enabled)"
	else
		display_options="$display_options (Linux Input Disabled)"
	fi
fi

echo "                clanDisplay = $enable_clanDisplay$display_options"
echo "                 clanVorbis = $enable_clanVorbis"
echo ""

if test "$enable_docs" == "no"; then
	echo "        Build Documentation = no (Use --enable-docs to enable)"
else
	echo "        Build Documentation = yes (Use \"make html\" and \"make install-html\" to also install the API documentation)"
fi

if test "$enable_debug" != "no"; then
	echo "                Debug Build = yes"
	else
	echo "                Debug Build = no (Use --enable-debug to enable)"
fi

echo "------------------------------------------------------------------------------"
echo "Configure done."
echo ""
echo "Type 'make' to compile ClanLib."
echo "Then 'sudo make install' to install ClanLib."
echo ""

dnl EOF dnl


