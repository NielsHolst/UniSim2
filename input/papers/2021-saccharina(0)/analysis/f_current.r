graphics.off()

library(ggplot2)


x = (0:40)/100
uMin = 0.28
U63 = 0.045

f = function(x) {
  (1 - uMin)*(1. - exp(-x/U63)) + uMin;
}
f(U63)


M = data.frame(
  Current = x,
  fCurrent = f(x)
)

ggplot(M, aes(Current, fCurrent)) +
  geom_line() +
  theme_bw()
  
