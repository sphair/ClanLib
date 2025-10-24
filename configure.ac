dnl $Id: configure.ac,v 1.40 2004/04/04 16:40:22 grumbel Exp $
dnl
dnl Process this file with autoconf to produce a configure script.
dnl

AC_PREREQ(2.50)
AC_INIT(ClanLib, 1.0.0)
AC_CONFIG_SRCDIR(Sources/API/core.h)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE( foreign dist-bzip2 dist-zip 1.5b subdir-objects )

CLANLIB_MAJOR_VERSION=1
CLANLIB_MINOR_VERSION=0
CLANLIB_MICRO_VERSION=0

CLANLIB_VERSION=$CLANLIB_MAJOR_VERSION.$CLANLIB_MINOR_VERSION.$CLANLIB_MICRO_VERSION

AC_SUBST(CLANLIB_MAJOR_VERSION)
AC_SUBST(CLANLIB_MINOR_VERSION)
AC_SUBST(CLANLIB_MICRO_VERSION)
AC_SUBST(CLANLIB_VERSION)

# Set this to: MAJOR.MINOR, ie 0.7, 0.8, 0.9 it gets append as:
#  libclanDisplay-$LT_RELEASE
LT_RELEASE=$CLANLIB_MAJOR_VERSION.$CLANLIB_MINOR_VERSION
if test -z "$LDFLAGS_LT_RELEASE"; then
	LDFLAGS_LT_RELEASE=-release $LT_RELEASE
fi

# If any features got added to the API since last release incremeant
# this by 1 (ie increment this for each develpment release, and only
# if features have been added to a stable release)
LT_CURRENT=1

# If any interfaces have been added since the last public release,
# then increment AGE. If any interfaces have been removed since the
# last public release, then set AGE to 0. So when backward compability
# is broken, set this to 0, else stay in sinc with LT_CURRENT
# (development releases should stay 0, stable be in sync with
# LT_CURRENT)
LT_AGE=0

# If LT_CURRENT has changed, set this to 0, if LT_CURRENT stayed the
# same increment by 1 (so stable releases should increment, while
# development releases stay at 0)
LT_REVISION=0

# libclanFoobar-$LT_RELEASE.so.[$LT_CURRENT-$LT_AGE].$LT_AGE.$LT_REVISION

AC_SUBST(LDFLAGS_LT_RELEASE)
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AC_PROG_CXX
AC_PROG_CC
AM_PROG_AS
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_LANG(C++)

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

case $target in
    *cygwin* )
        X11="no"
        WIN32="yes"
        CXXFLAGS="$CXXFLAGS -mno-cygwin -DWIN32"
        ;;
    *mingw*)
        X11="no"
        WIN32="yes"
        ;;
    *)
        X11="yes"
        WIN32="no"
        dnl We set CC and AS here to make libtool happy
        dnl FIXME: Why exactly do we do this?!
        ;;
esac

# Extra flags that are needed to compile
if test "$WIN32" = "yes"; then
        extra_CFLAGS_clanApp="-mwindows"
        extra_LIBS_clanCore="-lz"
        extra_LIBS_clanDisplay="-lwinmm -lddraw -ldinput8 -ldxguid -lgdi32 -lole32 -ljpeg -lpng"
        extra_LIBS_clanGL="-lopengl32 -lglu32"
        extra_LIBS_clanSound="-ldsound -ldxguid -lwinmm"
        extra_LIBS_clanMikMod="-lmikmod"
        extra_LIBS_clanVorbis="-lvorbis -logg"
        extra_LIBS_clanSDL="-lSDL"
        extra_LIBS_clanNetwork="-lws2_32"
fi

