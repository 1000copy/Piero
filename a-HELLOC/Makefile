_lib=-lws2_32 -lole32 -luuid -lShlwapi -lcomctl32 

all:k.exe
mutex.exe:mutex.cpp
	g++ -mwindows mutex.cpp -o mutex.exe -static
hello.res:hello.rc
	windres hello.rc -O coff -o hello.res
k.exe: hello.cpp hello.res logger.c u_shortcut.cpp u_robot.cpp u_string.cpp u_win.h u_dialog.h u_dialog.cpp 
	g++.exe -Wno-write-strings -mwindows  -static  hello.cpp hello.res logger.c u_shortcut.cpp u_robot.cpp \
		u_string.cpp u_dialog.cpp $(_lib) -o $@
bbi_correct.exe: big_bang_inplace.cpp logger.c
	g++.exe -Wno-write-strings -mwindows  -static  big_bang_inplace.cpp logger.c -o $@
bbi_bang.exe: big_bang_inplace.cpp logger.c
	g++.exe -DBANG_NOW -Wno-write-strings -mwindows  -static  big_bang_inplace.cpp logger.c -o $@
msgbox_doesnt_appear.exe:	msgbox_doesnt_appear.c 
	g++ msgbox_doesnt_appear.c -mwindows -o  $@
why_deletefile_failure.exe:	why_deletefile_failure.c  logger.c u_string.cpp
	g++ -Wno-write-strings why_deletefile_failure.c -mconsole logger.c -o  $@
clean:
	rm piero.txt &	rm *.exe & rm *.res
	