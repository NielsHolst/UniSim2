rm(list=ls(all=TRUE))
library(ggplot2)
graphics.off()

# Set the working directory to point to your input folder
setwd("/Users/au152367/Documents/QDev/UniSim2/input")
source("papers/2021-saccharina/analysis/setup.R")

here = function(file_name) {
  paste0("papers/2021-saccharina/obs/", file_name)
}

file_name = "alge-obs-2019-AVG-vandrette-liner-100m-CN.txt"
obs = read.table(here(file_name), TRUE, "\t")
obs$date = dmy(obs$date)
obs = subset(obs, dryWeight>0)
obs$kA = with(obs, Wstruct/area)

model = lm(kA~Wstruct, obs[-3,])
summary(model)
intercept = coef(model)[1]
slope     = coef(model)[2]

f = function(x) {
  x/(slope*x + intercept)
}

model = lm(log(area)~log(Wstruct), obs)
summary(model)
intercept2 = coef(model)[1]
slope2     = coef(model)[2]

g = function(x) {
  logy = slope2*log(x) + intercept2
  exp(logy)
}

x = (0:240)/40
M = data.frame(
  Wstruct = x,
  Area1 = f(x),
  Area2 = g(x),
  kA1   = x/f(x),
  kA2   = x/g(x)
)
A = melt(M[,1:3], .(Wstruct), value.name="Area", variable.name="Model")
K = melt(M[,c(1, 4:5)], .(Wstruct), value.name="kA", variable.name="Model")

ggarrange(
  ggplot(obs, aes(Wstruct)) +
    geom_point(aes(y=area), colour=green, size=3) +
    geom_line(aes(y=Area, colour=Model), data=A) +
    theme_bw()
  ,
  ggplot(obs, aes(Wstruct, kA, colour=Model)) +
    geom_point(colour=green, size=3) +
    geom_line(data=K) +
    geom_hline(yintercept=0.6, colour=violet,size=1) +
    theme_bw()
  ,
  ncol=1, align="h"
)

arr = arrow(type="closed", length=unit(2, "mm"))
ggarrange(
  ggplot(obs, aes(date, kA)) +
    geom_point(colour=red, size=3) +
    geom_path(colour=blue, size=1, arrow=arr)+
    labs(x="") +
    theme_bw()
  ,
  ggplot(obs, aes(N, kA)) +
    geom_point(colour=red, size=3) +
    geom_path(colour=blue, size=1, arrow=arr)+
    theme_bw()
  ,
  ggplot(obs, aes(C, kA)) +
    geom_point(colour=red, size=3) +
    geom_path(colour=blue, size=1, arrow=arr)+
    theme_bw()
  ,
  ncol=1, align="h"
)
    
    