AC_SUBST(extra_CFLAGS_clanApp)
AC_SUBST(extra_CFLAGS_clanCore)
AC_SUBST(extra_CFLAGS_clanDisplay)
AC_SUBST(extra_CFLAGS_clanGL)
AC_SUBST(extra_CFLAGS_clanSDL)
AC_SUBST(extra_CFLAGS_clanGUI)
AC_SUBST(extra_CFLAGS_clanSignals)
AC_SUBST(extra_CFLAGS_clanSound)
AC_SUBST(extra_CFLAGS_clanNetwork)
AC_SUBST(extra_CFLAGS_clanVorbis)
AC_SUBST(extra_CFLAGS_clanMikMod)

AC_SUBST(extra_LIBS_clanApp)
AC_SUBST(extra_LIBS_clanCore)
AC_SUBST(extra_LIBS_clanDisplay)
AC_SUBST(extra_LIBS_clanGL)
AC_SUBST(extra_LIBS_clanSDL)
AC_SUBST(extra_LIBS_clanGUI)
AC_SUBST(extra_LIBS_clanSignals)
AC_SUBST(extra_LIBS_clanSound)
AC_SUBST(extra_LIBS_clanNetwork)
AC_SUBST(extra_LIBS_clanVorbis)
AC_SUBST(extra_LIBS_clanMikMod)

AM_CONDITIONAL([WIN32], [test $WIN32 = "yes"])
AM_CONDITIONAL(LINUX, [test -n "`echo ${target} | grep linux`"])

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

AC_ARG_WITH(sdl-gfx, AS_HELP_STRING([--with-sdl-gfx],[use sdl-gfx library for scaling and rotation (automatic)]),
  use_sdl_gfx="$withval", use_sdl_gfx=auto)

AC_CHECK_HEADERS(unistd.h fcntl.h sys/kd.h sys/vt.h sys/sysctl.h)
AC_EGREP_HEADER(fcvt, stdlib.h,,AC_DEFINE(NEED_FCVT))

dnl Checks the system type
AC_MSG_CHECKING(for i386)
case $target_cpu in
    i?86) i386=enabled;;
    *)    i386=disabled;;
esac

if test x$i386 = "xenabled"; then
    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi

dnl -----------------------------------------------------
dnl Check commandline parameters to the configure script:
dnl -----------------------------------------------------

AC_MSG_CHECKING(for i386 assembly support)
CHECK_ENABLED(asm386,
    AC_HELP_STRING([--enable-asm386], [Enable i386 assembly support]),
    USE_I386_ASSEMBLER, $i386)

AC_MSG_CHECKING(for dynamic loading support)
CHECK_ENABLED(dyn,
   AC_HELP_STRING([--enable-dyn], [Enable dynamic loading]),
   USE_DYN, disabled)

AC_CHECK_HEADER(libgen.h)
AC_CHECK_HEADER(sys/types.h)
AC_CHECK_HEADER(sys/ipc.h)
AC_CHECK_HEADER(sys/shm.h)
AC_CHECK_HEADER(fstab.h, AC_DEFINE([HAVE_FSTAB_H]))

AC_MSG_CHECKING([for extern *__progname])
AC_TRY_COMPILE([],
[extern const char *__progname;],
AC_MSG_RESULT(yes);AC_DEFINE(EXTERN___PROGNAME),
AC_MSG_RESULT(no))

dnl -----------------------------------------------------------------------
dnl Check system endianess: (note: ClanLib is _not_ endian clean right now)
dnl -----------------------------------------------------------------------

dnl AC_C_BIGENDIAN
dnl AC_CHECK_SIZEOF(int, 4)

if test "$ac_cv_sizeof_int" = "8"; then
	AC_DEFINE(USE_64BIT)
fi

if test "$ac_cv_c_bigendian" = "yes"; then
	AC_DEFINE(USE_BIG_ENDIAN)
fi

dnl ----------------------------------------
dnl Search for libraries useful for ClanLib:
dnl ----------------------------------------
if test "$WIN32" = "no"; then
    AC_CHECK_LIB(pthread,pthread_create,AC_SUBST(LIBPTHREAD,"-lpthread"), AC_MSG_ERROR([" *** ClanLib requires pthread to run."]))
