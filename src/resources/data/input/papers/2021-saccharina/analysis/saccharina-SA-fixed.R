# Set the working directory to point to your input folder
setwd("/Users/au152367/Documents/QDev/UniSim2/input")
source("papers/2021-saccharina/analysis/setup.R")

source("papers/2021-saccharina/analysis/saccharina-SA-fixed.part.R")
load(  "papers/2021-saccharina/analysis/saccharina-SA-fixed.Rdata")

plot_all(sim)
