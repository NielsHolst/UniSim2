rm(list=ls(all=TRUE))

setwd("/Users/au152367/Documents/QDev/UniSim2/input/student/saccharina/obs")
obs = read.table("alge-obs-2019-AVG-vandrette-liner-100m.txt", header=TRUE, sep="\t")

Cstruct = 0.2
Nstruct = 0.01
kN = 2.72
kC = 2.12

estimate = function(pctC, pctN) {
  a = (Cstruct - pctC)/(pctC*kC - 1)
  b = pctC*kN/(pctC*kC - 1)/(pctN*kN - 1)
  C = (a - b*(Nstruct-pctN)) / (b*pctN*kC)
  data.frame(
    C = C,
    N = (Nstruct - pctN - pctN*C*kC) / (pctN*kN - 1)
  )
}

estimate(25,5)

pctC=25
pctN=5
  