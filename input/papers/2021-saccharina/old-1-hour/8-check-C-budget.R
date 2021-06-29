setwd("C:/Users/au152367/Documents/QDev/UniSim2/input/papers/2021-saccharina")
obs = read.table("obs/alge-obs-2019-AVG-vandrette-liner-100m-CN.txt", TRUE, "\t")[,1:3]
colnames(obs) = c("Date", "Area", "DryWeight")
obs = subset(obs, DryWeight>0)
obs$Date = dmy(obs$Date)
# obs$Area   = 400*obs$Area/100      # m2 leaf per m line


days = as.numeric(max(obs$Date) - min(obs$Date))
dates = min(obs$Date) + 0:days
area = approxfun(obs$Date, obs$Area)
dw   = approxfun(obs$Date, obs$DryWeight)

obs = data.frame(
  Date = dates,
  Area = area(dates),
  DryWeight = dw(dates)
)
obs$Growth = 400*c(diff(obs$DryWeight), NA)
length = 3.5*sqrt(obs$Area)/10 # m
obs$CrownZone = length # m2 per m line

env = read.table("env/env_file_2019-AVG.txt", TRUE, "\t")
env$Date = dmy(env$Date)
env = subset(env, Date>=min(obs$Date) & Date<max(obs$Date))
env = ddply(env, .(Date), summarise, I=sum(I))
env$I = env$I*3600e-6

M = join(env, obs, by="Date")
M$LAI = M$Area/M$CrownZone
M$Iabs = (1 - exp(-M$LAI))*M$I
M$Alpha = M$Growth/M$Iabs
M = melt(M, id.vars="Date")
ggplot(M, aes(Date, value, colour=variable)) +
  geom_line() +
  facet_wrap(~variable, scales="free") +
  theme_bw() +
  theme(
    legend.position = "none"
  )
