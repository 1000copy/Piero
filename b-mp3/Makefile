_lib=-lws2_32 -lole32 -luuid -lShlwapi -lcomctl32 

all:k.exe
k.exe: hello.cpp hello.res logger.c u_shortcut.cpp u_robot.cpp u_string.cpp u_win.h u_dialog.h u_dialog.cpp 
	g++.exe -Wno-write-strings -mwindows  -static  hello.cpp hello.res logger.c u_shortcut.cpp u_robot.cpp \
		u_string.cpp u_dialog.cpp  $(_lib) -o $@
clean:
	rm piero.txt &	rm *.exe & rm *.res
	