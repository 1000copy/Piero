all:t.exe nt.exe
t.exe:3-testcase.c dialog_comom.c
	g++.exe -Wno-write-strings -mwindows -static 3-testcase.c dialog_comom.c \
	 -lws2_32 -lole32 -luuid -lShlwapi -o $@
nt.exe:4-nongta.cpp dialog_comom.c
	g++.exe -Wno-write-strings -mwindows -static 4-nongta.cpp dialog_comom.c -o $@