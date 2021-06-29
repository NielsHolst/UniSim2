source(paste0(box_script_folder,"/configure.R"))
sim = read_output(output_file_name)

bw = c("white", "grey", "black")

Acols = c("cropGrowthStage", "susceptible", "exposed", "cadavers")
A = melt(sim[,Acols], .(cropGrowthStage), value.name="Density", variable.name="Population")

Pcols = c("cropGrowthStage", "prevalence", "cadaverPrevalence")
P = melt(sim[,Pcols], .(cropGrowthStage), value.name="Prevalence", variable.name="Type")


make_plot = function(line_size=1) {
  ggarrange(
    ggplot(A, aes(x=cropGrowthStage)) +
      geom_area(aes(y=Density, fill=Population), colour="black", size=1) +
      geom_line(aes(y=uncontrolled), linetype="dashed", data=sim) +
      scale_fill_manual(values=bw) +
      labs(x="", y="Individuals per tiller") +
      scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
      theme_classic() +
      theme (
        legend.position = "none"
      )
    ,
    ggplot(subset(P, cropGrowthStage<80) , aes(x=cropGrowthStage, Prevalence, colour=Type)) +
      geom_line() +
      scale_colour_manual(values=bw[2:3]) +
      labs(x="Crop growth stage (Zadoks scale)") +
      scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
      theme_classic() +
      theme (
        legend.position = "none"
      )
    ,
    ncol=1, align="h"
  )
}

open_plot_window(7,4)
print(make_plot())

# Dimensions
mm = function(x) x/25.4
W = 84
H = 135

# Black and white for manuscript
png("fig-2-bw.png", width=W, height=H, units="mm", res=1200, type="cairo-png")
print(make_plot(0.5) + theme1)
dev.off()

# Black and white for journal
mm = function(x) x/25.4

cairo_ps("fig-2-bw.eps", width=mm(W), height=mm(H))
print(make_plot(0.5) + theme1)
dev.off()

