library(ggplot2)
library(gridExtra)
library(plyr)
library(reshape2)
library(rjson)

graphics.off()

setwd("/Users/au152367/Documents/QDev/UniSim2/input/projects/ig")
json = fromJSON(file="Unisim-input-21-08-2019.json")


