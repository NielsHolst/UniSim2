library(ggplot2)

slope = 8
a50 = 6

maturity = function(area) {
  1/(1. + exp(-slope*(log(area) - log(a50))))
}

x = (0:120)/10

M = data.frame(
  Area = x,
  Maturity = maturity(x)
)

ggplot(M, aes(Area, Maturity)) +
  geom_line(colour=red) +
  labs(x="Area (dm2)") +
  geom_vline(xintercept=a50, linetype="dashed") +
  geom_hline(yintercept=0.5, linetype="dashed") +
  scale_x_continuous(breaks=2*(0:6)) +
  theme_bw()
  