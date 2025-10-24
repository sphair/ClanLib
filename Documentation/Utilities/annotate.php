<?php
//-----------------------------------------------------------------------------
// Module to allow users to annotate online documentation.
// Matthias Braun <matze@braunis.de>
//-----------------------------------------------------------------------------

// this needs to be writable by Apache
$basedir="/home/japj/annotations";

$theme=preg_replace("\" \"","_",$theme);
$theme=preg_replace("[^a-zA-Z0-9_]","",$theme);
$file=$basedir."/$theme.xml";
$self=$PHP_SELF;

readXMLFile($file);
print "<hr><a name=\"comments\">\n";

if ($action=="") {
    printEntries();
    print "<a href=\"$self?action=showadd#comments\">Add a comment</a><br>\n";
} elseif ($action == "showadd") {
    print "<form action=\"$self?action=add#comments\" method=\"post\">\n";
    print "<b>Name:</b><input name=\"authorname\" value=\"\"><br>\n";
    print "<b>E-Mail:</b><input name=\"emailname\" value=\"\"><br>\n";
    print "<textarea cols=44 rows=8 name=\"texttext\" wrap=\"virtual\"></textarea><br>\n";
    print "<input type=\"submit\" value=\"Add Comment\"><br>\n";
    print "</form><br>\n";
    printEntries();
} elseif ($action == "add") {
    $newentry = new entry;
    $newentry->author=$authorname;
    $newentry->email=$emailname;
    if ($authorname=="" && $emailname=="")
	die ("<br><h2>Please give name or email!</h2>\n");
    $newentry->date=time();
    $newentry->addr=$REMOTE_ADDR;
    $newentry->agent=$HTTP_USER_AGENT;
    $newentry->text=nl2br($texttext);
    $last =count($entries)-1;
    if (!($newentry->author == $entries[$last]->author &&
	$newentry->email == $entries[$last]->email &&
	$newentry->text == $entries[$last]->text)) {
        $entries[] = $newentry;
        $h= fopen ($file, "w");
        if (!$h) {
		die ("Couldn't write to disk!<br>\n");
        }
        writeXMLFile($h);
        include ("mail.php");
        foreach ($mail as $i) {
	    $emailmangled = str_replace("@", " at ", $emailname);
	    $emailmangled = str_replace(".", " dot ", $emailmangled);
	    mail ($i, "ClanLib documentation annotated by $authorname",
		"Author: $authorname\n".
		"E-Mail: $emailmangled\n".
		"Topic:  $theme\n".
		"File:   $HTTP_REFERER\n".
		"Time:   ".strftime("%a, %d %b %G (%H:%M UTC)",$newentry->date)." (".$newentry->date.")\n".
		"Comment:\n".
		stripslashes($texttext));
        }
        fclose($h);
        chmod($file, 0666);
        print "<h3>Comment added!</h3>\n";
	unset($entries);
	readXMLFile($file);
    }
    printEntries();
    print "<a href=\"$self?action=showadd#comments\">Add a comment</a><br>\n";
}   

class entry
{
    var $author;
    var $email;
    var $text;
    var $date;
    var $addr;
    var $agent;

    function entry()
    {
	$this->author="";
	$this->email="";
	$this->text="";
	$this->addr="";
	$this->agent="";
	$this->data=0;
    }

    function writeE($h)
    {
	if ($this->author=="" && $this->email=="") {
		return;
	}

	fputs ($h, "<comment>\n");
	fputs ($h, "<author>".$this->author."</author>\n");
	fputs ($h, "<email>".$this->email."</email>\n");
	fputs ($h, "<date>".$this->date."</date>\n");
	fputs ($h, "<ipaddr>".$this->addr."</ipaddr>\n");
	fputs ($h, "<agent>".$this->agent."</agent>\n");
	fputs ($h, "<?text ".$this->text ."?>\n");
	fputs ($h, "</comment>\n");
    }
}