fi
AC_CHECK_LIB(z, main, , AC_MSG_ERROR([" *** ClanLib requires zlib."]))

dnl ----------------------------------------------------------
dnl Check for ClanLib modules that should be enabled/disabled
dnl ----------------------------------------------------------
echo ""
echo "Checking for ClanLib Modules to build..."
echo "========================================"

CLANLIB_ARG_ENABLE(docs,        auto, [Build Clanlib documentation], [whether we should try to build documentation])

CLANLIB_ARG_ENABLE(clanDisplay, auto, [Build clanDisplay module],    [whether we should try to build clanDisplay])

CLANLIB_ARG_ENABLE(clanSDL,     auto, [Build clanSDL module],        [whether we should try to build clanSDL])

CLANLIB_ARG_ENABLE(clanGL,      auto, [Build clanGL module],         [whether we should try to build clanGL])

CLANLIB_ARG_ENABLE(clanSound,   auto, [Build clanSound module],      [whether we should try to build clanSound])

CLANLIB_ARG_ENABLE(clanNetwork, auto, [Build clanNetwork module],    [whether we should try to build clanNetwork])

CLANLIB_ARG_ENABLE(clanGUI,     auto, [Build clanGUI module],        [whether we should try to build clanGUI])

CLANLIB_ARG_ENABLE(clanMikMod,  auto, [Build clanMikMod module],     [whether we should try to build clanMikMod])

CLANLIB_ARG_ENABLE(clanVorbis,  auto, [Build clanVorbis module],     [whether we should try to build clanVorbis])

echo ""
if test "$enable_clanDisplay" != "no"; then
    echo "Checking for clanDisplay stuff"
    echo "=============================="
    CLANLIB_CHECK_LIB(png,  [`cat $srcdir/Setup/Tests/png.cpp`],  clanDisplay, [ *** Cannot find png], [-lz])
    CLANLIB_CHECK_LIB(jpeg, [`cat $srcdir/Setup/Tests/jpeg.cpp`], clanDisplay, [ *** Cannot find jpeg])
    if test "$WIN32" = "yes" && test "$enable_clanDisplay" != "no"; then
        CLANLIB_CHECK_LIB(gdi32,[#include <windows.h>
             int main(){} void used_stuff(){ CreateCompatibleDC(NULL); }],
            clanDisplay, [ *** Cannot find gdi32])
        CLANLIB_CHECK_LIB(dinput8,[#include <dinput.h>
             #if DIRECTINPUT_HEADER_VERSION < 0x0800
                 #error Need newer DirectInput header! (version 8)
             #endif
             int main(){}void used_stuff(){ DirectInput8Create(NULL, 0, IID_IDirectInput8, NULL, 0); } ],
            clanDisplay, [ *** Cannot find version 8 of DirectInput], [-ldxguid -lole32])
    fi
fi

