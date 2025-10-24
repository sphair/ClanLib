#!/usr/bin/perl -w
#==============================================================================
#
#    Eric's File Editor (tm)
#    Copyright (C) 2000,2001 by Eric Sunshine <sunshine@sunshineco.com>
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
#==============================================================================
#------------------------------------------------------------------------------
# efed.pl
#
#    A tool for applying pattern-based modifications to the content and/or
#    name of a file or batch of files.
#
#------------------------------------------------------------------------------
use strict;
use File::Basename;
use Getopt::Long;
$Getopt::Long::ignorecase = 0;

my $PROG_NAME = 'efed.pl';
my $PROG_VERSION = 5;
my $AUTHOR_NAME = 'Eric Sunshine';
my $AUTHOR_EMAIL = 'sunshine@sunshineco.com';
my $COPYRIGHT = "Copyright (C) 2000,2001 by $AUTHOR_NAME <$AUTHOR_EMAIL>";

@main::opt_edit = ();
@main::opt_rename = ();
@main::opt_accept = ();
@main::opt_ignore = ();
$main::opt_descend = 0;
$main::opt_verbose = 0;
$main::opt_v = 0;	# Alias for 'verbose'.
$main::opt_quiet = 0;
$main::opt_test = 0;
$main::opt_version = 0;
$main::opt_V = 0;	# Alias for 'version'.
$main::opt_help = 0;

my @script_options = (
    'edit=s@',
    'rename=s@',
    'accept=s@',
    'ignore=s@',
    'descend!',
    'verbose!',
    'v!',		# Alias for 'verbose'.
    'quiet!',
    'test!',
    'version',
    'V',		# Alias for 'version'.
    'help'
);

$main::verbosity = 0;
@main::paths = ();
$main::total_files = 0;
$main::total_files_ignored = 0;
$main::total_files_edited = 0;
$main::total_files_renamed = 0;
$main::total_edits = 0;

#------------------------------------------------------------------------------
# Emit an error message and terminate abnormally.
#------------------------------------------------------------------------------
sub fatal {
    my $msg = shift;
    die "ERROR: $msg\n";
}

#------------------------------------------------------------------------------
# Emit a warning message.
#------------------------------------------------------------------------------
sub warning {
    my $msg = shift;
    warn "WARNING: $msg\n";
}

#------------------------------------------------------------------------------
# Print an underlined title.
#------------------------------------------------------------------------------
sub print_title {
    my $msg = shift;
    print "$msg\n", '-' x length($msg), "\n";
}

#------------------------------------------------------------------------------
# Should script be verbose?
#------------------------------------------------------------------------------
sub verbose {
    return $main::verbosity > 0;
}

#------------------------------------------------------------------------------
# Should script be quiet?
#------------------------------------------------------------------------------
sub quiet {
    return $main::verbosity < 0;
}

#------------------------------------------------------------------------------
# Synthesize a subroutine which applies a list of patterns to a string.
# Synthesized subroutines may be configured to return upon the first
# successful match or to apply all patterns in succession (which is useful
# when a series of s/// or tr/// style patterns must be applied).  Synthesized
# subroutines which return after the first successful match return a boolean
# result, whereas subroutines which apply all patterns return the number of
# edits made by the patterns (which can also be viewed as a boolean result).
# Such subroutines also modify the input string in-place on the assumption
# that the caller will want access to the edited string after the patterns
# have been applied.  If the pattern list is empty then the synthesized
# subroutine will return the input value $empty_result.
#------------------------------------------------------------------------------
sub synthesize {
    my ($name, $patterns, $apply_all, $empty_result) = @_;
    my $s = "sub $name " . '{ local $_ = $_[0]; my $n = scalar(';
    if (!@{$patterns}) {
	$s .= $empty_result;
    }
    else {
	my ($pattern, $tests, $op) = ('', '', $apply_all ? '+' : '||');
	foreach $pattern (@{$patterns}) {
	    $tests .= " $op " if $tests;
	    $tests .= "$pattern";
	}
	$s .= $tests;
    }
    $s .= ');';
    $s .= '$_[0] = $_;' if $apply_all; # Return edited string for s///, etc.
    $s .= ' return $n; }';
    eval $s;
    fatal $@ if $@;
}

