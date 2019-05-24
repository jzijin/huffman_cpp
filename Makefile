objs = main.o huffman.o
huffman : $(objs)
	g++ -o huffman $(objs)
main.o : main.cpp huffman.h
	g++ -c main.cpp
huffman.o : huffman.cpp huffman.h
	g++ -c huffman.cpp

clean:
	rm -rf $(objs) huffman