if test "$enable_clanDisplay" != "no"; then
    enable_clanDisplay=yes
    if test "$X11" = "yes"; then
        AC_PATH_X
        AC_PATH_XTRA
        
        if false; then
            echo "---------------------------"
            echo "LIBS:         $LIBS"
            echo "LDFLAGS:      $LDFLAGS"
            echo "CXXFLAGS:     $CXXFLAGS"
            echo "CFLAGS:       $CFLAGS"
            echo "X_CFLAGS:     $X_CFLAGS"
            echo "X_LIBS:       $X_LIBS"
            echo "X_EXTRA_LIBS: $X_EXTRA_LIBS"
            echo "X_PRE_LIBS:   $X_PRE_LIBS"
            echo "x_includes:   $x_includes"
            echo "x_libraries:  $x_libraries"
            echo "---------------------------"
        fi
        
        dnl Add the flags we found to the default ones
        dnl CLAGS="$CFLAGS $X_CFLAGS"
        dnl CXXFLAGS="$CXXFLAGS $X_CFLAGS"
        CPPFLAGS="$CPPFLAGS $X_CFLAGS"
        LDFLAGS="$LDFLAGS $X_LIBS"

        
        OLDLIBS="$LIBS"
        LIBS=""
        AC_SEARCH_LIBS(XF86VidModeQueryExtension, [ Xxf86vm_pic Xxf86vm ],
            [ VIDMODE=enabled; X_LIBS="$X_LIBS $LIBS" ], [ VIDMODE=disabled ], [ $X_LIBS ])
        LIBS="$OLDLIBS"
        clanDisplay_CFLAGS="$clanDisplay_CFLAGS $X_CFLAGS"
        clanDisplay_LIBS="$clanDisplay_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS"
    fi

    if test "$enable_clanSDL" != "no"; then
        AC_LANG(C)
        echo "Checking for clanSDL stuff"
        echo "=============================="
        AM_PATH_SDL([1.2.0], 
            [AC_SUBST(SDL_CFLAGS)
             AC_SUBST(SDL_LIBS)],
            CLANLIB_DISABLE_MODULE(clanSDL, [ *** Could not find SDL]))
        if test "$enable_clanSDL" != "no"; then
        enable_clanSDL=yes
        dnl testing for SDL_gfx
        if test "$use_sdl_gfx" != "no"; then
        OLDLIBS="$LIBS"
        LIBS="$SDL_LIBS $LIBS"
        AC_CHECK_LIB(SDL_gfx, zoomSurface, 
            [SDL_LIBS="$SDL_LIBS -lSDL_gfx"
             AC_DEFINE(HAVE_LIBSDL_GFX)], 
            [if test "$use_sdl_gfx" = "auto"; then
                AC_MSG_WARN([SDL_gfx not found, clanSDL will not be able to use scaling and rotation!])
             else
                AC_MSG_ERROR([SDL_gfx not found!])
             fi])
        LIBS="$OLDLIBS"
        fi
        clanSDL_LIBS="$clanSDL_LIBS $SDL_LIBS"
        clanSDL_CFLAGS="$clanSDL_CFLAGS $SDL_CFLAGS"
        fi
        AC_LANG(C++)
    fi

    if test "$enable_clanGL" != "no"; then
        echo "Checking for clanGL stuff"
        echo "=============================="
        if test "$WIN32" = "yes"; then
            CLANLIB_CHECK_LIB(opengl32, [`cat $srcdir/Setup/Tests/opengl.cpp`], clanGL, [ *** Cannot find opengl32/glu32], [-lglu32])
        else # test "$WIN32" = "yes"
            CLANLIB_CHECK_LIB(GL, [`cat $srcdir/Setup/Tests/opengl.cpp`], clanGL, [ *** Cannot find GL/GLU], [-lGLU -lpthread])
            if test "$enable_clanGL" != "no" && test "$X11" = "yes"; then
                AC_CHECK_LIB(Xi,XListInputDevices, [clanGL_LIBS="$clanGL_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS -lXi"],
                    CLANLIB_DISABLE_MODULE(clanGL, [ *** Cannot find libXi (xinput)]))
                if test "$enable_clanGL" != "no"; then
            
            dnl Check that glXGetProcAddressARB is defined in glx.h
            AC_MSG_CHECKING([whether glx.h defines glXGetProcAddressARB]);
            AC_TRY_COMPILE([#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>], [ char *foo = (char*) glXGetProcAddressARB; ],
                            glx_has_getprocaddressarb="yes", glx_has_getprocaddressarb="no")
            AC_MSG_RESULT([$glx_has_getprocaddressarb])

            if test "x$glx_has_getprocaddressarb" = "xyes"; then
              AC_DEFINE(HAVE_GLX_GETPROCADDRESSARB, 1,
                 [Define if the OpenGL library supports the glXGetProcAddressARB call])
            fi

	    dnl Check for linux/joystick.h
	    linux_joystick=no
	    AC_CHECK_HEADERS(linux/joystick.h, linux_joystick=yes)
	    AM_CONDITIONAL(LINUX_JOYSTICK, test x$linux_joystick = xyes)

	    dnl Check for linux/input.h
	    linux_input=no
	    AC_CHECK_HEADERS(linux/input.h, linux_input=yes)
	    AM_CONDITIONAL(LINUX_INPUT, test x$linux_input = xyes)
                fi
            fi
        fi
        test "$enable_clanGL" = "auto" && enable_clanGL=yes
    fi
    test "$enable_clanGUI" = "auto" && enable_clanGUI=yes
    echo ""
else
    CLANLIB_DISABLE_MODULE(clanSDL, [ *** clanSDL depends on clanDisplay])
    CLANLIB_DISABLE_MODULE(clanGUI, [ *** clanGUI depends on clanDisplay])
    CLANLIB_DISABLE_MODULE(clanGL,  [ *** clanGL  depends on clanDisplay])
fi

if test "$enable_clanSound" != "no"; then
    echo "Checking for clanSound stuff"
    echo "============================"
    if test "$WIN32" = "yes"; then
        CLANLIB_CHECK_LIB(dsound,[#include <dsound.h>
             int main() { return 0; } void used_stuff() {DirectSoundCreate(NULL, NULL, NULL);}],
            clanSound, [ *** Cannot find directsound libs],[-ldxguid -lwinmm])
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
	SOUND_LDFLAGS="$sound_libs"
	AM_CONDITIONAL(ALSA, test x$have_alsa = xyes)
	AC_SUBST(SOUND_LDFLAGS)
    fi
fi

if test "$enable_clanSound" != "no"; then
    enable_clanSound=yes
    echo ""
    CLANLIB_CHECK_LIB(mikmod, [`cat $srcdir/Setup/Tests/mikmod.cpp`], clanMikMod, [ *** Cannot find mikmod])
    test "$enable_clanMikMod" = "auto" && enable_clanMikMod=yes
    
    PKG_CHECK_MODULES(clanVorbis, vorbis >= 1.0,,[AC_MSG_CHECKING([for libvorbis directly])
        CLANLIB_CHECK_LIB(vorbis, [`cat $srcdir/Setup/Tests/vorbis.cpp`], clanVorbis, [ *** Cannot find vorbis],[-logg])])
    test "$enable_clanVorbis" = "auto" && enable_clanVorbis=yes
    echo ""
else
    CLANLIB_DISABLE_MODULE(clanMikMod, [ *** clanMikMod depends on clanSound])
    CLANLIB_DISABLE_MODULE(clanVorbis, [ *** clanVorbis depends on clanSound])
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
            ], clanNetwork, [ *** Could not find ws2_32 (winsock2)])
    fi