#------------------------------------------------------------------------------
# Compile all patterns and edit operations for fast matching.
#------------------------------------------------------------------------------
sub compile {
    synthesize('apply_edits',  \@main::opt_edit,   1, 0);
    synthesize('apply_rename', \@main::opt_rename, 1, 0);
    synthesize('match_accept', \@main::opt_accept, 0, 1);
    synthesize('match_ignore', \@main::opt_ignore, 0, 0);
}

#------------------------------------------------------------------------------
# Normalize a pattern.  If it is of the form // then conver it to m//.  If the
# pattern is a raw string, then insert it into m// (under the assumption that
# '/' does not already appear in the pattern).
#------------------------------------------------------------------------------
sub normalize_pattern {
    local $_ = $_[0];
    return if /^m(\W).*\1$/;		# Already in m// form.
    $_ = "/$_/" unless /^(\W).*\1$/;	# Wrap in // if not already in // form.
    $_[0] = "m$_";			# Convert // to m//.
}

#------------------------------------------------------------------------------
# Normalize an edit operation.  Ensure correctness of one of the regular-
# expression operators, s///, tr///, or y///.  If the incoming pattern does
# not begin with one of the operators, 's', 'tr', or 'y', then assume s///.
# If an s/// operation does not end with any modifiers and if $global is set,
# then assume s///g.
#------------------------------------------------------------------------------
sub normalize_edit {
    local $_ = $_[0];
    my $global = $_[1];
    fatal "Invalid pattern matching operation: $_" unless
	/^\W/ || /^s\W/ || /^tr\W/ || /^y\W/;
    s/^(\W)/s$1/;			# Assume s/// for ///.
    s/(\W)$/$1g/ if $global && /^s\W/;	# Assume s///g for s///.
    $_[0] = $_;
}

#------------------------------------------------------------------------------
# Normalize all edit operations and patterns.
#------------------------------------------------------------------------------
sub normalize {
    my $s;
    foreach $s (@main::opt_rename) {
	normalize_edit($s, 0);
    }
    foreach $s (@main::opt_edit) {
	normalize_edit($s, 1);
    }
    foreach $s (@main::opt_accept, @main::opt_ignore) {
	normalize_pattern($s);
    }
}

#------------------------------------------------------------------------------
# Recognize and replace special alias convenience patterns:
#   `old=new' --> `s/\.old$/\.new/' (filename suffix substitution)
#   `old:new' --> `s/\.old$/\.new/' (ditto)
#------------------------------------------------------------------------------
sub process_aliases {
    my $s;
    foreach $s (@main::opt_rename) {
	$s = "s/\\.$1\$/\\.$2/" if $s =~ /^(\w+)[=:](\w+)$/;
    }
}

#------------------------------------------------------------------------------
# Scan a directory for filenames.
#------------------------------------------------------------------------------
sub scandir {
    my $dir = shift;
    opendir(DIR, $dir) or fatal("Directory scan failed ($dir): $!");
    my @files = grep(!/^\.\.?$/, readdir(DIR));
    closedir(DIR);
    return sort { lc($main::a) cmp lc($main::b) } map("$dir/$_", @files);
}

#------------------------------------------------------------------------------
# As a convenience, always descend one level deep into directories explicitly
# mentioned on the command-line even if recursion was not requested.  This
# pre-processing step is skipped if recursion is enabled since the directory
# contents will be examined later on anyhow; otherwise the contents would be
# examined twice.
#------------------------------------------------------------------------------
sub prescan {
    if (!$main::opt_descend) {
	my ($path, @paths) = ('', ());
	foreach $path (@main::paths) {
	    push(@paths, scandir($path)) if -d $path;
	}
	push(@main::paths, @paths);
    }
}

#------------------------------------------------------------------------------
# Read file contents.
#------------------------------------------------------------------------------
sub load_file {
    my $path = shift;
    my $content = '';
    my $size = -s $path;
    if ($size) {
	open(FILE, "<$path") or fatal("Unable to open $path: $!");
	binmode(FILE);
	read(FILE, $content, $size) == $size
	    or fatal("Failed to read all $size bytes of $path: $!");
	close(FILE);
    }
    return $content;
}

#------------------------------------------------------------------------------
# Save file contents.
#------------------------------------------------------------------------------
sub save_file {
    my ($path, $content) = @_;
    open (FILE, ">$path") or fatal("Unable to create $path: $!");
    binmode(FILE);
    print FILE $content if length($content);
    close(FILE);
}

