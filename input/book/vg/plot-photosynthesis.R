library(ggplot2)

# Change this path
setwd("C:/Users/au152367/Documents/QDev/UniSim2/output")

# Load simulation output
load("sim.Rdata")

# Get an overview
head(sim)
unique(sim$parAbsorbed)
unique(sim$co2)
unique(sim$temperature)
unique(sim$rh)

# Function definitions
reorder_levels = function(the_factor, new_order) {
  factor(the_factor,levels(the_factor)[new_order])
}

plot_Pgmax_by_parAbsorbed = function() {
  M = subset(sim,
    temperature %in% c(0,10,20,30,40) &
    rh %in% c(20,60,80,100) &
    co2 %in% c(400,600,1000,1200) 
  )
  M$temperature = factor(paste(M$temperature, "oC"))
  M$rh          = factor(paste(M$rh, "%RH"))
  M$co2         = factor(paste(M$co2, "ppm"))
  M$rh  = reorder_levels(M$rh,  c(2:4,1))
  M$co2 = reorder_levels(M$co2, c(3:4,1:2))
  
  ggplot(M, aes(parAbsorbed, Pgmax, colour=temperature)) +
    geom_line() +
    guides(colour = guide_legend(reverse = TRUE)) +
    facet_grid(co2~rh)
}

plot_Pgmax_by_temperature = function() {
  M = subset(sim,
    parAbsorbed %in% c(100,500,900,1300) &
    rh %in% c(20,60,80,100) &
    co2 %in% c(400,600,1000,1200) 
  )
  M$rh          = factor(M$rh)
  M$parAbsorbed = factor(paste(M$parAbsorbed, "uPAR"))
  M$co2         = factor(paste(M$co2, "ppm"))
  M$parAbsorbed = reorder_levels(M$parAbsorbed,  c(1,3,4,2))
  M$co2 = reorder_levels(M$co2, c(3:4,1:2))
  
  ggplot(M, aes(temperature, Pgmax, colour=rh)) +
    geom_line() +
    guides(colour = guide_legend(reverse = TRUE)) +
    facet_grid(co2~parAbsorbed)
}

# Plot figures
plot_Pgmax_by_parAbsorbed()
plot_Pgmax_by_temperature()


