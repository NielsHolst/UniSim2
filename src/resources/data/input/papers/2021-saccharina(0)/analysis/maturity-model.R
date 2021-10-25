graphics.off()
setwd("/Users/au152367/Documents/QDev/UniSim2/input/papers/2021-saccharina")

f = function(A) {
  m1 = 1
  m2 = 0
  A0 = 6
  1 - m1*exp(-(A/A0)^2) + m2
}

g = function(A) {
  L = s.5*sqrt(A)
  
}

x = (0:200)/10
M = data.frame(
  Area = x,
  Maturity = f(x),
  Source = "Broch"
)


load("maturity-model-gevaert-sep-c-pct.Rdata")
Gevaert$Area = with(Gevaert, (Length/10/3.5)^2)
Gevaert$Maturity = with(Gevaert, Value-min(Value) + (Value-min(Value))/((max(Value)-min(Value))))
Gevaert$Maturity = Gevaert$Maturity/max(Gevaert$Maturity)

head(Gevaert)
tail(Gevaert)

ggplot(M, aes(Area, Maturity)) +
  geom_line(colour=green) +
  geom_line(colour=blue, data=Gevaert) +
  xlim(0,20) +
  theme_bw()
  