#------------------------------------------------------------------------------
# Rename a file.
#------------------------------------------------------------------------------
sub rename_file {
    my ($oldpath, $newpath) = @_;
    unlink($newpath) if -e $newpath;
    rename($oldpath, $newpath)
	or fatal("Failed to rename $oldpath to $newpath: $!");
}

#------------------------------------------------------------------------------
# Should file be ignored?
#------------------------------------------------------------------------------
sub should_ignore {
    my $path = shift;
    my $name = basename($path);
    my $ignore = !match_accept($name) || match_ignore($name);
    if ($ignore) {
	$main::total_files_ignored++;
	print "Ignored: $path\n" if verbose();
    }
    return $ignore;
}

#------------------------------------------------------------------------------
# Process a path rename operation.
#------------------------------------------------------------------------------
sub process_rename {
    my ($oldpath, $newpath) = @_;
    rename_file($oldpath, $newpath) unless $main::opt_test;
    print "Renamed: $newpath (was ", basename($oldpath), ")\n" if verbose();
}

#------------------------------------------------------------------------------
# Process a single path.
#------------------------------------------------------------------------------
sub process_path {
    my $path = shift;
    $path =~ s:^\./::;	# Strip leading "./".
    $main::total_files++;
    return if should_ignore($path);
    my ($name, $dir) = fileparse($path);
    $dir = '' if $dir =~ m:\./?$:;
    my $newname = $name;
    my $renamed = apply_rename($newname);
    my $newpath = "$dir$newname";
    $main::total_files_renamed++ if $renamed;

    if (-d $path) {
	process_rename($path, $newpath) if $renamed;
    }
    else {
	my ($content, $edits) = (undef, 0);
	if (@main::opt_edit) {
	    $content = load_file($path);
	    $edits = apply_edits($content);
	}
    
	if ($edits) {
	    $main::total_files_edited++;
	    $main::total_edits += $edits;
	    unless ($main::opt_test) {
		my $backuppath = "$path.save";
		rename_file($path, $backuppath) unless $renamed;
		save_file($newpath, $content);
		unlink($renamed ? $path : $backuppath);
	    }
	    print "Changed: $newpath [$edits edits]",
		($renamed ? " (was $name)" : ''), "\n" if verbose();
	}
	elsif ($renamed) {
	    process_rename($path, $newpath);
	}
	else {
	    print "Studied: $newpath (no changes)\n" if verbose();
	}
    }
}

#------------------------------------------------------------------------------
# Process all input paths.
#------------------------------------------------------------------------------
sub process {
    prescan();
    print_title("Processing") if verbose();
    while (@main::paths) {
	my $path = shift @main::paths;
	process_path($path);
	push(@main::paths, scandir($path)) if $main::opt_descend and -d $path;
    }
    print "\n" if verbose();
}

#------------------------------------------------------------------------------
# Display a list of patterns.
#------------------------------------------------------------------------------
sub print_patterns {
    my ($title, $patterns) = @_;
    return unless @{$patterns};
    my $pattern;
    print "$title:";
    foreach $pattern (@{$patterns}) {
	print "\t$pattern\n";
    }
}

#------------------------------------------------------------------------------
# Display summary of requested edits.
#------------------------------------------------------------------------------
sub summarize_options {
    print_title("Option Summary");
    print_patterns("Rename", \@main::opt_rename);
    print_patterns("Edit",   \@main::opt_edit  );
    print_patterns("Accept", \@main::opt_accept);
    print_patterns("Ignore", \@main::opt_ignore);
    print "\n";
}

#------------------------------------------------------------------------------
# Display edit summary.
#------------------------------------------------------------------------------
sub summarize_edits {
    print_title("Edit Summary");
print <<"EOT";
Files Examined: $main::total_files
Files Ignored:  $main::total_files_ignored
Files Renamed:  $main::total_files_renamed
Files Edited:   $main::total_files_edited
Edits Applied:  $main::total_edits
EOT
}

#------------------------------------------------------------------------------
# Display an opening banner.
#------------------------------------------------------------------------------
sub banner {
    my $stream = shift;
    $stream = \*STDOUT unless $stream;
    print $stream "\n$PROG_NAME version $PROG_VERSION\n$COPYRIGHT\n\n";
}

