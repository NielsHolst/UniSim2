keepPages = FALSE; keepVariables = FALSE
box_script_folder = "C:/Users/au152367/Documents/QDev/UniSim2/input/projects/vg/excel"
output_file_name  = "C:/Users/au152367/Documents/QDev/UniSim2/output/2020-10-28-leakage-transformed_0037.txt"
source("C:/Users/au152367/Documents/QDev/UniSim2/input/scripts/begin.R")
sim = read_output(output_file_name)

S = sim
# S = subset(sim, dateTime >= dmy("9/7/2001") & dateTime < dmy("10/7/2001"))

# x=S[c("dateTime", "actScreen1", "indoorsT", "screen1T")]
# x[2:4] = round(x[2:4],2)
# head(x,30)


M = melt(S[c("dateTime", "iteration", "spVentilationTemperature", "indoorsT", "spHeatingTemperature")], id.vars=c("dateTime", "iteration"))
p1 = ggplot (M, aes(dateTime, value, colour=variable)) +
  geom_line(alpha=0.5) +
  guides(color = guide_legend(override.aes = list(alpha = 1)))
  
M = melt(S[c("dateTime", "iteration", "spRhMax", "indoorsRh", "spRhMin")], id.vars=c("dateTime", "iteration"))
p2 = ggplot (M, aes(dateTime, value, colour=variable)) +
  geom_line(alpha=0.5) +
  guides(color = guide_legend(override.aes = list(alpha = 1)))
  
M = melt(S[c("dateTime", "iteration", "airFluxPower", "actHeatingPower")], id.vars=c("dateTime", "iteration"))
p3 = ggplot (M, aes(dateTime, value, colour=variable)) +
  geom_line(alpha=0.5) +
  guides(color = guide_legend(override.aes = list(alpha = 1)))

M = melt(S[c("dateTime", "iteration", "airFluxTotal", "spCrackVentilation")], id.vars=c("dateTime", "iteration"))
p4 = ggplot (M, aes(dateTime, value, colour=variable)) +
  geom_line(alpha=0.5) +
  guides(color = guide_legend(override.aes = list(alpha = 1)))

# M = melt(S[c("dateTime", "iteration", "actScreen1HeatCapacity")], id.vars=c("dateTime", "iteration"))
# p5 = ggplot (M, aes(dateTime, value, colour=variable)) +
  # geom_line() 
  
grid.arrange(p1, p2, p3, p4, ncol=2)
