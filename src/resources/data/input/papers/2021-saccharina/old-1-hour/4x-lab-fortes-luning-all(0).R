# library(concaveman)
library(ggforce)

here = function(file_name) {
  box_script_folder = "C:/Users/au152367/Documents/QDev/UniSim2/input/papers/2021-saccharina"
  paste(box_script_folder, file_name, sep="/")
}
 
source(here("0-lab-end-functions.R"))
graphics.off()

load(here("4a-lab-fortes-luning-M1.Rdata"))
obs10 = M1
obs10$I = 10
load(here("4b-lab-fortes-luning-M1.Rdata"))
obs20 = M1
obs20$I = 20
load(here("4c-lab-fortes-luning-M1.Rdata"))
obs30 = M1
obs30$I = 30
load(here("4d-lab-fortes-luning-M1.Rdata"))
obs70 = M1
obs70$I = 70

obs = rbind(obs10, obs20, obs30, obs70)
obs$I = factor(obs$I)
obs$I = reorder_levels(obs$I, 4:1)
obs = subset(obs, alpha<1)
size=1
ggarrange (
  ggplot(obs, aes(gamma, alpha, colour=I)) +
    annotate("rect", xmin=-Inf, xmax=Inf, ymin=0.40, ymax=0.50, fill="#BBBBBB") +
    geom_point(alpha=0.3) +
    geom_mark_hull(expand=0, radius=0, size=size, fill=NA) +
    scale_y_continuous(breaks=(0:10)/10) +
    theme_bw()
  ,
  ggplot(obs, aes(g, alpha, colour=I)) +
    annotate("rect", xmin=-Inf, xmax=Inf, ymin=0.40, ymax=0.50, fill="#BBBBBB") +
    geom_point(alpha=0.3) +
    geom_mark_hull(expand=0, radius=0, size=size, fill=NA) +
    scale_y_continuous(breaks=(0:10)/10) +
    labs(y="") +
    theme_bw()
  ,
  ncol = 2, align="hv", common.legend=TRUE, legend="right"
)  


