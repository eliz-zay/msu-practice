set term png
set output "diagram.png"
set title "OpenMP multiplication"

set datafile separator "\t"

set hidden3d
set xlabel "Threads" offset -5,0,0
set ylabel "Matrix size" offset -2,-3,0
set zlabel "Time"

set ytics 0,400
set ztics 0,10

splot 'data.csv' with linespoints