set term png transparent size 256,256
set output "square.png"
set size square
set xrange [0:1]
set yrange [-1:+1]
unset key
unset border
unset xtics
unset ytics
plot (x <= 0) ? 0 : (x >= 1) ? 0 : (x < 0.5) ? -1 : +1 with lines lw 10
