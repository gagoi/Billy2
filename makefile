INCD=../Libraries \
	D:\Programs\mingw64\lib\gcc\x86_64-w64-mingw32\7.1.0\include\c++ \
	D:\Programs\mingw64\include
INCD_P=$(foreach d, $(INCD), -I$d)

INC=mingw32
INC_P=$(foreach i, $(INC), -l$i)

LIBD=\
	../SDL2\lib
LIBD_P=$(foreach i, $(LIBD), -L$i)

CPP=main.cpp

main: $(CPP)
	g++ -g $(CPP) -o main $(LIBD_P) $(INCD_P) $(INC_P)
	./main.exe