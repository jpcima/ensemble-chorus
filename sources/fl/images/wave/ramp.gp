set term png transparent size 256,256
set output "ramp.png"
set size square
set xrange [0:1]
set yrange [0:+1]
unset key
unset border
unset xtics
unset ytics
plot (x >= 1) ? 0 : x with lines lw 10
