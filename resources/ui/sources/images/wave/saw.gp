set term png transparent size 256,256
set output "saw.png"
set size square
set xrange [0:1]
set yrange [0:+1]
unset key
unset border
unset xtics
unset ytics
plot (x <= 0) ? 0 : (1 - x) with lines lw 10
