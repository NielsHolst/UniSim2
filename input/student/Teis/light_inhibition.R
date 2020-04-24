library(ggplot2)
library(gridExtra)

Pmax_target = 0.001
Isat = 200
alpha = 3.75e-5

Pmax = function(beta) {
  a = alpha*Isat/log(1 + alpha/beta)
  b = alpha/(alpha+beta)
  c = beta/(alpha+beta)
  a*b*c^(beta/alpha)
}

error = function(beta) {
  abs(Pmax_target - Pmax(beta))
}

x = 10^((-160:-20)/10)

M = data.frame(
  Log10Beta = log10(x),
  Pmax = Pmax(x),
  Error = error(x)
)

grid.arrange(
  ggplot(M, aes(Log10Beta, Pmax)) +
    geom_hline(yintercept=Pmax_target, colour="orange", size=1) +
    geom_line(colour="blue") +
    geom_point(colour="forestgreen")
  ,
  ggplot(M, aes(Log10Beta, Error)) +
    geom_hline(yintercept=0, colour="orange", size=1) +
    geom_line(colour="blue") +
    geom_point(colour="red")
)

Pmax(1e-8)
Pmax(2.31308e-08)
