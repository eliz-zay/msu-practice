set term png
set output "diagramTotal.png"
set title "Total time"

set xlabel "Processes"
set ylabel "Time"

plot 'total.csv' with linespoints