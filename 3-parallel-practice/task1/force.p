set term png
set output "diagram.png"
set xtics ("ijk" 0, "ikj" 1, "kij" 2, "jik" 3, "jki" 4, "kji" 5)
plot 'data.csv' with lines