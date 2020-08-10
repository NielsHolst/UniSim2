graphics.off()
rm(list=ls(all=TRUE))
library(ggplot2)
library(plyr)
library(reshape2)

setwd("C:/Users/au152367/Documents/QDev/UniSim2/input/projects/vg/excel")

# ggplot set-up
unisim_colours = c('#e41a1c','#377eb8','#4daf4a','#984ea3','#ff7f00','#a65628','#f781bf','#999999') 
scale_colour_discrete = function(...) {
  scale_colour_manual(..., values = rep(unisim_colours, 10))
}
scale_linetype_discrete = function(...) {
  n = length(unisim_colours)
  scale_linetype_manual(..., values = rep(c(rep(1, n), rep(7, n)), 5))
}

# General functions
reorder_levels = function(the_factor, new_order) {
  factor(the_factor,levels(the_factor)[new_order])
}

# Global data
param = read.table("radiation-parameters.txt", TRUE, "\t", stringsAsFactors=FALSE)
param
p = NULL 
n = NULL

capacities = rep(Inf, nrow(param)/2)
names(capacities) = param$Layer[1:(nrow(param)/2)]
capacities[2] = 6968
capacities[3:4] = 2313
capacities

# infiltration = rep(Inf, nrow(param)/2)  # per hour
# names(infiltration) = param$Layer[1:(nrow(param)/2)]
# infiltration["Glass"] = 0.25
# infiltration["ThermalScreen"] = 0.25

rho.air = 1.225 # kg/m3
Cair.mass = 1020 # J/kg/K
Cair = rho.air*Cair.mass # J/K/m3

layers = function(layer_names) {
  short = subset(param, Radiation=="ShortWave" & Layer %in% layer_names)[,-2]
  long  = subset(param, Radiation=="LongWave" & Layer %in% layer_names)[,-2:-1]
  colnames(long) = paste0("L", colnames(long))
  M = cbind(short,long)
  M$T = NA
  M$E = 0
  M$E_ = 0
  M$LE = 0
  M$LE_ = 0
  M$F = 0
  M$F_ = 0
  M$LF = 0
  M$LF_ = 0
  M$A = 0
  M$A_ = 0
  M$LA = 0
  M$LA_ = 0
  M$Q = 0
  rownames(M) = {}
  M
}

air.set.up = function(M, Tout, Tin) {
  n = nrow(M)
  T = rep(Tin, n-1)
  T[1] = Tout
  data.frame(
    Layer = paste0("Over", M$Layer[2:n]),
    Volume = c(Inf, 0.6, 3, 0.3),
    T = T,
    Q = 0,
    DT = 0,
    stringsAsFactors = FALSE
  )
}

a  = function(i) p[i,1]
r  = function(i) p[i,2]
t  = function(i) p[i,3]
a_ = function(i) p[i,4]
r_ = function(i) p[i,5]
t_ = function(i) p[i,6]

distribute.down = function(I, I_, A, A_) {
  for (i in 1:(n-1)) {
    k = 1 - r(i+1)*r_(i)
    th  = t(i+1)/k
    ah  = a(i+1)/k
    th_ = t_(i) /k * r(i+1)
    ah_ = a_(i) /k * r(i+1)
    I [i+1] = I [i+1] + th *I[i]
    A [i+1] = A [i+1] + ah *I[i]
    I_[i]   = I_[i]   + th_*I[i]
    A_[i]   = A_[i]   + ah_*I[i]
  }
  I = 0
  list(I=I, I_=I_, A=A, A_=A_)
}

distribute.up = function(I, I_, A, A_) {
  for (i in n:2) {
    k = 1 - r_(i-1) *r(i)
    th_ = t_(i-1)/k
    ah_ = a_(i-1)/k
    th  = t(i)   /k * r_(i-1)
    ah  = a(i)   /k * r_(i-1)
    I_[i-1] = I_[i-1] + th_*I_[i]
    A_[i-1] = A_[i-1] + ah_*I_[i]
    I [i]   = I [i]   + th *I_[i]
    A [i]   = A [i]   + ah *I_[i]
  }
  I_ = 0
  list(I=I, I_=I_, A=A, A_=A_)
}

distribute.radiation.fluxes = function(M) {
  M$A = M$A_ = M$LA = M$LA_ = 0
  N = 5
  n <<- nrow(M)
  p <<- M[,2:7]
  for (i in 1:N) {
    down = with(M, distribute.down(F, F_, A, A_))
    M$F  = down$I
    M$F_ = down$I_
    M$A  = down$A
    M$A_ = down$A_
    up = with(M, distribute.up(F, F_, A, A_))
    M$F  = up$I
    M$F_ = up$I_
    M$A  = up$A
    M$A_ = up$A_
  }
  p <<- M[,8:13]
  for (i in 1:N) {
    down = with(M, distribute.down(LF, LF_, LA, LA_))
    M$LF  = down$I
    M$LF_ = down$I_
    M$LA  = down$A
    M$LA_ = down$A_
    up = with(M, distribute.up(LF, LF_, LA, LA_))
    M$LF  = up$I
    M$LF_ = up$I_
    M$LA  = up$A
    M$LA_ = up$A_
  }
  M
}

thermal.radiation = function(em, T) {
  T0 = 273.15
  s = 5.6704e-8
  v = em*s*(T+T0)^4
  v[is.na(v)] = 0
  v
}

update.radiation.fluxes = function(M) {
  M$F  = M$E
  M$F_ = M$E_
  # Thermal radiation downwards (LF) uses underside long-wave absorptivity (La_) for emissivity
  M$LF  = with(M, LE  + thermal.radiation(La_, T))
  # Thermal radiation upwards (LF_) uses upperside long-wave absorptivity (La) for emissivity
  M$LF_ = with(M, LE_ + thermal.radiation(La, T))
  M$LF_[1] = M$LF[nrow(M)] = 0
  M
}

