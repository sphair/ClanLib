dnl CLANLIB_CHECK_LIB(lib, testprog, module, failed_message, use_libs, use_cflags) library test macro
define([CLANLIB_CHECK_LIB],[if test "$enable_$3" != "no"; then
    OLDLIBS="$LIBS"; LIBS="$5"; AC_MSG_CHECKING(for $1)
    OLD_CXXFLAGS="$CXXFLAGS"; CXXFLAGS="$6";
    AC_RUN_IFELSE([AC_LANG_SOURCE($2)], [CL_RESULT=yes], [CL_RESULT=no], [AC_LINK_IFELSE([AC_LANG_SOURCE($2)], [CL_RESULT="yes, linked"],[CL_RESULT=no])])
    AC_MSG_RESULT([$CL_RESULT])
    if test "$CL_RESULT" = "no"; then
        CLANLIB_DISABLE_MODULE([$3],[$4])
    else
        $3_CXXFLAGS=" $$3_CXXFLAGS $CXXFLAGS "
        extra_LIBS_$3=" $extra_LIBS_$3 $LIBS "
    fi
    CXXFLAGS="$OLD_CXXFLAGS"
    LIBS="$OLDLIBS"
fi])

dnl CLANLIB_CHECK_CPP(testprog, use_libs, use_cflags) library test macro
define([CLANLIB_CHECK_CPP],[
    OLDLIBS="$LIBS"; LIBS="$2";
    OLD_CXXFLAGS="$CXXFLAGS"; CXXFLAGS="$3";
    AC_RUN_IFELSE([AC_LANG_SOURCE($1)], [CL_RESULT=yes], [CL_RESULT=no], [CL_RESULT=no])
    CXXFLAGS="$OLD_CXXFLAGS"
    LIBS="$OLDLIBS"
])

dnl CLANLIB_DISABLE_MODULE(module, message)
define([CLANLIB_DISABLE_MODULE],[
if test "$enable_$1" = "yes"; then
    AC_MSG_ERROR([$2])
else
    AC_MSG_WARN([$2. Disabling $1]);enable_$1=no
fi])

dnl CLANLIB_ENABLE_MODULES(modules) set Module variables
define([CLANLIB_ENABLE_MODULES],[
ClanLib_Modules="$ClanLib_Modules $1"
ClanLib_pkgconfig="$ClanLib_pkgconfig m4_bpatsubst($1,\w+,[clan\&.pc])"
ClanLib_API_Modules="$ClanLib_API_Modules m4_bpatsubst($1,\w+,[\\$(clan\&_includes)])"
AC_CONFIG_FILES(m4_bpatsubst($1,\w+,Sources/\&/Makefile Setup/Unix/pkgconfig/clan\&.pc))])

dnl CLANLIB_ARG_ENABLE(option, default, option label, checking message)
define([CLANLIB_ARG_ENABLE],[AC_MSG_CHECKING($4)
AC_ARG_ENABLE($1, AS_HELP_STRING([--enable-$1],[$3]),,[enable_$1="$2"])
AC_MSG_RESULT([$enable_$1])])
dnl ------------------------------------------------
dnl BBNs macro for checking commandline parameters:
dnl ------------------------------------------------

dnl CHECK_ENABLED(feature,help,define,default)
define([CHECK_ENABLED],
[
  AC_ARG_ENABLE($1,[$2],
  [
    if test "$enableval" != "no"; then
      $3="enabled"
      AC_DEFINE($3)
      AC_MSG_RESULT([enabled])
    else
      $3="disabled"
      AC_MSG_RESULT([disabled])
    fi
  ],
  [
    if test "$4" = "enabled"; then
      $3="enabled"
      AC_DEFINE($3)
      AC_MSG_RESULT([enabled])
    else
      $3="disabled"
      AC_MSG_RESULT([disabled])
    fi
  ])
])


dnl EOF dnl

