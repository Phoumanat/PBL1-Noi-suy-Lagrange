set title "Spline Interpolation"
set xlabel "X-axis"
set ylabel "Y-axis"
set grid

plot "input.txt" using 1:2 with points pt 7 ps 1.5 lc "red" title "Data Points", \
     "input.txt" using 1:2 smooth cspline with lines lc "blue" title "Spline Curve"
pause -1