fi

if test "$enable_clanNetwork" != "no"; then
    enable_clanNetwork=yes
    dnl Disabled IPV6 thing because on all the linuxes I have tried it on, it just doesnt work.
    dnl (the getaddr functions are there, but using it just causes it to fail) -- mbn 27 marts 2003
    dnl Works fine on my system.  You gotta have the ipv6 module loaded thought. --Mark April 13, 2003
    AC_CHECK_FUNC(getaddrinfo,getaddr=enabled,)

    AC_MSG_CHECKING(for GetAddr (ipv6) support)
    CHECK_ENABLED(getaddr,
        AC_HELP_STRING([--enable-getaddr], [Enable getaddr(ipv6) functions in Network]),
        USE_GETADDR, $getaddr)

    echo ""
fi

dnl ----------------------------------------------------
dnl Which clan subprojects & examples should be compiled?
dnl ----------------------------------------------------

# Set the include path
CXXFLAGS="$CXXFLAGS -I\$(top_srcdir)/Sources"

# Modules which will be build
ClanLib_Modules="Application"
ClanLib_pkgconfig="clanApp.pc"

# API header files for the Modules which will be installed
ClanLib_API_Modules="$ClanLib_API_Modules \$(clanApp_includes)"

# Default examples which will be build
ClanLib_Examples="Directory Minimum Resources ResourcesCustom Signals ThreadSync XML Zip"

