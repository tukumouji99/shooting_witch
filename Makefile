shooting.exe: gamemain.o
	g++ gamemain.o -o shooting.exe -lfreeglut -lglu32 -lopengl32

.c.o:
	g++ -c $<

run:
	./shooting.exe

clean:
	rm -rf *.o