all: clanSignals clanCore clanApp clanNetwork clanDisplay clanGL clanSDL clanSound clanMikMod clanVorbis clanGUI clanGUIStyleSilver ClanLib
	@echo Build complete!

clanSignals : 
	@install -d Frameworks/clanSignals.framework/Versions/0.8.0
	@install -d Frameworks/clanSignals.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanSignals.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanSignals.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanSignals.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanSignals.framework/Resources
	@install clanSignals-Info.plist Frameworks/clanSignals.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanSignals.framework/Versions/0.8.0/clanSignals -o clanSignals -FFrameworks  
	@install -d Frameworks/clanSignals.framework/Versions/0.8.0
	install clanSignals Frameworks/clanSignals.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanSignals Frameworks/clanSignals.framework/clanSignals

clanCore : clanSignals 
	@install -d Frameworks/clanCore.framework/Versions/0.8.0
	@install -d Frameworks/clanCore.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanCore.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanCore.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanCore.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanCore.framework/Resources
	@install clanCore-Info.plist Frameworks/clanCore.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanCore.framework/Versions/0.8.0/clanCore -o clanCore -FFrameworks -framework clanSignals -lz -lpthread -framework Carbon 
	@install -d Frameworks/clanCore.framework/Versions/0.8.0
	install clanCore Frameworks/clanCore.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanCore Frameworks/clanCore.framework/clanCore

clanApp : clanCore clanSignals 
	@install -d Frameworks/clanApp.framework/Versions/0.8.0
	@install -d Frameworks/clanApp.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanApp.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanApp.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanApp.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanApp.framework/Resources
	@install clanApp-Info.plist Frameworks/clanApp.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanApp.framework/Versions/0.8.0/clanApp -o clanApp -FFrameworks -framework clanCore -framework clanSignals  
	@install -d Frameworks/clanApp.framework/Versions/0.8.0
	install clanApp Frameworks/clanApp.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanApp Frameworks/clanApp.framework/clanApp

clanNetwork : clanSignals clanCore 
	@install -d Frameworks/clanNetwork.framework/Versions/0.8.0
	@install -d Frameworks/clanNetwork.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanNetwork.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanNetwork.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanNetwork.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanNetwork.framework/Resources
	@install clanNetwork-Info.plist Frameworks/clanNetwork.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanNetwork.framework/Versions/0.8.0/clanNetwork -o clanNetwork -FFrameworks -framework clanSignals -framework clanCore  
	@install -d Frameworks/clanNetwork.framework/Versions/0.8.0
	install clanNetwork Frameworks/clanNetwork.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanNetwork Frameworks/clanNetwork.framework/clanNetwork

clanDisplay : clanSignals clanCore 
	@install -d Frameworks/clanDisplay.framework/Versions/0.8.0
	@install -d Frameworks/clanDisplay.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanDisplay.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanDisplay.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanDisplay.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanDisplay.framework/Resources
	@install clanDisplay-Info.plist Frameworks/clanDisplay.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanDisplay.framework/Versions/0.8.0/clanDisplay -o clanDisplay -FFrameworks -framework clanSignals -framework clanCore -lpng -ljpeg 
	@install -d Frameworks/clanDisplay.framework/Versions/0.8.0
	install clanDisplay Frameworks/clanDisplay.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanDisplay Frameworks/clanDisplay.framework/clanDisplay

clanGL : clanSignals clanCore clanDisplay 
	@install -d Frameworks/clanGL.framework/Versions/0.8.0
	@install -d Frameworks/clanGL.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanGL.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanGL.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanGL.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanGL.framework/Resources
	@install clanGL-Info.plist Frameworks/clanGL.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanGL.framework/Versions/0.8.0/clanGL -o clanGL -FFrameworks -framework clanSignals -framework clanCore -framework clanDisplay -framework OpenGL -framework AGL -framework Carbon 
	@install -d Frameworks/clanGL.framework/Versions/0.8.0
	install clanGL Frameworks/clanGL.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanGL Frameworks/clanGL.framework/clanGL

clanSDL : clanSignals clanCore clanDisplay 
	@install -d Frameworks/clanSDL.framework/Versions/0.8.0
	@install -d Frameworks/clanSDL.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanSDL.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanSDL.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanSDL.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanSDL.framework/Resources
	@install clanSDL-Info.plist Frameworks/clanSDL.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanSDL.framework/Versions/0.8.0/clanSDL -o clanSDL -FFrameworks -framework clanSignals -framework clanCore -framework clanDisplay -lsdl -lobjc 
	@install -d Frameworks/clanSDL.framework/Versions/0.8.0
	install clanSDL Frameworks/clanSDL.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanSDL Frameworks/clanSDL.framework/clanSDL