CLANLIB_ENABLE_MODULES([Core Signals])

if test "$enable_clanDisplay" = "yes"; then
    CLANLIB_ENABLE_MODULES(Display)
fi
if test "$enable_clanSound" = "yes"; then
    CLANLIB_ENABLE_MODULES(Sound)
	ClanLib_Examples="$ClanLib_Examples SoundFilters SoundBuffer StreamSoundProvider PlayCD"
fi

if test "$enable_clanSDL" = "yes"; then
    CLANLIB_ENABLE_MODULES(SDL)
fi
if test "$enable_clanGL" = "yes"; then
    CLANLIB_ENABLE_MODULES(GL)
	ClanLib_Examples="$ClanLib_Examples AdventureScene Basic2D Canvas DisplayModes Font GLCube GLFog GlyphBuffer InitDisplay Joystick MousePoll MouseSignal Pacman ShadowOfTheBeast Surface SpritesRTS ScreenShot Time"
fi

if test "$enable_clanGUI" = "yes"; then
    CLANLIB_ENABLE_MODULES([GUI GUIStyleSilver])
	ClanLib_Examples="$ClanLib_Examples GLGUICube GUISimple GUILayout GUITreeView GUIWindow GUIInputDialog GUIScript GUIStyle PilotsPuzzle"
fi

if test "$enable_clanNetwork" = "yes"; then
    CLANLIB_ENABLE_MODULES(Network)
	ClanLib_Examples="$ClanLib_Examples NetChannel NetFTP IRCBot CTalk UDPTunnel"
	# todo: Many of these examples require more than Network, and a check should be performed
fi

if test "$enable_clanMikMod" = "yes"; then
    CLANLIB_ENABLE_MODULES(MikMod)
	ClanLib_Examples="$ClanLib_Examples MikMod"
fi

if test "$enable_clanVorbis" = "yes"; then
    CLANLIB_ENABLE_MODULES(Vorbis)
	ClanLib_Examples="$ClanLib_Examples Vorbis"
fi

dnl ----------------------------------------------------
dnl Setup libs to be linked with the core library files.
dnl ----------------------------------------------------

dnl check wich display targets to build

if false; then
    if test "$USE_DIRECTFB" = "enabled"; then 
	disp_directfb='$(DISP_DIRECTFB)'; 
    else
	directfb_cflags=""; 
	directfb_libs=""; 
    fi
    
    AC_SUBST(directfb_cflags)
    AC_SUBST(directfb_libs)
    AC_SUBST(disp_directfb)
fi

AC_SUBST(js_cflags)
AC_SUBST(js_libs)
AC_SUBST(js_defs)

AC_SUBST(extra_includes)
AC_SUBST(extra_libraries)

AC_SUBST(libs)
AC_SUBST(objf_nondyn)
AC_SUBST(LIBS)

dnl ---------------------------------------------------------------------
dnl Set which extra object files that should be used in case of assembly:
dnl ---------------------------------------------------------------------

if test "$USE_I386_ASSEMBLER" = "enabled"; then
    clanSound_extra_objects="$clanSound_extra_objects mixer_asm.lo"
fi

AC_SUBST(clanSound_extra_objects)

