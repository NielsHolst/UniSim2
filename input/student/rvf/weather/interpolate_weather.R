library(ggplot2)
library(lubridate)
library(plyr)

rm(list=ls(all=TRUE))
graphics.off()
setwd("/Users/au152367/Documents/QDev/UniSim2/input/student/rvf/weather")

interpolate_temperature = function(fileName) {
  obs = read.table(fileName, header=TRUE)
  obs$Date = dmy(obs$Date)

  days_num = as.numeric(max(obs$Date) - min(obs$Date))
  days_range = min(obs$Date) + (0:days_num)

  M = data.frame(
    Date = days_range,
    Temperature = approx(obs$Date, obs$Tavg, days_range)$y
  )
  M
}  

interpolate_rainfall = function(fileName) {
  obs = read.table(fileName, header=TRUE)
  obs$Date = dmy(obs$Date)
  obs$Rainfall[obs$Rainfall < 0] = 0

  days_num = as.numeric(max(obs$Date) - min(obs$Date))
  days_range = min(obs$Date) + (0:days_num)

  M = data.frame(Date = days_range)
  hits = (M$Date %in% obs$Date)
  M$Rainfall = 0
  M$Rainfall[hits] = obs$Rainfall
  M
}

merge_files = function(location) {
  M1 = interpolate_temperature(paste0("temperature_", location, ".txt"))
  M2 = interpolate_rainfall(paste0("rainfall_", location, ".txt"))
  from_date = max(c(min(M1$Date), min(M2$Date)))
  to_date   = min(c(max(M1$Date), max(M2$Date)))
  M1 = subset(M1, Date>= from_date & Date<=to_date)
  M2 = subset(M2, Date>= from_date & Date<=to_date)
  M = cbind(M1, Rainfall=M2$Rainfall)
  M[,2:3] = round(M[,2:3], 2)
  write.table(M, paste0("weather_", location, ".txt"), quote=FALSE, sep="\t", row.names=FALSE, )
}

merge_files("ngorongoro_narok")
merge_files("musoma")
