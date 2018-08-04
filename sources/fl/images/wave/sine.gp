set term png transparent size 256,256
set output "sine.png"
set size square
set xrange [0:2*pi]
set yrange [-1:+1]
unset key
unset border
unset xtics
unset ytics
plot sin(x) with lines lw 10
