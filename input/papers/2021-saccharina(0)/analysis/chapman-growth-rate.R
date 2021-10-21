rm(list=ls(all=TRUE))
graphics.off()

library(drc)
library(ggplot2)
library(ggpubr)
library(plyr)
library(reshape2)


setwd("/Users/au152367/Documents/QDev/UniSim2/input")
source("scripts/begin.R")

here = function(file_name) {
  paste0("papers/2021-saccharina/obs/", file_name)
}

CN = read.table(here("C-N-chapman-et-al-1978-CN.txt"), TRUE, "\t")
G = read.table(here("growth-rate-chapman-et-al-1978.txt"), TRUE, "\t")

M = cbind(
  G, 
  N1 = CN[2*(0:5)+1, "N"],
  N2 = CN[2*(0:5)+2, "N"]
)
M = adply(M, 1, mutate, gMean=mean(c(gLow, gHigh)), Nmean=mean(c(N1, N2)))

model = drm(gMean~Nmean, data=M, fct=LL.4())
co = coef(model)
b = co[1]
c = co[2]
d = co[3]
e = co[4]
ymax = d

f = function(x) {
  c + (d-c)/(1 + exp(b*(log(x) - log(e))))
}
x = (0:105)/2000
S = data.frame(
  X = x,
  Y = f(x)
)

c = c/ymax
d = 1
Ssc = data.frame(
  X = x,
  Y = f(x)
)

g = function(y) {
  y = y/ymax
}

Msc = M
Msc$gLow = g(M$gLow)
Msc$gHigh = g(M$gHigh)
Msc$gMean = g(M$gMean)


my_plot = function(M, S, ylab) {
  ggplot(M) +
    geom_segment(aes(x=N1, xend=N2, y=gMean, yend=gMean), size=1, colour=red) +
    geom_segment(aes(x=Nmean, xend=Nmean, y=gLow, yend=gHigh), size=1, colour=red) +
    geom_line(aes(X,Y), data=S, colour=blue) +
    labs(x="Nitrogen reserves (Nres)", y=ylab) +
    theme_bw()
}

ggarrange(
  my_plot(M, S, "Growth rate (per day)"),
  my_plot(Msc, Ssc, "Growth rate scaling") + 
    scale_y_continuous(breaks=c(0.545,0.6,0.8,1.0)) +
    geom_hline(yintercept=0.545, linetype="dashed"),
  ncol=1, align="hv"
)



