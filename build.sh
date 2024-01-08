rm main
touch debugLog
touch input
touch trainLog
cd Build
cmake ..
make 
# valgrind --log-file=valReport --leak-check=full --show-reachable=yes --leak-resolution=low --track-origins=yes ./main 