set term png
set output "diagram.png"
set title "MPI multiplication"

set datafile separator "\t"

set hidden3d
set xlabel "Processes" offset -5,0,0
set ylabel "Matrix size" offset -2,-3,0
set zlabel "Time"

set ytics 0,400

splot 'dataMPI.csv' with linespoints