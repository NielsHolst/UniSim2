library(ggplot2)
graphics.off()
rm(list=ls(all=TRUE))

source("calendar_demo.R")

ggplot(U, aes(dateTime)) +
	geom_line(aes(y=dayLength), colour="red", size=2) 
	
windows()
ggplot(U, aes(dateTime)) +
	geom_line(aes(y=sunrise), colour="red", size=2) 
