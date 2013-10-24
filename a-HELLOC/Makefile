all:bigbang.exe t.exe bbi_correct.exe bbi_bang.exe
hello.res:hello.rc
	windres hello.rc -O coff -o hello.res
t.exe: hello.cpp hello.res u_win.cpp logger.c
	g++.exe -Wno-write-strings -mwindows  -static  hello.cpp hello.res logger.c -o $@
bigbang.exe: big_bang.cpp
	g++.exe -Wno-write-strings -mconsole  -static  big_bang.cpp -o $@
bbi_correct.exe: big_bang_inplace.cpp logger.c
	g++.exe -Wno-write-strings -mwindows  -static  big_bang_inplace.cpp logger.c -o $@
bbi_bang.exe: big_bang_inplace.cpp logger.c
	g++.exe -DBANG_NOW -Wno-write-strings -mwindows  -static  big_bang_inplace.cpp logger.c -o $@
clean:
	rm piero.txt
	rm *.exe
	rm *.res
	