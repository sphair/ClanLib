         Name: CTalk
       Status: Windows(Y), Linux(Y)
        Level: Intermediate
   Maintainer: Magnus (mbn)
      Summary: Network chat client using GUI

This point of this application is to illustrate how clanGUI and the IRC part
of clanNetwork work. Its a working IRC client which in time, if I dont get
bored, will use some CTCP extensions to add extra features to IRC.






MainFrame
	vector<View*>
	Document*


View
	ChatView
		Chat*
		CL_LineEdit*
		CL_ListView*
	ServerListView
		CL_ListView*
		CL_PushButton*

Chat
	CL_Scrollbar*
	vector<ChatLine*>

ChatLine
	CL_SpanLayout
	CL_String timestamp, nick, text
	CL_Color

Document
	vector<IRCConnection*>


CL_IRCConnection
	IRCConnection




I got 99 problems but a bitch ain't one:

 2# focus not kept on CL_LineEdit
 8# remember geometry for ctalk mainframe
11# nicks leaving (or quit) not noticed on quakenet?
12# smart tab completion on nicks (double tab-clicking, using chat history)
13# /part channel does not close tab
15# sorting the user list alphabetically (and maybe by status?)
17# blinking tray icon [PARTIAL]
20# selection crash bug
22# double click connection to connect
23# Preferences: add/edit/delete connections
24# restore window on tray double click
25# Text selection does not work on multiline text when the line contains a graphic (eg "*")
26# IRC window does not open: Log into #clanlib using /nick my_user .Open up a new ctalk (keeping the old one open)
    Connect to freenode. Type /nick my_user. (nick used error is shown). Type /nick my_user_234. 
    Type /join #clanlib.  Notice the #clanlib window does not appear,  but you are connected in the other ctalk application

 1# not all characters can be typed in CL_LineEdit [FIXED]
 3# highlight tabs when text is written [FIXED]
 4# icon missing for mainframe [FIXED]
 5# you cannot close tabs [FIXED]
 6# copy from chat text is not possible [FIXED]
 7# clicking urls in chat text is not possible [FIXED]
 9# perform list of commands on connect [FIXED]
10# topic of a channel cannot be seen [FIXED]
14# user status changes (op,voice) does not update the user list [FIXED]
16# minimize to tray [FIXED]
18# blinking window on activity [FIXED]
19# scroll wheel in chat view [FIXED]
21# gui-initiated private chats [FIXED]
 
first IRCText searches for the escapes
then for each escape it finds, it removes it from the string and stores the offset the escape is at in the removed string version
removed from string version, that is
so lets say we have ?bold? here
then it would have an Escape being Bold at offset 23 or smt like that
and then it would have another Escape Bold at offset 27
then for the url matching, it would search for urls in the escape cleaned version, and also generate UrlStart and UrlEnd escapes for it
so now we have a clean CL_String with the text and a std::vector<IRCEscape> with the escapes
then you split the clean string into parts for each escape range in the vector
then push/pop the escapes (+bold, -bold, +url, -url) and call span_layout.add_text for each such range with the font and color
dont know if that description was particular good :)
but even just coding the function that scans for IRC escapes (^B, ^U, etc) was too boring to code
so IRCText just returns the string with ? instead