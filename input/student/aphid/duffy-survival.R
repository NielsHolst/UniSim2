library(ggplot2)

survival = function(T) {
  94.4 - 3.32e-8*exp(0.726*T)
}

x=seq(0,30,0.5)
M = data.frame(T=x, Survival=survival(x))

ggplot(M, aes(x=T, y=Survival)) +
  geom_line(colour="red", size=2) +
  geom_hline(yintercept=100, size=1.2)
