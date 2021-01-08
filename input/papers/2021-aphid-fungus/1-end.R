source(paste0(box_script_folder,"/configure.R"))

sim = read_output(output_file_name)
sim$iteration = factor(sim$iteration)

# Colourblind-safe from http://colorbrewer2.org/#type=qualitative&scheme=Paired&n=4
col_areas = c('#33a02c', '#b2df8a', '#a6cee3')
col_lines = "#1f78b4"

bw_areas = c("white", "grey", "black")
bw_lines = "black"

# Pick columns to plot
M = sim[,c("iteration", 
           "value", 
           "withoutFungus.density.total",
           "withFungus.density.total",
           "withFungus.infectious.cadavers.content")]
colnames(M) = c("Iteration", "CropGrowthStage", "Without fungus", "With fungus", "Cadavers")
M = melt(M, id.vars = c("Iteration", "CropGrowthStage"), value.name="Density", variable.name="Population")

Y = sim[,c("iteration", 
           "value", 
           "withoutFungus.diagnostics.yield.loss",
           "withFungus.diagnostics.yield.loss")]
colnames(Y) = c("Iteration", "CropGrowthStage", "Without fungus", "With fungus")
Y$'Without fungus' = 200*Y$'Without fungus'
Y$'With fungus'    = 200*Y$'With fungus'
Y = melt(Y, id.vars = c("Iteration", "CropGrowthStage"), value.name="YieldLoss", variable.name="Fungus")


ix_max = which.max(sim$withFungus.infectious.cadavers.content)
fmax = max(sim$withFungus.infectious.cadavers.content)
gs_fmax = sim$value[ix_max]
fmax
gs_fmax

print_statistics = function() {
  prevalence = sim$withFungus.infectious.cadavers.content[ix_max] / (sim$withFungus.infectious.cadavers.content[ix_max] + sim$withFungus.density.total[ix_max])
  print(paste("Prevalence at max. density =", round(100*prevalence,1), "%"))
  
  n = nrow(sim)
  yield_loss_effect = round(100*(sim$withoutFungus.diagnostics.yield.loss[n] - sim$withFungus.diagnostics.yield.loss[n]),1)
  print(paste("Yield loss effect =", yield_loss_effect, "% points"))
  
  area_without = sum(sim$withoutFungus.density.total)
  area_with = sum(sim$witFungus.density.total)
  aphid_effect = area_without - area_with
  print(paste("Aphid effect =", round(aphid_effect,0), "aphid-days"))
}
print_statistics()

make_plot = function(tint_areas, tint_lines, tint_arrow, line_size) {
  ggplot(M, aes(x=CropGrowthStage, y=Density)) +
    geom_area(aes(fill=Population), position="identity", alpha=1, color="black", size=0.1) +
    scale_fill_manual(values=tint_areas, name="Population density") + 
    geom_line(data=Y, aes(y=YieldLoss, linetype=Fungus), colour=tint_lines, size=line_size) +
    scale_linetype_manual(values=c('dotted', 'solid'), name="Yield loss") + 
    annotate("segment", xend=gs_fmax, yend=fmax, x=gs_fmax, y=0, arrow=arrow(length=unit(0.06, "inches"),type="open"), size=line_size, colour=tint_arrow) +
    labs(x="Crop growth stage (Zadoks scale)", y="Individuals per tiller") +
    scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
    scale_y_continuous(sec.axis = sec_axis(~.*0.5, name="Yield loss (%)")) +
    theme_classic() 
}


# Show on screen
open_plot_window(7,4)
print(make_plot(col_areas, col_lines, "black", 1))

# Write two graphics files
setwd(output_figures_folder)
print(paste("Writing figures as tiff files to", getwd()))

tiff("fig-2-bw.tiff", width=86, height=75, units="mm", res=1200, compression="zip")
print(make_plot(bw_areas, bw_lines, "white", 0.5) + theme1)
dev.off()

tiff("fig-2-colour.tiff", width=86, height=75, units="mm", res=1200, compression="zip")
print(make_plot(col_areas, col_lines, "black", 0.5) + theme1)
dev.off()

