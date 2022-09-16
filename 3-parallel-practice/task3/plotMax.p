set term png
set output "diagramMax.png"
set title "Max time among processes"

set xlabel "Processes"
set ylabel "Time"

plot 'max.csv' with linespoints