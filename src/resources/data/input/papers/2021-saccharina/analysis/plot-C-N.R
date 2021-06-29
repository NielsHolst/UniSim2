library(ggplot2)
rm(list=ls(all=TRUE))
graphics.off()

# Set the working directory to point to your input folder
setwd("/Users/au152367/Documents/QDev/UniSim2/input")
source("papers/2021-saccharina/analysis/setup.R")
source("papers/2021-saccharina/analysis/C-N-functions.R")

here = function(file_name) {
  paste0("papers/2021-saccharina/obs/", file_name)
}


plot_chapman = function() {
  file_name = "C-N-chapman-et-al-1978-CN.txt"
  obs = read.table(here(file_name), TRUE, "\t")
  colnames(obs)[2:5] = c("N%","C%","Cres","Nres")

  M = melt(obs, id.vars="nitrogenWater")
  M$variable = factor(M$variable)
  levels(M$variable)
  M$variable = reorder_levels(M$variable, c(2,1,3,4))

  A = ddply(M, .(nitrogenWater, variable), summarise, value = mean(value))

  ggplot(M, aes(nitrogenWater/1000, value)) +
    geom_point(colour=red, size=2) +
    geom_line(colour=blue, data=A) +
    facet_wrap(~variable, ncol=1, scales="free") +
    labs(x="Water nitrogen concentration (nM)", y="") +
    theme_bw()
}

plot_sjotun = function() {
  file_name = "C-N-proximal-sjotun-1993-CN.txt"
  obs1 = read.table(here(file_name), TRUE, "\t")
  obs1$Position = "Proximal"

  file_name = "C-N-apical-sjotun-1993-CN.txt"
  obs2 = read.table(here(file_name), TRUE, "\t")
  obs2$Position = "Distal"
  obs = rbind(obs1, obs2)
  colnames(obs)[2:5] = c("N%","C%","Cres","Nres")

  M = melt(obs, id.vars=c("Position","month"))
  
  M$Position = factor(M$Position)
  levels(M$Position)
  M$Position = reorder_levels(M$Position, 2:1)
  

  M$variable = factor(M$variable)
  M$variable = reorder_levels(M$variable, c(2,1,3,4))
  M$Order = (M$month+4)%%12

  ggplot(M, aes(Order, value)) +
    geom_point(colour=red, size=2) +
    geom_line(colour=blue) +
    facet_grid(variable~Position, scales="free") +
    scale_x_continuous(breaks=unique(M$Order), labels=unique(M$month)) +
    labs(x="Month", y="") +
    theme_bw()
}

plot_nielsen = function() {
  file_name = "C-N-nielsen-et-al-2014-CN.txt"
  obs = read.table(here(file_name), TRUE, "\t")
  colnames(obs)[2:5] = c("N%","C%","Cres","Nres")
  obs$Order = 1:nrow(obs)
  obs

  M = melt(obs, id.vars=c("Order","month"))
  M$variable = factor(M$variable)
  levels(M$variable)
  M$variable = reorder_levels(M$variable, c(2,1,3,4))

  ggplot(M, aes(Order, value)) +
    geom_point(colour=red, size=2) +
    geom_line(colour=blue) +
    facet_wrap(~variable, ncol=1, scales="free") +
    scale_x_continuous(breaks=1:6, labels=M$month[1:6]) +
    labs(x="Month", y="") +
    theme_bw()
}

plot_gerard = function() {
  file_name = "C-N-gerard-1990-CN.txt"
  obs = read.table(here(file_name), TRUE, "\t")[,-3]
  colnames(obs)[3:6] = c("C%","N%","Cres","Nres")
  M = melt(obs, id.vars=c("Origin","Irradiation"))

  ggplot(M, aes(Irradiation, value)) +
    geom_point(colour=red, size=2) +
    geom_line(colour=blue) +
    facet_grid(variable~Origin, scales="free") +
    scale_x_continuous(breaks=c(1,3,12)) +
    labs(x="Irradiation (mol PAR/day)", y="") +
    theme_bw()
}

windows()
plot_chapman()
windows()
plot_nielsen()
windows()
plot_sjotun()
windows()
plot_gerard()
