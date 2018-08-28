keepPages = FALSE
keepVariables = FALSE
source("../input/scripts/begin.R")

path = "C:/Users/au152367/Documents/QDev/UniSim2/output"
setwd(path)
filePath = paste(path, "ig-scenario_0008.txt", sep="/")
filePath

sim = read_output(filePath)
summary(sim)

sim_sample = subset(sim, minute(dateTime)==0)
summary(sim_sample)

ggplot(sim_sample, aes(x=dateTime, y=indoorsT)) +
  geom_line(colour="blue") +
  geom_point(colour="red")

  
  
