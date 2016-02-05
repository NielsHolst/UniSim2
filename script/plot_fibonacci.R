library(ggplot2)
graphics.off()
rm(list=ls(all=TRUE))

setwd("C:/data/QDev/UniSim2/script")
source("fibonacci.R")

ggplot(output, aes(step, log10(value))) +
	geom_line(colour="red", size=2) +
	geom_point(colour="red", size=4)
	
