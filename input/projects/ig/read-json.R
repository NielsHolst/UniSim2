rm(list=ls(all=TRUE))
graphics.off()

library(ggplot2)
library(gridExtra)
library(lubridate)
library(plyr)
library(reshape2)
library(rjson)

setwd("/Users/au152367/Documents/QDev/UniSim2/input/projects/ig")
json = fromJSON(file="UnisimInput_2019-28-05_ 2019-06-04-2019.json")


print(paste("Number of samples:", length(json)))
print(paste("Items per sample :", length(json[[1]])))
names(json[[1]])

obs = json[[1]]

date_time = function(day, hour) {
  x = dmy("31/12/2018")
  h = floor(hour)
  m = floor(60*(hour - h))
  s = floor(3600*(hour - h) - 60*m)
  x + day + hours(h) + minutes(m) + seconds(s)
}

get_screens = function(screens) {

  get_screen = function(screen) {
    screen$Effect$Value/100
  }

  M = data.frame(t(ldply(screens, get_screen)))
  colnames(M) = paste0("ScreenEffect", 1:length(screens))
  rownames(M) = {}
  M
}

get_data = function(obs) {
  area = obs$Construction$Length * obs$Construction$SpanWidth * obs$Construction$SpanCount 
  indoors = obs$IndoorClimate
  outdoors = obs$OutdoorClimate
  growthLights = obs$GrowthLights
  if (length(growthLights) > 1) stop("More than one element in growthLights")
  growthLight = growthLights[[1]]
  M = data.frame(
    DateTime = NA,
    Day = obs$TimeStamp$DayOfYear,
    Hour = obs$TimeStamp$TimeOfDay,
    IndoorsTemperature = indoors$Temperature$Value,
    IndoorsLightIntensity = indoors$LightIntensity$Value*7.9,  # Convert from klux to W/m2 (sunlight spectrum)
    IndoorsRH = indoors$RelativeHumidity$Value,
    IndoorsCo2 = indoors$Co2$Value,
    OutdoorsTemperature = outdoors$Temperature$Value,
    OutdoorsIrradiation = outdoors$Irradiation$Value,
    OutdoorsCo2 = outdoors$Co2$Value,
    OutdoorsWindSpeed = outdoors$WindSpeed$Value,
    OutdoorsWindDirection = outdoors$WindDirection$Value,
    GrowthLightIntensity = growthLight$Intensity / area
  ) 
  M$DateTime = date_time(M$Day, M$Hour)
  M$IndoorsLightIntensityMax = M$OutdoorsIrradiation + M$GrowthLightIntensity

  cbind(M, get_screens(obs$Screens))
}

M0 = ldply(json, get_data)

ggplot(M0, aes(x=IndoorsLightIntensityMax, y=IndoorsLightIntensity, colour=Hour)) +
  geom_point() +
  theme_minimal()
  


plot_sensors = function() {
  M = melt(M0[, c(1, 4:12)], id.vars="DateTime", value.name="Value", variable.name="Variable")

  ggplot(M, aes(x=DateTime, y=Value, colour=Variable)) +
    geom_line() +
    ylab("") + 
    theme_classic() +
    facet_wrap(~Variable, scales="free")
}

plot_actuators = function() {
  M = melt(M0[, c(1, 13:16)], id.vars="DateTime", value.name="Value", variable.name="Variable")

  ggplot(M, aes(x=DateTime, y=Value, colour=Variable)) +
    geom_line() +
    ylab("") + 
    theme_classic() +
    facet_wrap(~Variable, scales="free")
}

windows(12,6)
plot_sensors()

windows(12,6)
plot_actuators()

model = lm(IndoorsLightIntensity ~ OutdoorsIrradiation + GrowthLightIntensity +ScreenEffect1 + ScreenEffect2, data=M0)
summary(model)
plot(model, 1)

ggplot(M0, aes(x=OutdoorsIrradiation, y=predict(model), colour=Hour)) +
  geom_point() +
  theme_minimal()
  




    