library(ggplot2)

U = function(windSpeed) {
  y = windSpeed^0.8/4.^0.8
  ix = (windSpeed <= 4) 
  y[ix] = (2.8 + 1.2*windSpeed[ix])/7.6
  y
}

x = 0:20
M = data.frame(
  WindSpeed = x,
  Uscaling = U(x)
)

ggplot(M, aes(x=WindSpeed, y=Uscaling)) +
  geom_line()
  
  
U(0)
U(4)

