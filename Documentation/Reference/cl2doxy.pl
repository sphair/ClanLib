#!/usr/bin/perl

use English;

$detailed_type = "";

open (FILE, $ARGV[0]) || die;
while ($line = <FILE>)
{
	if ($line =~ /\/\/([a-zA-Z0-9-].*)\n$/)
	{
		# We are in a documentation block.
		$doc_text = $1;

		if ($doc_text =~ /^- (.*)$/)
		{
			$doc_text = "$1";
		}
		elsif ($doc_text =~ /^param (.*): (.*)$/)
		{
			$doc_text = "\\param $1 $2";
		}
		elsif ($doc_text =~ /^return: (.*)$/)
		{
			$doc_text = "\\return $1";
		}
		
		if ($documentation_block)
		{
			print "\n";
			$line =~ s/\/\/[a-zA-Z0-9:-].*\n$/    $doc_text/;
		}
		else
		{
			$line =~ s/\/\/[a-zA-Z0-9:-].*\n$/\/** $doc_text/;
			$documentation_block = 1;
		}
	}
	elsif ($documentation_block)
	{
		# End of doc block.
		print "*/\n";
		$documentation_block = 0;
	}
	
	$line =~ s/\/\/:(.*$)/\/** \\brief $1*\//;

	if ($line =~ /\/\/! (clan.*?)="(.*)"$/)
	{
		$groupname = "$1_$2";
		$groupdescription = "$1 $2";
		$groupname =~ s/[ :\/]/_/g;
		
		$line =~ s/\/\/! clan.*?=".*"($)/\/** \\addtogroup $groupname $groupdescription *\/$1/;
		$line .= "//\@{\n";
		$in_module = 1;
	}
	$line =~ s/\/\/! (header.*)($)/\/** $1 *\/$2/;
	if ($ingroup == 1)
	{
		$ingroup = 0 if ($line =~ s/^(}.*$)/\/\/\@}\n$1/);
		$line =~ s/\/\/! (.*?):(.*$)/\n\/\/\@}\n\/** \\name $1 *\/$2\n\/\/\@{\n/;
	}
	elsif ($line =~ s/\/\/! (.*?):(.*$)/\/** \\name $1 *\/$2\n\/\/\@{\n/)
	{
		$ingroup=1;
	}
	
	print $line;
}
if ($in_module)
{
	print "//\@}\n";
}
close (FILE);
