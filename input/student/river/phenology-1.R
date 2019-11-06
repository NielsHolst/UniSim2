library(devRate)
library(ggplot2)

#data for larva development
Temperature <- c(18, 22, 24, 26, 28, 30)
DevLarva <- c(35.5, 21.5, 21.3, 22.5, 35, 26)
DevRateLarva <- 1/DevLarva
obs <- data.frame(Temperature,DevLarva,DevRateLarva)

Bieri = function(T) {
  a = coef(model)[1]
  Tmin = coef(model)[2]
  Tmax = coef(model)[3]
  y = a*T*(T-Tmin)*sqrt(Tmax-T)
  y[T<Tmin | T>Tmax] = 0
  y
}

show_model = function() {
  x = -10:35
  Pred = data.frame(
    Temperature = x,
    DevRateLarva = Bieri(x)
  )

  ggplot(Pred, aes(x=Temperature, y=DevRateLarva)) +
    geom_line(colour="blue", size=1) +
    geom_point(colour="red", size=3, data=obs) +
    theme_minimal()
}  

# Complete data
model <- devRateModel(eq = briere1_99, temp = Temperature,  devRate = DevRateLarva,  
                      startValues = list(aa = 0.004022, Tmin = 16, Tmax = 32))
resultNLS <- devRatePrint(myNLS = model, temp = Temperature, devRate = DevRateLarva)
show_model()

# Remove probable outlier
model <- devRateModel(eq = briere1_99, temp = Temperature[-6],  devRate = DevRateLarva[-6],  
                      startValues = list(aa = 0.004022, Tmin = 0, Tmax = 32))
summary(model)
show_model()
                      