keepPages = FALSE; keepVariables = FALSE
box_script_folder = "C:/Users/au152367/Documents/QDev/UniSim2/input/projects/vg/excel"
output_file_name  = "C:/Users/au152367/Documents/QDev/UniSim2/output/2020-10-28-sp-heating-transformed_0003.txt"
source("C:/Users/au152367/Documents/QDev/UniSim2/input/scripts/begin.R")
sim = read_output(output_file_name)

M = ddply(sim, .(iteration), summarise,
      irradiation = max(totalIrradiation),
      skyAbsorbed = max(totalSkyAbsorbed),
      convectionEnergy = min(totalConvectionEnergy),
      ventilation = min(totalVentilationEnergy),
      soilAbsorbed = min(totalSoilEnergy),
      heating = max(totalHeatingEnergy),
      light = max(totalGrowthLightEnergy),
      Tavg = mean(indoorsT),
      Tunder = mean(pmax(spVentilationTemperature - indoorsT,0)),
      Tabove = mean(pmax(indoorsT - spHeatingTemperature,0)),
      coverTavg = mean(coverT),
      coverTmax = max(coverT),
      airFluxPoweravg = mean(airFluxPower)
)
M$spHeating = 5 + c(4,8,12,16,20)
M = melt(M[c("spHeating", "ventilation", "heating")], id.vars="spHeating")
levels(M$variable) = c("Energy lost to by ventilation", "Energy used for heating")

ggplot(M, aes(spHeating, value, colour=variable)) +
  geom_point(size=4) +
  geom_line(size=2) +
  scale_colour_manual(values=c('#377eb8','#e41a1c')) +
  facet_wrap(~variable, ncol=1, scales="free") +
  theme(legend.position = "none") 
  

S = subset(sim, (iteration==1 | iteration==11) & dateTime>=dmy("1/7/2001") & dateTime<=dmy("2/7/2001"))
S$iteration = factor(S$iteration)
S$Hour = hour(S$dateTime) + minute(S$dateTime)/60
levels(S$iteration) = c("spHeating = 4 oC", "spHeating = 24 oC")

M = melt(S[c("Hour", "iteration", "spVentilationTemperature", "indoorsT", "spHeatingTemperature")], id.vars=c("Hour", "iteration"), value.name="Temperature")
p1 = ggplot (M, aes(Hour, Temperature, colour=variable)) +
  geom_line(alpha=0.5) +
  guides(color = guide_legend(override.aes = list(alpha = 1))) +
  scale_x_continuous(breaks = 4*(0:6)) +
  facet_wrap(~iteration)
  
M = melt(S[c("Hour", "iteration", "spRhMax", "indoorsRh", "spRhMin")], id.vars=c("Hour", "iteration"), value.name="RH")
p2 = ggplot (M, aes(Hour, RH, colour=variable)) +
  geom_line(alpha=0.5) +
  guides(color = guide_legend(override.aes = list(alpha = 1)))+
  scale_x_continuous(breaks = 4*(0:6)) +
  facet_wrap(~iteration)
  
M = melt(S[c("Hour", "iteration", "airFluxPower", "actHeatingPower")], id.vars=c("Hour", "iteration"), value.name="Energy")
p3 = ggplot (M, aes(Hour, Energy, colour=variable)) +
  geom_line(alpha=0.5) +
  guides(color = guide_legend(override.aes = list(alpha = 1)))+
  scale_x_continuous(breaks = 4*(0:6)) +
  facet_wrap(~iteration)

M = melt(S[c("Hour", "iteration", "airFluxTotal", "spCrackVentilation")], id.vars=c("Hour", "iteration"), value.name="AirFlux")
p4 = ggplot (M, aes(Hour, AirFlux, colour=variable)) +
  geom_line(alpha=0.5) +
  guides(color = guide_legend(override.aes = list(alpha = 1)))+
  scale_x_continuous(breaks = 4*(0:6)) +
  facet_wrap(~iteration)

ggarrange(p1, p2, p3, p4, align="hv")

S = subset(sim, iteration==1 & dateTime>=dmy("1/7/2001") & dateTime<=dmy("2/7/2001"))
S[c("dateTime", "airFluxPower", "actHeatingPower", "thermostatState", "thermostatAction", "thermostatSolution")]


