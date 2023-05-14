# Rolando Rosales 1001850424 - Project 2 Assignment 3 Autorun Script

for num_threads in 2 4 8
do
    for K in 100 200 400 800
    do
        echo "Running with num_threads = $num_threads and K = $K"
        gcc "3320_project2-assignment3-$1.c" -o 3320_project2-assignment3-$1 -D K=$K -lpthread && ./3320_project2-assignment3-$1 $num_threads
    done
done