update.temperatures.layers = function(M, Dt) {
  capacity = function(layer) {
    C = capacities[layer == names(capacities)]
  }
  C =sapply(M$Layer, capacity)
  M$DT = M$NetFlux/C*Dt 
  M$T = M$T + M$DT
  M
}

update.temperatures.air = function(air, Dt) {
  air$DT = air$Q/Cair*Dt  # K = J/m2 / (W/m3/K) s = J / m2 K m3 / W s
  air$T = air$T + air$DT
  air
}

convection.heat.transfer.from.outside = function(Tout, Tlayer, wind.speed=4) {
  h = 7.2 + 3.8*wind.speed
  h*(Tout - Tlayer)
}

# cuberoot = function(x) {
  # sign(x)*abs(x)^(1/3)
# }

convection.heat.transfer.to.inside = function(Touter, Tinner) {
  h = 1.247*abs(Touter - Tinner)^(1/3)
  h*(Touter - Tinner)
}

update.convection.layers = function(M, air) {
  n = nrow(M)
  M$Q[2] = convection.heat.transfer.from.outside(air$T[1], M$T[2]) +
           convection.heat.transfer.to.inside (air$T[2], M$T[2]) 
  for (i in 3:(n-1)) {
    M$Q[i] = convection.heat.transfer.to.inside (air$T[i-1], M$T[i]) + 
             convection.heat.transfer.to.inside (air$T[i],   M$T[i])
  }
  M$Q[n] = convection.heat.transfer.to.inside (air$T[n-1], M$T[n]) 
  M
}

update.convection.air = function(M, air) {
  n = nrow(air)
  for (i in 2:n) {
    air$Q[i] = convection.heat.transfer.from.outside(M$T[i], air$T[i]) +
               convection.heat.transfer.to.inside(M$T[i+1], air$T[i]) 
  }
  air
}

update.net.fluxes = function(M) {
  M = update.radiation.fluxes(M)
  M$NetFlux = with(M, A + A_ + LA + LA_ + Q - (E + E_ +  LF + LF_))
  M
}

with_totals = function(M) {
  cols = 14:ncol(M)
  M = rbind(M,c(rep(NA,13), colSums(M[,cols])))
  M$Layer[nrow(M)] = "Total"
  M[cols] = round(M[cols], 3)
  M
}

#
# Define scenarios
#

scenarios = vector("list", 6)
scenarios[[1]] = layers(c("Sky", "Glass", "ShadeScreen", "Plant", "Floor"))
scenarios[[1]]$E[1] = 400

scenarios[[2]] = layers(c("Sky", "Glass", "GrowthLight", "Plant", "Floor"))
scenarios[[2]]$E[3] = 200

scenarios[[3]] = layers(c("Sky", "Glass", "ShadeScreen", "GrowthLight","Heating","Plant", "Floor"))
scenarios[[3]]$E[1] = 400
scenarios[[3]]$E [4] = 108
scenarios[[3]]$LE[4] = 12
scenarios[[3]]$LE [5] = 75
scenarios[[3]]$LE_[5] = 75
scenarios[[3]]$T = c(10, 0, 0, 0, Inf, 20, 20)

scenarios[[4]] = scenarios[[1]]
scenarios[[4]]$T = c(10, 10, 20, 20, 15)

scenarios[[5]] = scenarios[[2]]
scenarios[[5]]$LE[3] = 20
scenarios[[5]]$LE_[3] = 20
scenarios[[5]]$T = c(10, 10, NA, 20, 20)

#
# Main
#

show.scenario = function(scene, outputs, with.air, levels.order=NA) {
  scene = 3
  outputs = c("Glass")
  i = 1
  with.air = TRUE
  
  M = scenarios[[scene]]
  air = if (with.air) air.set.up(M, 5, 20) else {}
  M = update.radiation.fluxes(M)
  Dt = 60
  steps = 30
  output_cols = which(M$Layer %in% outputs)
  R = matrix(nrow=steps+1, ncol=1+length(output_cols)+length(air$T))
  R[1,] = c(0, M$T[output_cols], air$T)
  
  for (i in 1:steps) {
    M = distribute.radiation.fluxes(M)
    if (with.air) {
      M = update.convection.layers(M, air)
      air = update.convection.air(M, air)
    }
    M = update.net.fluxes(M)
    M = update.temperatures.layers(M, Dt)
    if (with.air)
      air = update.temperatures.air(air, Dt)
    R[i+1,] = c(i*Dt/60, M$T[output_cols], air$T)
  }
  R = data.frame(R)
  colnames(R) = c("Time", M$Layer[output_cols], air$Layer)
  R = melt(R, id.vars="Time", value.name="Temperature", variable.name="Variable")
  levels(R$Variable)
  if (!is.na(levels.order))
    R$Variable = reorder_levels(R$Variable, levels.order)
  levels(R$Variable)
  print(with_totals(M))
  print(air)
  
  windows(6,4)
  title = paste("Scenario", scene, if (with.air) "with" else "without", "convection")
  ggplot(R, aes(Time, Temperature, colour=Variable)) +
    geom_line() +
    labs(title=title, subtitle = paste(M$Layer, collapse=" - ")) +
    theme_minimal()
}

# show.scenario(4, c("Glass", "ShadeScreen"), FALSE)
# show.scenario(4, c("Glass", "ShadeScreen"), TRUE, c(3,1,4,2,5,6))
# show.scenario(5, "Glass", FALSE)

# show.scenario(3, "Glass", FALSE)

