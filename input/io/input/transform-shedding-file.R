library(ggplot2)
library(lubridate)
library(plyr)
library(reshape2)

setwd("C:/Users/au152367/Documents/QDev/UniSim2/input/io/input")
obs = read.delim("Bavaria shedding onset.txt",  header=TRUE)
obs$OnsetDate =  dmy(obs$Onset)
obs$Year = factor(year(obs$OnsetDate))
obs = ddply(obs, .(Year), transform, ProbabilitySum=round((1:length(OnsetDate))/length(OnsetDate),4))
summary(obs)

write.table(obs[,-c(1,3,4)], "shedding-onset.txt", quote=FALSE, sep="\t", row.names=FALSE)

