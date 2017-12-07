sim = read_output(output_file_name)
sim = sim[, c("iteration", "date", "Tabsoluta.adult.content", "oviposition.fecundity.fecundity", "Tavg")]
colnames(sim) = c("Simulation", "Date", "PopSize", "Fecundity", "Temperature")

fileName = "C:/Users/au152367/Documents/QDev/UniSim2/input/student/tuta/data.csv"
obs = read.table (file = fileName, header = TRUE, sep = ",")
obs = obs[,1:4]
obs$Date = dmy("30/6/2016") + 7*(0:24)
obs$Date = as.POSIXct(obs$Date)
obs = melt(obs, id.vars="Date", value.name="PopSize", variable.name="Cage")

P1 = ggplot(obs, aes(x=Date, y=PopSize)) +
  geom_line(data=sim, aes(group=Simulation), size=3, alpha=0.4) +
  geom_line(aes(colour=Cage), size=1) +
  geom_point(aes(colour=Cage), size=2) +
  scale_x_datetime(breaks = date_breaks('months'), labels = date_format('%b')) 
  
P2 = ggplot(sim, aes(x=Date)) +
  geom_line(aes(y=Temperature), colour="red", size=1.5) +
  geom_line(aes(y=Fecundity, group=Simulation), size=3, alpha=0.4) 

P = grid.arrange(P1, P2, ncol=1)
  
print(P)
