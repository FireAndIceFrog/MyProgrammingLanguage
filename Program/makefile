Main.exe	: Main.o 
	g++ -O2 -std=c++11 -o Main.exe Main.o -l gdi32 -static-libgcc -static-libstdc++
			
Main.o	: Main.cpp symbolTable.cpp 
	g++ -O2 -std=c++11 -c -Wno-write-strings Main.cpp symbolTable.cpp 
	
clean:
	del *.o
	

