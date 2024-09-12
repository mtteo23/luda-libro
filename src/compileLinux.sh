cp Linux.h specific.h
g++ -c main.cpp
g++ main.o -o ../bin/lin/main -lsfml-graphics -lsfml-window -lsfml-system -lX11
rm main.o
rm specific.h
cd ../bin/lin/
./main
