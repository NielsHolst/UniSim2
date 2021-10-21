library(ggplot2)

x = 0:10
a0 = 2
a1 = 6
gmin = 0.010
gmax = 0.033
f = function(x) {
  slope = (gmin-gmax)/(a1-a0)
  intercept = gmax - a0*slope
  y = slope*x + intercept
  y[y<gmin] = gmin
  y[y>gmax] = gmax
  y
}


M = data.frame(
  Area = x,
  g = f(x)
)

ggplot(M, aes(Area, g)) +
  geom_vline(xintercept=c(a0,a1), colour="blue") +
  geom_line(colour="red") +
  geom_point() +
  scale_x_continuous(breaks=0:10) +
  scale_y_continuous(breaks=c((0:6)*0.005, gmax))
