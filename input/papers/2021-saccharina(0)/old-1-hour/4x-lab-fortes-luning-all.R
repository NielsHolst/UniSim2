setwd("C:/Users/au152367/Documents/QDev/UniSim2/input")
source("scripts/begin.R")

library(ggforce)

here = function(file_name) {
  paste("papers/2021-saccharina", file_name, sep="/")
}
 

load(here("4a-lab-fortes-luning-M1.Rdata"))
obs10 = M1
obs10$I = 10
load(here("4b-lab-fortes-luning-M1.Rdata"))
obs20 = M1
obs20$I = 20
load(here("4c-lab-fortes-luning-M1.Rdata"))
obs30 = M1
obs30$I = 30


obs = rbind(obs10, obs20, obs30)
obs$Irradiation = factor(obs$I)
# obs$I = reorder_levels(obs$I, 4:1)
# obs = subset(obs, alpha<1)
size=1
ggarrange (
  ggplot(obs, aes(Pmax, alpha, colour=Irradiation)) +
    annotate("rect", xmin=0.0017, xmax=0.0018, ymin=0.06, ymax=0.10, fill="#BBBBBB") +
    geom_point(alpha=0.3) +
    geom_mark_hull(expand=0, radius=0, size=size, fill=NA) +
    scale_colour_manual(values=c(brown, blue, green)) +
    labs(x="") +
    theme_bw()
  ,
  ggplot(obs, aes(Pmax, gamma, colour=Irradiation)) +
    annotate("rect", xmin=0.0017, xmax=0.0018, ymin=min(obs$gamma), ymax=max(obs$gamma), fill="#BBBBBB") +
    geom_point(alpha=0.3) +
    geom_mark_hull(expand=0, radius=0, size=size, fill=NA) +
    scale_colour_manual(values=c(brown, blue, green)) +
    theme_bw()
  ,
  ncol = 1, align="hv", common.legend=TRUE, legend="bottom"
)  


