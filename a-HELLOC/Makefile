_lib=-lws2_32 -lole32 -luuid -lShlwapi -lcomctl32 

all:bigbang.exe t.exe bbi_correct.exe bbi_bang.exe msgbox_doesnt_appear.exe why_deletefile_failure.exe
hello.res:hello.rc
	windres hello.rc -O coff -o hello.res
t.exe: hello.cpp hello.res u_win.cpp logger.c u_shortcut.cpp u_robot.cpp
	g++.exe -Wno-write-strings -mwindows  -static  hello.cpp hello.res logger.c u_shortcut.cpp u_robot.cpp $(_lib) -o $@
bigbang.exe: big_bang.cpp
	g++.exe -Wno-write-strings -mconsole  -static  big_bang.cpp -o $@
bbi_correct.exe: big_bang_inplace.cpp logger.c
	g++.exe -Wno-write-strings -mwindows  -static  big_bang_inplace.cpp logger.c -o $@
bbi_bang.exe: big_bang_inplace.cpp logger.c
	g++.exe -DBANG_NOW -Wno-write-strings -mwindows  -static  big_bang_inplace.cpp logger.c -o $@
msgbox_doesnt_appear.exe:	msgbox_doesnt_appear.c 
	g++ msgbox_doesnt_appear.c -mwindows -o  $@
why_deletefile_failure.exe:	why_deletefile_failure.c  logger.c
	g++ -Wno-write-strings why_deletefile_failure.c -mwindows logger.c -o  $@
clean:
	rm piero.txt
	rm *.exe
	rm *.res
	