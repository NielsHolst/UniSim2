# Read simulation output
sim = read_output(output_file_name)
head(sim)

# Simplify headers (unique_names is defined by the UniSim framework)
colnames(sim) = unique_names(colnames(sim))
head(sim)

# Prepare for plotting
sim = sim[,-(1:2)]
Msim = melt(sim, id.vars="date")
Msim$Date = as.Date(Msim$date)

# Read observations
setwd(box_script_folder)
obs = read.table("obs-bmsb-2017.txt", header=TRUE, sep="\t")
obs$Date = as.Date(dmy(obs$Date))

# Use common header names
colnames(obs)[2:7] = colnames(sim)[5:10]

# Prepare for plotting
Mobs = melt(obs, id.vars="Date")

# sim is for 2015 and obs is for 2017; for now, cheat by replacing year in obs
Mobs$Date = dmy(paste(day(Mobs$Date), month(Mobs$Date), 2015, sep="/"))

p = ggplot(Msim, aes(Date, value, colour=variable)) +
  geom_line() +
  geom_point(data=Mobs) +
  scale_x_date(labels = date_format("%m"), breaks="months", limits=c(dmy("1/5/2015"), dmy("30/9/2015"))) +
  facet_wrap(~variable, scales="free_y") +
  theme(legend.position = "none")
print(p)
