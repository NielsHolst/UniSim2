library(ggplot2)
library(lubridate)
library(plyr)
library(reshape2)

setwd("C:/Users/au152367/Documents/QDev/UniSim2/input/io/input")
obs = read.delim("Westerloge-Bad Hersfeld.txt",  header=TRUE)
obs$Date =  dmy(obs$Date)
obs$Year = factor(year(obs$Date))
obs = ddply(obs, .(Year), transform, PollenSum=cumsum(Pollen))
obs = ddply(obs, .(Year), transform, PollenCurve=round(PollenSum/max(PollenSum),4))
summary(obs)

ggplot(obs, aes(x=yday(Date), y=PollenCurve, colour=Year)) +
  geom_line() +
  theme_minimal()

write.table(obs[,-c(2,4,5)], "weather-Westerloge.txt", quote=FALSE, sep="\t", row.names=FALSE)

