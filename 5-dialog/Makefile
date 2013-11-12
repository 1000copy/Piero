all:t.exe
t.exe:3-testcase.c dialog_comom.c
	g++.exe -Wno-write-strings -mwindows -static 3-testcase.c dialog_comom.c \
	../dict/share/common.c -lws2_32 -lole32 -luuid -lShlwapi -o $@