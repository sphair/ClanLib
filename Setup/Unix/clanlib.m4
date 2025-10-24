dnl CLANLIB_INIT([MINIMUM-VERSION])
dnl
dnl Check for clanlib availability,etc

dnl 
AC_DEFUN([CLANLIB_INIT],
[
	AC_CHECKING(ClanLib presence)
	dnl AC_MSG_CHECKING(for clanlib-config)
	AC_CHECK_PROG(clanlib_config,clanlib-config,yes,no)
	if test "$clanlib_config" = "no"; then
		AC_MSG_ERROR(please make sure you've installed clanlib correctly)
	else
		AC_PATH_PROG(CLANLIB_CONFIG,clanlib-config)
		
		CLANLIB_LDFLAGS=`clanlib-config --libs`
		CLANLIB_CXXFLAGS=`clanlib-config --cflags`

		min_clanlib_version=ifelse([$1], , 0.5.0,$1)
		AC_MSG_CHECKING(for ClanLib - version >= $min_clanlib_version)

    	clanlib_config_major_version=`$CLANLIB_CONFIG --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    	clanlib_config_minor_version=`$CLANLIB_CONFIG --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    	clanlib_config_micro_version=`$CLANLIB_CONFIG --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

		clanlib_config_version="$clanlib_config_major_version.$clanlib_config_minor_version.$clanlib_config_micro_version"
		dnl echo "$clanlib_config_version (found)"
		
  		if test "x$no_clanlib" = xyes ; then
     		AC_MSG_ERROR(failed to find minimum required ClanLib)      
  		else
  			AC_MSG_RESULT(ok)
  		fi

		
		AC_SUBST(CLANLIB_LDFLAGS)
		AC_SUBST(CLANLIB_CXXFLAGS)
	fi
])