dnl ----------------------------------------
dnl Check for documentation generation tools
dnl ----------------------------------------
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
fi
if test "$enable_docs" != "no"; then
    xslt_exec=''
    AC_PATH_PROGS([xslt_exec], [Xalan xsltproc sabcmd MSXSL])
    if test -z "$xslt_exec"; then
        if test "$enable_docs" = "yes"; then
            AC_MSG_ERROR([cannot find xalan, xsltproc, sabcmd or MSXSL])
        else
            AC_MSG_WARN([cannot find xalan, xsltproc, sabcmd or MSXSL. Disabling documentation generation]); enable_docs=no
        fi
    else
        # $1="xml file" $2="output file" $3="xslt stylesheet"
        case "$xslt_exec" in
        *Xalan)
           xslt_exec="$xslt_exec -o \$2 \$1 \$3"
           ;;
        *xsltproc)
           xslt_exec="$xslt_exec -o \$2 \$3 \$1"
           ;;
        *sabcmd)
           xslt_exec="$xslt_exec \$3 \$1 \$2"
           ;;
        *MSXSL)
           xslt_exec="$xslt_exec \$1 \$3 -o \$2"
        esac
    fi
fi
AC_MSG_CHECKING([if documentation should be generated]); AC_MSG_RESULT([$enable_docs])
if test "$enable_docs" != "no"; then
    Kavanek_Tutorial_HTML=CLANLIB_FILE_LIST([Documentation/Tutorial/Kavanek], [*.xml], ['$0!="menu.xml"{sub("\\.xml$",".html");print}'])
    AC_SUBST(Kavanek_Tutorial_HTML)
    AC_SUBST(ClanLib_docs,"Documentation")
    AC_SUBST(ClanLib_add_docs,"Reference Overview")
    enable_docs=yes
    AC_CONFIG_FILES([Documentation/Utilities/xslt.sh], [chmod +x Documentation/Utilities/xslt.sh])
    AC_CONFIG_FILES([Documentation/Utilities/webbuilder.pl], [chmod +x Documentation/Utilities/webbuilder.pl])
    AC_CONFIG_FILES([Documentation/Reference/pce2], [chmod +x Documentation/Reference/pce2])
    AC_CONFIG_FILES([Documentation/Makefile Documentation/Overview/Makefile Documentation/Reference/Makefile
        Documentation/Tutorial/Makefile Documentation/Tutorial/Kavanek/Makefile])
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

# Set which parts of ClanLib should be build (could probally solved more nicely)
AC_SUBST(ClanLib_Modules)
AC_SUBST(ClanLib_pkgconfig)
AC_SUBST(ClanLib_API_Modules)
AC_SUBST(ClanLib_Examples)

AC_CONFIG_FILES([
        pkgconfig/clanApp.pc
        pkgconfig/Makefile
        Makefile
        Examples/Makefile
        Examples/Makefile.conf
        Setup/Makefile
        Sources/API/Makefile
        Sources/Application/Makefile
        Sources/Makefile
        Tests/Makefile.conf
        ])

AC_OUTPUT


dnl echo Building dependencies...please wait.
dnl make dep > /dev/null 2>&1

echo "------------------------------------------------------------------------------"
echo "The following modules will be built:"
echo ""
echo "                     clanGL = $enable_clanGL"
echo "                    clanSDL = $enable_clanSDL"
echo "                    clanApp = yes"
echo "                    clanGUI = $enable_clanGUI"
echo "                   clanCore = yes"
echo "                  clanSound = $enable_clanSound"
echo "                clanNetwork = $enable_clanNetwork"
echo "                clanSignals = yes"
echo "                clanDisplay = $enable_clanDisplay"
echo "                 clanMikmod = $enable_clanMikMod"
echo "                 clanVorbis = $enable_clanVorbis"
echo ""
echo "        Build Documentation = $enable_docs"
echo "                Debug Build = $enable_debug"
dnl echo "                   CXXFLAGS = $CXXFLAGS"
dnl echo "                       LIBS = $LIBS"
dnl echo "                    LDFLAGS = $LDFLAGS"
dnl echo ""
echo "------------------------------------------------------------------------------"
echo "Configure done."
echo ""
echo "Type 'make' to compile ClanLib."
echo ""

dnl Local Variables:
dnl compile-command: "./autogen.sh && ./configure"
dnl End:

dnl EOF dnl
