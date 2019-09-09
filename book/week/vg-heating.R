graphics.off()
rm(list=ls(all=TRUE))
library(ggplot2)
library(latex2exp)
# library(themes)

a = 0.0154
b = 1.253
d = 35
Cw = 4.181
Vp = pi/4*(d/10)^2*100
k = a*d/Cw/Vp
Ta = 20
L = 6400
V = pi/4*(d/1000)^2*6400
Vdot = 5000
dt = V/Vdot*10^ 6
A = 40*100

dT = function(dt, Tin) {
  Tin - Ta - (k*(b-1)*dt + (Tin - Ta)^(1-b))^(1/(1-b))
}

Tout = function(dt, Tin) {
  Tin - dT(dt,Tin)
}

dT(dt, 60)
Tout(dt, 60)

dE = 4.181*V*100^3*dT(dt, 60)
dE/A/dt

M = expand.grid(Tin=10*(3:9), dt=0:60)
M$dT = dT(60*M$dt, M$Tin)
M$Tout = Tout(60*M$dt, M$Tin)
ggplot(M, aes(x=dt, y=Tout, colour=factor(Tin))) +
  geom_hline(yintercept=20, linetype="longdash") +
  geom_line(size=1.1) +
  labs(x=TeX("$\\Delta t \\, (minutes)$"),
       y=expression(paste(T[out], "  ", (degree*C)))) +
  guides(colour=guide_legend(title=expression(paste(T['in'], " ", (degree*C))))) +
  theme_bw()
  
  