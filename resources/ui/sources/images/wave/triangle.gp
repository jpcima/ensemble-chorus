set term png transparent size 256,256
set output "triangle.png"
set size square
set xrange [0:1]
set yrange [0:+1]
unset key
unset border
unset xtics
unset ytics
plot (x < 0.5) ? (2 * x) : (1 - 2 * (x - 0.5)) with lines lw 10
