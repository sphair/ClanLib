         Name: ExePath
       Status: Windows(Y), Linux(Y)
        Level: Beginner
   Maintainer: Ingo Ruhnke <grumbel@gmx.de>
      Summary: How to get the executable path of the binary

This example demonstrates how to get the executable path of the
binary, this is important since programs have to locate their data
files somehow and the current directory is not sufficient for doing
at all times. GNU/Linux programs when installed are mostly started
from the PATH, thus the executable doesn't get a clue on where the
data files might be located (/usr, /usr/local, ...) since the current
working directory can be anything.

With CL_System::get_exe_path() however one can get the location of the
binary, for example if the binary is in /usr/bin/your_binary, the
function would return /usr/bin/. By appending ../share/games/your_game/
you could now find the location of your data without resorting to ugly
workarounds like hard-compiled path names in your binary or wrapper scripts.
