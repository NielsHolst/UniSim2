# Use stored or fresh simulation output
if (exists("use_stored_simulation")) {
  load("fig2.Rdata")
} else {
  sim = read_output(output_file_name)
  theme1 = NULL
}
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

make_plot = function(tint_areas, tint_lines, line_size) {
  ggplot(M, aes(x=CropGrowthStage, y=Density)) +
    geom_area(aes(fill=Population), position="identity", alpha=1, color="black", size=0.1) +
    scale_fill_manual(values=tint_areas, name="Aphid density") + 
    geom_line(data=Y, aes(y=YieldLoss, linetype=Fungus), colour=tint_lines, size=line_size) +
    scale_linetype_manual(values=c('dotted', 'solid'), name="Yield loss") + 
    labs(x="Crop growth stage (Zadoks scale)", y="Aphids per tiller") +
    scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
    scale_y_continuous(sec.axis = sec_axis(~.*0.5, name="Yield loss (%)")) +
    theme_classic() 
}

# Show on screen
open_plot_window(7,4)
print(make_plot(col_areas, col_lines, 1))

# Write two graphics files
if (exists("use_stored_simulation")) {
  # Black and white
  tiff("output/fig-2-bw.tiff", width=86, height=75, units="mm", res=1200, compression="zip")
  print(make_plot(bw_areas, bw_lines, 0.5) + theme1)
  dev.off()
  # Coloured
  tiff("output/fig-2-colour.tiff", width=86, height=75, units="mm", res=1200, compression="zip")
  print(make_plot(col_areas, col_lines, 0.5) + theme1)
  dev.off()
  # Black and white on screen
  open_plot_window(3.2, 2.8)
  print(make_plot(bw_areas, bw_lines, 0.5) + theme1)
}