clanSound : clanSignals clanCore 
	@install -d Frameworks/clanSound.framework/Versions/0.8.0
	@install -d Frameworks/clanSound.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanSound.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanSound.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanSound.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanSound.framework/Resources
	@install clanSound-Info.plist Frameworks/clanSound.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanSound.framework/Versions/0.8.0/clanSound -o clanSound -FFrameworks -framework clanSignals -framework clanCore -framework CoreAudio 
	@install -d Frameworks/clanSound.framework/Versions/0.8.0
	install clanSound Frameworks/clanSound.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanSound Frameworks/clanSound.framework/clanSound

clanMikMod : clanSignals clanCore clanSound 
	@install -d Frameworks/clanMikMod.framework/Versions/0.8.0
	@install -d Frameworks/clanMikMod.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanMikMod.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanMikMod.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanMikMod.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanMikMod.framework/Resources
	@install clanMikMod-Info.plist Frameworks/clanMikMod.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanMikMod.framework/Versions/0.8.0/clanMikMod -o clanMikMod -FFrameworks -framework clanSignals -framework clanCore -framework clanSound -lmikmod 
	@install -d Frameworks/clanMikMod.framework/Versions/0.8.0
	install clanMikMod Frameworks/clanMikMod.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanMikMod Frameworks/clanMikMod.framework/clanMikMod

clanVorbis : clanSignals clanCore clanSound 
	@install -d Frameworks/clanVorbis.framework/Versions/0.8.0
	@install -d Frameworks/clanVorbis.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanVorbis.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanVorbis.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanVorbis.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanVorbis.framework/Resources
	@install clanVorbis-Info.plist Frameworks/clanVorbis.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanVorbis.framework/Versions/0.8.0/clanVorbis -o clanVorbis -FFrameworks -framework clanSignals -framework clanCore -framework clanSound -logg -lvorbis 
	@install -d Frameworks/clanVorbis.framework/Versions/0.8.0
	install clanVorbis Frameworks/clanVorbis.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanVorbis Frameworks/clanVorbis.framework/clanVorbis

clanGUI : clanSignals clanCore clanDisplay 
	@install -d Frameworks/clanGUI.framework/Versions/0.8.0
	@install -d Frameworks/clanGUI.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanGUI.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanGUI.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanGUI.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanGUI.framework/Resources
	@install clanGUI-Info.plist Frameworks/clanGUI.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanGUI.framework/Versions/0.8.0/clanGUI -o clanGUI -FFrameworks -framework clanSignals -framework clanCore -framework clanDisplay  
	@install -d Frameworks/clanGUI.framework/Versions/0.8.0
	install clanGUI Frameworks/clanGUI.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanGUI Frameworks/clanGUI.framework/clanGUI

clanGUIStyleSilver : clanSignals clanCore clanDisplay clanGUI 
	@install -d Frameworks/clanGUIStyleSilver.framework/Versions/0.8.0
	@install -d Frameworks/clanGUIStyleSilver.framework/Versions/0.8.0/Headers
	@install -d Frameworks/clanGUIStyleSilver.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/clanGUIStyleSilver.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/clanGUIStyleSilver.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/clanGUIStyleSilver.framework/Resources
	@install clanGUIStyleSilver-Info.plist Frameworks/clanGUIStyleSilver.framework/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name clanGUIStyleSilver.framework/Versions/0.8.0/clanGUIStyleSilver -o clanGUIStyleSilver -FFrameworks -framework clanSignals -framework clanCore -framework clanDisplay -framework clanGUI  
	@install -d Frameworks/clanGUIStyleSilver.framework/Versions/0.8.0
	install clanGUIStyleSilver Frameworks/clanGUIStyleSilver.framework/Versions/0.8.0
	@ln -s -f Versions/Current/clanGUIStyleSilver Frameworks/clanGUIStyleSilver.framework/clanGUIStyleSilver

ClanLib : Setup/OSX/clanlib.cpp
	@install -d Frameworks/ClanLib.framework/Versions/0.8.0
	@install -d Frameworks/ClanLib.framework/Versions/0.8.0/Headers
	@install -d Frameworks/ClanLib.framework/Versions/0.8.0/Resources
	@ln -s -f 0.8.0 Frameworks/ClanLib.framework/Versions/Current
	@ln -s -f Versions/Current/Headers Frameworks/ClanLib.framework/Headers
	@ln -s -f Versions/Current/Resources Frameworks/ClanLib.framework/Resources
	@install ClanLib-Info.plist Frameworks/ClanLib.framework/Versions/0.8.0/Resources/Info.plist
	g++ -dynamiclib -compatibility_version 1 -current_version 1 -install_name ClanLib.framework/Versions/0.8.0/ClanLib -o ClanLib -FFrameworks Setup/OSX/clanlib.cpp
	@install -d Frameworks/ClanLib.framework/Versions/0.8.0
	install ClanLib Frameworks/ClanLib.framework/Versions/0.8.0
	@ln -s -f Versions/Current/ClanLib Frameworks/ClanLib.framework/ClanLib

