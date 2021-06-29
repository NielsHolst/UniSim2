library(ggplot2)
library(ggpubr)
library(plyr)
library(reshape2)
rm(list=ls(all=TRUE))
graphics.off()

setwd("/Users/au152367/Documents/QDev/UniSim2/input")

here = function(file_name) {
  paste0("papers/2021-saccharina/analysis/", file_name)
}

source(here("setup.R"))
source(here("C-N-functions.R"))

obs = read.table(here("gevaert-et-al-2001.txt"), TRUE, ";")
max_length = data.frame(
  S=60, 
  D=100,
  MA=130,
  J=150
)

reorder_levels = function(the_factor, new_order) {
  factor(the_factor,levels(the_factor)[new_order])
}

x = 1:150 # (1:200)/10
f = function(o) {
  M = data.frame(
    Length = x,
    Extrapolation = FALSE,
    Value = o$a*x^o$b
  )
  M$Extrapolation[M$Length>max_length[1,o$Month]] = TRUE
  M
}

my_plot = function(M) {
  ggplot(subset(M, !Extrapolation), aes(Length, Value, colour=Month)) +
    geom_line() +
    geom_line(data=subset(M, Extrapolation), linetype="dotted") +
    theme_bw() +
    labs(y="") +
    facet_wrap(~Variable, ncol=1, scales="free")
}

M = adply(obs, 1, f)
M$Variable = reorder_levels(factor(M$Variable), c(2,1,3))
M$Month = reorder_levels(factor(M$Month), c(4,1,3,2))
my_plot(M)

W = dcast(M, Month+Length+Extrapolation~Variable, value.var="Value")
W$CarbonPct = with(W, CarbonWeight/DryWeight/1000*100)
W$NitrogenPct = with(W, NitrogenWeight/DryWeight/1000*100)
CN = solveCN(W$CarbonPct,W$NitrogenPct)
W = cbind(W, CN)
colnames(W)[7:10] = c("C%","N%","Cres","Nres")

windows()
M2 = melt(W[,c("Month","Length","Extrapolation","C%","N%","Cres","Nres")], id.vars=c("Month","Length","Extrapolation"), variable.name="Variable", value.name="Value")
my_plot(M2)

Gevaert = subset(M2, Month=="S" & Variable=="C%" & !Extrapolation)
save(Gevaert, file=here("gevaert-Sep-Cpct.Rdata"));