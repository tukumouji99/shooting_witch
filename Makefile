shooting.exe: gamemain.o ImageData.o ImageIO.o TextureImage.o GenericTimer.o gamefunctions.o
	g++ gamemain.o ImageData.o ImageIO.o TextureImage.o GenericTimer.o gamefunctions.o -o shooting.exe -lfreeglut -lglu32 -lopengl32

.c.o:
	g++ -c $< -I. -Wall 

# gamemain.o: ImageData.h ImageIO.h TextureImage.h GenericTimer.h gamefunctions.h

run:
	./shooting.exe

clean:
	rm -rf *.o