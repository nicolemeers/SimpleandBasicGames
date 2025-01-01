# SimpleandBasicGames
I'm just tinkering around with stuff and putting it up here.

## How to run
There are several main files to run each game on their own or to just use the "menu manager" to select each respective game.
I wrote this in Visual Studio 2022 on windows (which is important for some of vcpkg instructions) and then figured out how to compile it with GCC on windows (via mingw).

### For MenuManager and Hangman
Hangman uses libcurl, which can be included, but needs to be manually installed. The way to do this is with vcpkg.
You can follow [this link here](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell#1---set-up-vcpkg) for specific instructions.
But for brevity, I'll post the instructions here:
1. clone the repository: 
---
    git clone https://github.com/microsoft/vcpkg.git
---
3. Navigate to the vcpkg directory and run the bat script (you can run the executable itself, I think, instead of on commandline. I had to mess around with it for a moment since this line didn't work for me but I don't remember what I did): 
---
    cd vcpkg; .\bootstrap-vcpkg.bat
---
4. Now you have set the environment variables so that visual studio/your current terminal knows where to find it (you will have to set these for every new session, unless you update your system environment variables): 
---
   	$env:VCPKG_ROOT = "C:\path\to\vcpkg" \
   	$env:PATH = "$env:VCPKG_ROOT;$env:PATH"

5. Now run the install command to add libcurl: 
---
	vcpkg install curl:x64-windows
---
6. Update the build installation (For visual studio specifically): 
---
	vcpkg integrate install
---
7. If in visual studio, you will need to restart it
<br/>
<br/>

### Run TicTacToeGame
run in terminal: \
g++ mainTicTacToe.cpp src/Player.cpp src/Board.cpp src/TicTacToeGame.cpp -o TicTacToeGame
.\TicTacToeGame

### Run Hangman Game
run in terminal: \
g++ mainHangman.cpp src/Hangman.cpp -IC:\dev\vcpkg\vcpkg\packages\curl_x64-windows\include -LC:\<path_to_vcpkg>\vcpkg\packages\curl_x64-windows\lib -LC:\dev\vcpkg\vcpkg\packages\curl_x64-windows\bin -llibcurl -o HangmanGame
.\HangmanGame
________________
Some notes on the flags: \
-I<include_path> adds the include directorys needed to include the "curl/libcurl.h" into the c++ files. \
-L</path/...bin> adds the library directory for libcurl.dll \
-L</path/...lib> adds the library directory for libcurl.lib \
*NOTE* \
It is important to manually add the libcurl.dll file into the same directory as the executable. If it is only in the linker directory the executable will complain that it cannot find the libcurl.dll file. \
I tried to use -I<libcurl.dll path> but that also produced the same error.
________________

### Run MenuManager
run in terminal: \
g++ main.cpp src/Hangman.cpp src/MenuManager.cpp src/Player.cpp src/Board.cpp src/TicTacToeGame.cpp -IC:\dev\vcpkg\vcpkg\packages\curl_x64-windows\include -LC:\dev\vcpkg\vcpkg\packages\curl_x64-windows\lib -LC:\dev\vcpkg\vcpkg\packages\curl_x64-windows\bin -llibcurl -o BasicGames
.\BasicGames
