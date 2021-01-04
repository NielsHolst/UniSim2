library(ggplot2)
library(ggpubr)
library(plyr)

rm(list=ls(all=TRUE))
graphics.off()

setwd("/Users/au152367/Documents/QDev/UniSim2/input/student/saccharina/obs")
fileName = "alge-obs-2019-AVG-vandrette-liner-100m.txt"
# fileName = "alge-obs-2019-AVG.txt"

obs = read.table(fileName, header=TRUE, sep="\t")
obs = subset(obs, carbonPct>0 & nitrogenPct>0)

Wstruct = 10
Cstruct = 0.21 #0.2
Nstruct = 0.02 #0.01
kN = 2.72
kC = 2.12
Cmax = 0.8
Nmax = 0.073

n = 100
Cvalues = seq(0, Cmax, length.out=n)
Nvalues = seq(0, Nmax, length.out=n)

model = expand.grid(C = Cvalues, N = Nvalues)

pctC = function(C,N) {
  100*(C + Cstruct) / (1 + C*kC + N*kN)
} 

pctN = function(C,N) {
  100*(N + Nstruct) / (1 + C*kC + N*kN)
} 

C = function(pctN, N) {
  pctN = pctN/100
  (N + Nstruct - pctN -pctN*N*kN) / (pctN*kC)
}

N = function(pctC, C) {
  pctC = pctC/100
  (C + Cstruct - pctC -pctC*C*kC) / (pctC*kN)
}

solveCN = function(pctC, pctN) {
  pctN = pctN/100
  pctC = pctC/100
  solC = (kN*pctC*Nstruct - kN*pctN*Cstruct - pctC + Cstruct) / (kC*pctC + kN*pctN - 1)
  solN = N(100*pctC,solC)
  signif(data.frame(C=solC, N=solN),3)
}

EstimateN = function(pctC) {
  data.frame(
    C = Cvalues,
    N = N(pctC, Cvalues)
  )
}
# EstimateN(25)

EstimateC = function(pctN) {
  data.frame(
    N = Nvalues,
    C = C(pctN, Nvalues)
  )
}
# EstimateC(4.9)

model$PctC = pctC(model$C, model$N)
model$PctN = pctN(model$C, model$N)

plotCN = function(pctC, pctN) {
  ggarrange(
    ggplot(model, aes(C,N)) +
      geom_tile(aes(fill=PctC)) +
      geom_line(data=EstimateN(pctC), colour="yellow", size=1) +
      annotate("label", x=0.6, y=0.06, label=paste("%N =", pctN)) +
      ylim(0,Nmax)
    ,
    ggplot(model, aes(C,N)) +
      geom_tile(aes(fill=PctN)) +
      geom_line(data=EstimateC(pctN), colour="yellow", size=1) +
      annotate("label", x=0.6, y=0.06, label=paste("%C =", pctC)) +
      xlim(0,Cmax)
    ,
    ggplot(model, aes(C,N)) +
      geom_line(data=EstimateN(pctC), size=1) +
      geom_line(data=EstimateC(pctN), size=1) +
      annotate("point", x=solveCN(pctC,pctN)$C, y=solveCN(pctC,pctN)$N, colour="red", size=2) +
      ylim(0,Nmax) +
      xlim(0,Cmax)
    ,
    ncol=3
  )  
}

plots = alply(obs, 1, function(x) plotCN(x$carbonPct, x$nitrogenPct))
ggarrange(plotlist=plots, ncol=1)
# plotCN(25,4.9)

obs = adply(obs, 1, function(x) solveCN(x$carbonPct, x$nitrogenPct))
obs

outFileName = paste0(substr(fileName,1,nchar(fileName)-4), "-CN.txt")
write.table(obs, outFileName, quote=FALSE, row.names=FALSE, sep="\t")
