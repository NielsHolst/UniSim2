source(paste0(box_script_folder,"/configure.R"))
sim = read_output(output_file_name)

graphics.off()
bw = c("white", "grey", "black")

Acols = c("cropGrowthStage", "susceptible", "exposed", "cadavers")
A = melt(sim[,Acols], .(cropGrowthStage), value.name="Density", variable.name="Population")

Pcols = c("cropGrowthStage", "prevalence", "cadaverPrevalence")
P = melt(sim[,Pcols], .(cropGrowthStage), value.name="Prevalence", variable.name="Type")


make_plot = function(size1, size2) {
  ggarrange(
    ggplot(A, aes(x=cropGrowthStage)) +
      geom_area(aes(y=Density, fill=Population), colour="black", size=0.1) +
      geom_line(aes(y=uncontrolled), size=size1, linetype="dashed", data=sim) +
      scale_fill_manual(values=bw) +
      labs(x="", y="Aphids per tiller") +
      scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
      theme1 +
      theme (
        legend.position = "none"
      ) 
    ,
    ggplot(subset(P, cropGrowthStage<80) , aes(x=cropGrowthStage, Prevalence, colour=Type)) +
      geom_line(size=size2) +
      scale_colour_manual(values=bw[2:3]) +
      labs(x="Crop growth stage", y="Prevalence (%)") +
      scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
      theme1 +
      theme (
        legend.position = "none"
      ) 
    ,
    ncol=1, align="hv", labels="auto"
  )
}

# Dimensions
mm = function(x) x/25.4
W = 84
H = 110

# Screen plot
open_plot_window(mm(W),mm(H))
print(make_plot(1,1))

# File destination
setwd(output_figures_folder)

# Black and white for manuscript
png("fig-2-bw.png", width=W, height=H, units="mm", res=1200, type="cairo-png")
print(make_plot(0.3,1))
dev.off()

# Black and white for journal
cairo_ps("fig-2-bw.eps", width=mm(W), height=mm(H))
print(make_plot(0.3,1))
dev.off()

