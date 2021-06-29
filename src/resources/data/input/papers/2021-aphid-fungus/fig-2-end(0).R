source(paste0(box_script_folder,"/configure.R"))
sim = read_output(output_file_name)

bw_areas = c("white", "grey", "black")

Acols = c("cropGrowthStage", "withoutFungusAphidDensity", "withFungusAphidDensity", "cadaverDensity")
A = melt(sim[,Acols], .(cropGrowthStage), value.name="Density", variable.name="Population")
levels(A$Population) =  c("Without fungus", "With fungus", "Cadavers")

Ycols = c("cropGrowthStage", "withoutFungusYieldLoss", "withFungusYieldLoss")
Y = melt(sim[,Ycols], .(cropGrowthStage), value.name="YieldLoss", variable.name="Scenario")
Y$YieldLoss = 100*Y$YieldLoss
levels(Y$Scenario) =  c("Without fungus", "With fungus")

Pcols = c("cropGrowthStage", "prevalence", "cadaverPrevalence")
P = melt(sim[,Pcols], .(cropGrowthStage), value.name="Prevalence", variable.name="Type")


make_plot = function(line_size=1) {

  ggplot(A, aes(x=cropGrowthStage, y=Density)) +
    geom_area(aes(fill=Population), position="identity", color="black", size=0.1) +
    # geom_line(aes(group=Population), size=0.1) +
    geom_line(data=Y, aes(y=2*YieldLoss, linetype=Scenario), size=line_size) +
    scale_fill_manual(values=bw_areas, name="Population density") + 
    scale_linetype_manual(values=c('dotted', 'solid'), name="Yield loss") + 
    labs(x="Crop growth stage (Zadoks scale)", y="Individuals per tiller") +
    scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
    scale_y_continuous(sec.axis = sec_axis(~.*0.5, name="Yield loss (%)")) +
    theme_classic() +
    theme (
      legend.position = "bottom",
      legend.direction = "vertical"
    )
}

open_plot_window(7,4)
print(make_plot())

# tiff("fig-2-bw.tiff", width=84, height=75, units="mm", res=1200, compression="zip")
# print(make_plot(0.5) + theme1)
# dev.off()

mm = function(x) {
  x/25.4
}

# Black and white for manuscript
png("fig-2-bw.png", width=84, height=75, units="mm", res=1200, type="cairo-png")
print(make_plot(0.5) + theme1)
dev.off()

# Black and white for journal
cairo_ps("fig-2-bw.eps", width=mm(84), height=mm(75))
print(make_plot(0.5) + theme1)
dev.off()


# ggsave(
  # filename = "fig-2-bw.eps",
  # plot = print(make_plot(0.5)),
  # device = "eps",
  # width = unit(84, "mm"),
  # height = unit(75, "mm")
# )
