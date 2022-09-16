set term png
set output "total.png"
set title "Total time"

set xlabel "Threads"
set ylabel "Time"

plot 'total.csv' with linespoints