cd ~/Programmi/numera
cp input.txt lastInput.txt
cp ../sfml/gitLudlibrilo/luda-libro/src/libroj/Alroke.txt input.txt
./main
sed -i '1s/^/eo\n/' output.txt
cp output.txt ../sfml/gitLudlibrilo/luda-libro/bin/lin/resources/books/Alroke.txt
cp input2.txt ../sfml/gitLudlibrilo/luda-libro/src/libroj/Alroke.txt
