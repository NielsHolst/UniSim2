library(ggplot2)

Isat  = 200.0
alpha = 3.75e-05


Pmax = function(beta) {
  a = alpha*Isat/log(1 + alpha/beta)
  b = alpha/(alpha + beta)
  c = beta/(alpha + beta)
  a*b*c^(beta/alpha)
}


x = 10^(-(14:6))

M = data.frame (
  Beta = x,
  Pmax = Pmax(x)
)

ggplot(M, aes(log10(Beta), Pmax)) +
  geom_line() +
  geom_point() +
  geom_hline(yintercept = 0.0004, colour="red")
  