function printEntries()
{
    global $entries;
    print "<br><br>\n";

    foreach ($entries as $e) {
	print "<table width=\"100%\" bgcolor=\"#88bbff\" celspacing=\"0\" celpadding=\"0\">\n";
	print "<tr><td>\n";
	if ($e->author != "") {
		print $e->author;
	}
	if ($e->email != "") {
		$mail = str_replace("@","-<font color=red>at</font>-",$e->email);
		$mail = str_replace(".","-<font color=red>dot</font>-",$mail);
		if ($e->author != "")
			print " ($mail)";
		else
			print $mail;
	}
	print "</td>\n";

	print "<td align=\"right\"><b>\n";
	if ($e->date != 0)
	{
	    print strftime("%a, %d %b %G (%H:%M UTC)", $e->date) . "<br>\n";
	}
	print "</b></td></tr>\n";
	print "</table>\n";
	print "<table width=\"100%\" bgcolor=\"#eeeeee\"><tr><td>";
	print stripslashes($e->text);
	print "</td></tr></table>\n";
	print "<br>\n";
    }
}

function readXMLFile($file)
{
    global $status;
    global $entries;

    if (!file_exists($file)) {
	$newe = new entry;
	$newe->text="<h2>No user comments yet!</h2>\n";
	$entries[] = $newe;
	return;
    }

    $xml_parser = xml_parser_create();
    xml_set_element_handler($xml_parser, "startElement", "endElement");
    xml_set_character_data_handler($xml_parser, "characterData");
    xml_set_processing_instruction_handler($xml_parser, "piHandler");
    if (!($fp = fopen($file, "r"))) {
        die ("could not open XML input");
    }
    
    $status="global";
    
    while ($data = fread($fp, 4096)) {
        if (!xml_parse($xml_parser, $data, feof($fp))) {
	    die(sprintf("XML error: %s at line %d",
    		xml_error_string(xml_get_error_code($xml_parser)),
    		xml_get_current_line_number($xml_parser)));
        }
    }
    xml_parser_free($xml_parser);
}

function writeXMLFile($h)
{
    global $entries;

    fputs ($h, "<?xml version=\"1.0\"?>\n");
    fputs ($h, "<comments>\n");
    foreach ($entries as $e) {
	$e->writeE($h);
    }

    fputs ($h, "</comments>\n");
}

function startElement($parser, $name, $attrs) {
    global $status;
    global $aentry;

    $xmlglobdata="";
    switch ($status) {
	case "global":
	   if ($name == "COMMENT") {
		$status="comment";
		$aentry=new entry();
	    }
	    
	    break;
	case "comment":
	    if ($name == "AUTHOR") {
		$status="author";
	    }
	    if ($name == "EMAIL") {
		$status="email";
	    }
	    if ($name == "DATE") {
		$status="date";
	    }
	    if ($name == "IPADDR") {
		$status ="ipaddr";
	    }
	    if ($name == "AGENT") {
		$status = "agent";
	    }
	    if ($name == "TEXT") {
		$status="text";
	    }
	    break;
    }
}

function endElement($parser, $name) {
    global $status;
    global $aentry;
    global $xmlglobdata;
    global $entries;

    switch ($status) {
	case "comment":
	    if ($name == "COMMENT") {
		$status="global";
		$entries[] = $aentry;
	    }
	    break;
	case "author":
	    $aentry->author = $xmlglobdata;
	    $status="comment";
	    break;
	case "email":
	    $aentry->email = $xmlglobdata;
	    $status="comment";
	    break;
	case "date":
	    $aentry->date = (int)$xmlglobdata;
	    $status="comment";
	    break;
	case "text":
	    if ($name != "TEXT") { break; }
	    $aentry->text = $xmlglobdata;
	    $status="comment";
	    break;
	case "ipaddr":
	    $aentry->addr = $xmlglobaldata;
	    $status="comment";
	    break;
	case "agent":
	    $anetry->agent = $xmlglobaldata;
	    $status="comment";
	    break;
    }
}

function characterData($parser, $data) {
    global $xmlglobdata;
    if ($data=="\n")
	$data="";
    $xmlglobdata=$data;
}

function piHandler($parser, $name, $data) {
    global $status;
    global $aentry;
    
    if ($status=="comment" && $name=="text") {
	$aentry->text=$data;
    }
}

?>
