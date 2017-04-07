set terminal "png"
set output "plot.png"
plot "fix-sized-time.txt" using 0:1 title "naive" with lines, \
     "fix-sized-time.txt" using 0:2 title "row major" with lines, \
     "fix-sized-time.txt" using 0:3 title "avx row major" with lines
