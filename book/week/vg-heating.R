graphics.off()
rm(list=ls(all=TRUE))
library(ggplot2)
library(latex2exp)

setwd("C:\\Users\\au152367\\Documents\\QDev\\UniSim2\\book\\graphics\\vg")

Tair = 20

Tout = function(k, d, b, Dt, Tin) {
    Tair + (k*d*(b-1)*Dt + (Tin - Tair)^(1-b))^(1/(1-b))
}

P = function(k, d, b, Dt, Tin, flowRate) {
  Cw = 4184
  DT = Tin - Tout(k, d, b, Dt, Tin)
  DT*flowRate*Cw/3600  # kW
}

plot_Tout = function() {
  M = expand.grid(Dt=0:30, Tin=10*(3:8))
  M$Tout = Tout(1.7e-4, 37, 1.25, M$Dt, M$Tin)

  windows(4,2.5)
  ggplot(M, aes(x=Dt, y=Tout, colour=factor(Tin))) +
    geom_hline(yintercept=20, size=1.1, colour="darkgrey") +
    geom_line(size=1.1) +
    labs(x=TeX("$\\Delta t \\, (min)$"),
         y=expression(paste(T[out], "  ", (degree*C)))) +
    guides(colour=guide_legend(reverse=TRUE,
                               title=expression(paste(T['in'], " ", (degree*C))))) +
    scale_x_continuous(breaks=10*(0:6)) +
    scale_y_continuous(breaks=10*(2:8)) +
    theme_bw() 
}

plot_effect = function() {
  Tin=20:80
  A = 3024
  top = data.frame(
    Tin = Tin,
    Effect = P(1.48e-4, 36.7, 1.39, 15, Tin, 25.98)/A*1000
  )
  top$Pipe = "Top"
  bottom = data.frame(
    Tin = Tin,
    Effect = P(1.66e-4, 43.5, 1.23, 23, Tin, 20.48)/A*1000
  )
  bottom$Pipe = "Bottom"
  M = rbind(top,bottom)
  M$Pipe = factor(M$Pipe)

  print(paste("Max. total effect =", max(top$Effect) + max(bottom$Effect)))
  
  windows(4,2.5)
  ggplot(M, aes(x=Tin, y=Effect, colour=Pipe)) +
    geom_line(size=1.1) +
    labs(x=TeX("$T_{in} \\, (\\degree C)$"),
         y=TeX("Effect (P; $W/m^2$)")) +
    guides(colour=guide_legend(reverse=TRUE, title="Pipe system")) +
    # scale_x_continuous(breaks=10*(0:6)) +
    # scale_y_continuous(breaks=10*(2:8)) +
    theme_bw() 
}

plot_effect()


