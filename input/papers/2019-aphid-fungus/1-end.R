sim = read_output(output_file_name)
sim$iteration = factor(sim$iteration)
sim$Date = as.POSIXct( days(yday(sim$date)), origin="2000-12-31", tz="GMT" )

# Colourblind-safe from http://colorbrewer2.org/#type=qualitative&scheme=Paired&n=4
col_areas = c('#33a02c', '#b2df8a', '#a6cee3')
col_lines = "#1f78b4"

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

print(
  ggplot(M, aes(x=CropGrowthStage, y=Density)) +
    geom_area(aes(fill=Population), position="identity", alpha=1, color="black") +
    scale_fill_manual(values=col_areas, name="Aphid density") + 
    geom_line(data=Y, aes(y=YieldLoss, linetype=Fungus), colour=col_lines) +
    scale_linetype_manual(values=2:1, name="Yield loss") + 
    labs(x="Crop growth stage (Zadoks scale)", y="Aphid density (per tiller)") +
    scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
    scale_y_continuous(sec.axis = sec_axis(~.*0.5, name="Yield loss (%)")) +
    theme_classic() 
)
