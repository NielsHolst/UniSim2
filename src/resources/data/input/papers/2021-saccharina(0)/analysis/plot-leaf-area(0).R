rm(list=ls(all=TRUE))
library(ggplot2)
graphics.off()

# Set the working directory to point to your input folder
setwd("/Users/au152367/Documents/QDev/UniSim2/input")
source("papers/2021-saccharina/analysis/setup.R")

here = function(file_name) {
  paste0("papers/2021-saccharina/obs/", file_name)
}

model = function(obs) {
  model = lm(log(area)~log(Wstruct), obs)
  co = coef(model)
  names(co)={}
  c(a=exp(co[1]), b=co[2])
}

f = function(x, a, b) {
  a*x^b
}

Wstruct = (0:90)/30

own_model = function() {
  file_name = "alge-obs-2019-AVG-vandrette-liner-100m-CN.txt"
  obs = read.table(here(file_name), TRUE, "\t")
  obs = subset(obs, dryWeight>0)
  co = model(obs)
  data.frame(
    DataSet = "Own data",
    Wstruct = Wstruct,
    area = f(Wstruct, co[1], co[2])
  )
}

plot_own_model = function() {
  file_name = "alge-obs-2019-AVG-vandrette-liner-100m-CN.txt"
  obs = read.table(here(file_name), TRUE, "\t")
  obs = subset(obs, dryWeight>0)
  ggplot(obs, aes(Wstruct, area)) +
    geom_point(colour=red, size=3) +
    geom_line(colour=blue, data=own_model()) +
    theme_bw()
}

broch_model = function() {
  data.frame(
    DataSet = "Broch et al. (2011)",
    Wstruct = Wstruct,
    area = Wstruct/0.6
  )
}

# file_name = "C-N-gerard-1990-CN.txt"
# obs = read.table(here(file_name), TRUE, "\t")
# obs$wetWeight = obs$dryWeight/0.113
# obs$area = with(obs, AreaRatio*wetWeight/100)
# ggplot(obs, aes(Wstruct, area, colour=Origin)) +
  # geom_point(size=3) 


plot_own_model()

plot_all_models = function() {
  M = rbind(
    own_model(),
    broch_model()
  )
  ggplot(M, aes(Wstruct, area, colour=DataSet)) +
    geom_line()
}
plot_all_models()