#------------------------------------------------------------------------------
# Display usage statement.
#------------------------------------------------------------------------------
sub print_usage {
    my $stream = shift;
    $stream = \*STDOUT unless $stream;
    banner($stream);
    print $stream <<"EOT";
Applies pattern-based modifications to the content and/or name of a file
or batch of files.  Uses standard Perl regular expression directives for
controlling content modification and rename operations.

Usage: $PROG_NAME [options] <path> ...

<path> Path of the file to be edited.  If path refers to a directory, then
       all files within the directory are considered, subject to the
       constraints of --accept, --ignore, and --descend, as explained below.
       Any number of paths may be specified.

Options:
    -e <pattern>
    --edit=<pattern>
                 Specifies a Perl regular-expression editing operation to
                 apply to each file's content.  May be specified any number of
                 times.  Each pattern will be applied to the file's content in
                 succession.  Pattern should be one of the Perl editing
                 commands s///, tr///, or y///.  The pattern can also be a
                 bare substitution of the form ///, in which case s/// is
                 assumed.  The standard Perl pattern modifiers can be
                 specified following the trailing slash.  If no modifiers are
                 present on the s/// command, then s///g is assumed.
    -r <pattern>
    --rename=<pattern>
                 Specifies a Perl regular-expression used to rename the input
                 file or files.  May be repeated any number of times, though
                 typically one pattern is sufficient.  As with --edit, ///,
                 s///, tr///, or y/// may be used.  The standard pattern
                 modifiers can be specified.  Unlike edit patterns, the 'g'
                 modifier is not automatically assumed for s/// operations.
                 As a convencience, the patterns `old=new' and `old:new' are
                 automatically converted to `s/\\.old\$/\\.new/' to simplify
                 the task of changing a filename's suffix since this is such
                 a common procedure.
    -a <pattern>
    --accept=<pattern>
                 Specifies a Perl regular-expression used as a filename
                 filter.  Filenames which match the pattern will be processed.
                 This is especially useful when processing the contents of
                 entire directories.  The pattern may be specified with //,
                 m//, or as a raw string, in which case it is wrapped in m//.
    -i <pattern>
    --ignore=<pattern>
                 Specifies a Perl regular-expression used as a filename
                 filter.  Filenames which match the pattern will be excluded
                 from processing.  This is especially useful when processing
                 the contents of entire directories.  As with --match, the
                 pattern may be specified with //, m//, or as a raw string.
                 Ignore-patterns take precedence over accept-patterns, thus
                 a filename which is both accepted and ignored will not be
                 processed.
    -d --descend When processing entire directories, this flag will cause the
                 search to recurse into subdirectories as well.  Can be
                 negated with --nodescend.  Default is --nodescend.
    -v --verbose Emit informational messages about the processing.  Can be
                 negated with --noverbose.  Deafult is --noverbose.
    -q --quiet   Suppress all output except for error messages.  Can be
                 negated with --noquiet.  Default is --noquiet.
    -t --test    Perform all edits but do not actually modify any files.  Can
                 be negated with --notest.  Default is --notest.
    -V --version Display the version number of $PROG_NAME.
    -h --help    Display this usage message.
EOT
}

#------------------------------------------------------------------------------
# Process command-line options.
#------------------------------------------------------------------------------
sub process_options {
    GetOptions(@script_options) or usage_error('');
    print_help() if $main::opt_help;
    print_version() if $main::opt_version || $main::opt_V;
    usage_error("Must specify at least one input file.") if @ARGV == 0;
    usage_error("Must specify at least one edit or rename pattern.")
	unless (@main::opt_rename or @main::opt_edit);
    $main::verbosity =  1 if $main::opt_verbose or $main::opt_v;
    $main::verbosity = -1 if $main::opt_quiet;
    @main::paths = @ARGV;
    if (!quiet()) {
	banner();
	print "Non-destructive testing mode enabled.\n\n" if $main::opt_test;
    }
}

sub print_version {
    banner(\*STDOUT);
    exit(0);
}

sub print_help {
    print_usage(\*STDOUT);
    exit(0);
}

sub usage_error {
    my $msg = shift;
    print STDERR "ERROR: $msg\n" if $msg;
    print_usage(\*STDERR);
    exit(1);
}

#------------------------------------------------------------------------------
# Perform the complete repair process.
#------------------------------------------------------------------------------
process_options();
process_aliases();
normalize();
summarize_options() unless quiet();
compile();
process();
summarize_edits() unless quiet();
