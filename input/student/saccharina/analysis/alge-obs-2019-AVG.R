library(ggplot2)
library(ggpubr)
library(lubridate)
library(reshape2)

rm(list=ls(all=TRUE))
graphics.off()

setwd("C:/Users/au227859/UniversalSimulatorHome/input/teis")

obs = read.table("alge-obs-2019-AVG.txt", header=TRUE, sep="\t")
obsDate = ymd(obs$Date)
obs

#Calculate structural dryweight

Cstruct = 0.02

Nstruct = 0.01

obs$StructWeightYieldprmeter = obs$DryweightYieldprmeter - (obs$DryweightYieldprmeter * ((obs$carbonPct/100-Cstruct)+(obs$nitrogenPct/100-Nstruct)))

# Replace missing densities with average
n = mean(obs$plantDensity, na.rm=TRUE)
n
obs$plantDensity[is.na(obs$plantDensity)] = n
# Calculate average per plant, all dimensions in dm
obs$AvgArea = obs$totalLeafArea/obs$plantDensity*100
obs$AvgDryWeight = obs$DryweightYieldprmeter/obs$plantDensity
obs$AvgStructWeight = obs$StructWeightYieldprmeter/obs$plantDensity
obs$AvgLength = obs$Length/10
obs



ggarrange(
  ggarrange(
    ggplot(obs, aes(AvgArea, AvgLength)) +
      geom_point(size=4, colour="red") +
      labs(x="Leaf area (dm2)", y="Leaf length (dm)")
    ,
    ggplot(obs, aes(sqrt(AvgArea), AvgLength)) +
      geom_smooth(method="lm", colour="blue", size=2) +
      geom_point(size=4, colour="red") +
      labs(x="Sqrt Leaf area (dm)", y="Leaf length (dm)")
    ,
    ncol=1, align="hv"
  )
  ,
  ggarrange(
    ggplot(obs, aes(AvgDryWeight,AvgArea)) +
      geom_point(size=4, colour="forestgreen") +
      labs(y="Leaf area (dm2)", x="Dry weight g")
    ,
    ggplot(obs, aes(log(AvgDryWeight), log(AvgArea))) +
      geom_smooth(method="lm", colour="blue", size=2) +
      geom_point(size=4, colour="forestgreen") +
      labs(y="Log leaf area (dm2)", x="Log dry weight (g)")
    ,
    ncol=1, align="hv"
  )
  ,
  ggarrange(
    ggplot(obs, aes(AvgStructWeight,AvgArea)) +
      geom_point(size=4, colour="forestgreen") +
      labs(y="Leaf area (dm2)", x="Struct weight g")
    ,
    ggplot(obs, aes(log(AvgStructWeight), log(AvgArea))) +
      geom_smooth(method="lm", colour="blue", size=2) +
      geom_point(size=4, colour="forestgreen") +
      labs(y="Log leaf area (dm2)", x="Log Struct weight (g)")
    ,
    ncol=1, align="hv"
  )
)

model = lm(AvgLength ~ sqrt(AvgArea)-1, obs)
summary(model)
plot(model,1)
slope = coef(model)[1]
slope

x = (0:150)/10
M = data.frame(
  A = x,
  L = slope*sqrt(x)
)
ggplot(M, aes(A, L)) +
  geom_line() +
  labs(x="Area (dm2)", y = "Length (dm)")


model = lm(log(AvgArea) ~ log(AvgDryWeight), obs)
summary(model)
plot(model,1)
co = coef(model)
a = exp(co[1])
b = co[2]
a
b

  kA = 0.6  # g/dm2
  x = (0:100)/10 # g
  M = data.frame(
    Weight = x,
    AreaPowerLaw = a*x^b,
    AreaFixed = x/kA
  )
  M = melt(M, id.vars="Weight", value.name="Area", variable.name="Model")
  ggplot(M, aes(Weight, Area, colour=Model)) +
    labs(x="Weight (g)", y="Area (dm2)") +
    geom_line()
  
model = lm(log(AvgArea) ~ log(AvgStructWeight), obs)
summary(model)
plot(model,1)
co = coef(model)
c = exp(co[1])
d = co[2]
c
d
  
  kA = 0.6  # g/dm2
  x = (0:100)/10 # g
  M = data.frame(
    StructWeight = x,
    AreaPowerLaw = c*x^d,
    AreaFixed = x/kA
  )
  M = melt(M, id.vars="StructWeight", value.name="Area", variable.name="Model")
  ggplot(M, aes(StructWeight, Area, colour=Model)) +
    labs(x="Structural Weight (g)", y="Area (dm2)") +
    geom_line()

  