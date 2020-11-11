keepPages = FALSE; keepVariables = FALSE
box_script_folder = "C:/Users/au152367/Documents/QDev/UniSim2/input/projects/vg/excel"
output_file_name  = "C:/Users/au152367/Documents/QDev/UniSim2/output/2020-10-28-leakage-transformed_0032.txt"
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
M$inflow  = with(M, irradiation+heating+light)
M$outflow = with(M, -skyAbsorbed+convectionEnergy+ventilation+soilAbsorbed)
M

# M$leakage = factor((0:5)/10)
# M[c("leakage", "heating", "ventilation")]



# i = 480*364.5
# j = i+20
# sim[i:j, c("dateTime","airFluxPower","totalVentilationEnergy")]


M = melt(sim[c("dateTime", "iteration", "spVentilationTemperature", "indoorsT", "spHeatingTemperature")], id.vars=c("dateTime", "iteration"))
M$iteration = factor(M$iteration)
levels(M$iteration) = c(0,0.5)
p1 = ggplot (M, aes(dateTime, value, colour=variable)) +
  geom_line() +
  facet_wrap(~iteration)

M = melt(sim[c("dateTime", "iteration", "spRhMax", "indoorsRh", "spRhMin")], id.vars=c("dateTime", "iteration"))
M$iteration = factor(M$iteration)
levels(M$iteration) = c(0,0.5)
p2 = ggplot (M, aes(dateTime, value, colour=variable)) +
  geom_line() +
  facet_wrap(~iteration)
 
M = melt(sim[c("dateTime", "iteration", "airFluxPower", "actHeatingPower")], id.vars=c("dateTime", "iteration"))
M$iteration = factor(M$iteration)
levels(M$iteration) = c(0,0.5)
p3 = ggplot (M, aes(dateTime, value, colour=variable)) +
  geom_line() +
  facet_wrap(~iteration)

M = melt(sim[c("dateTime", "iteration", "airFluxTotal", "spCrackVentilation")], id.vars=c("dateTime", "iteration"))
M$iteration = factor(M$iteration)
levels(M$iteration) = c(0,0.5)
p4 = ggplot (M, aes(dateTime, value, colour=variable)) +
  geom_line() +
  facet_wrap(~iteration)

grid.arrange(p1, p2, p3, p4)
