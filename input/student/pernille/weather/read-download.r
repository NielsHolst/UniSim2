graphics.off()
rm(list=ls(all=TRUE))

library(ncdf4)
library(plyr)
library(raster)
library(rgdal)
library(stringr)

setwd("C:/Users/au152367/Documents/QDev/UniSim2/input/student/pernille/weather")
T0 = 273.15

parse_file_name = function(file_name) {
  s = str_split(file_name, "_")[[1]]
  data.frame(
    FileName = file_name,
    Latitude = as.numeric(s[1]),
    Longitude = as.numeric(s[2]),
    Year = as.numeric(s[3]),
    Month = as.numeric(substr(s[4],1,2))
  )
}

svp = function(temperature) {
  T = temperature + T0;
  133.32*exp((1.0887*T - 276.4)/(0.0583*T - 2.1938));
}

read_net_cdf_file = function(m) {
  obs = nc_open(paste0("download/", m$FileName))

  time = ncvar_get(obs,"time")
  d2m = ncvar_get(obs, "d2m")
  t2m = ncvar_get(obs, "t2m")
  sp = ncvar_get(obs, "sp")
  ssrd = ncvar_get(obs, "ssrd")
  tp = ncvar_get(obs, "tp")

  M = data.frame(
    DateTime = as.POSIXct(3600*time, origin = "1900-01-01",tz = "GMT"),
    Temperature = round(t2m - T0, 2),
    DewPoint = round(d2m - T0, 2),
    Pressure = round(sp, 1),
    Precipitation = round(1000*tp, 1)
  )
  M$RH = 100*svp(M$DewPoint)/svp(M$Temperature)
  M
}

count = function(w) {
  nrow(w)
}

M = adply(list.files("download"), 1, parse_file_name)[,-1]
head(M)
nrow(M)

weather = ddply(M, .(Latitude, Longitude, Year, Month), read_net_cdf_file)
head(weather)
summary(weather)
nrow(weather)
ddply(weather, .(Latitude, Longitude, Year), function(w) {nrow(w)})
ddply(subset(weather, is.na(Temperature)), .(Latitude, Longitude, Year), function(w) {nrow(w)})


save(weather, file="brazil-weather-2000.Rdata")


