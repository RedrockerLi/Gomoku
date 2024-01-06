rm main
cd Build
cmake ..
make 
# valgrind --log-file=valReport --leak-check=full --show-reachable=yes --leak-resolution=low --track-origins=yes ./main 