# win case

OBJS =		main.o

LIBS =		-L. -lvar_win

TARGET =	main.exe


all:# (1)
	g++ -O3 -c main.cpp -Wall
	g++ -o $(TARGET) $(OBJS)
	
makeLib:# (2)
	g++ -O3 -c var.cpp -Wall
	ar rcs libvar_win.a var.o
makeWithLib:# (2)
	g++ -O3 -c main.cpp -Wall
	g++ -o $(TARGET) $(OBJS) $(LIBS)
	
# (1) uncomment   ' include"var.cpp" ' at the end of the var.h
# (2) comment out ' include"var.cpp" ' ^