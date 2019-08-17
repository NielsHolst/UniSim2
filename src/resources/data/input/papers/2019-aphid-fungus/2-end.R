library(grid)

sim = read_output(output_file_name)
sim$iteration = factor(sim$iteration)
sim$Date = as.POSIXct( days(yday(sim$date)), origin="2000-12-31", tz="GMT" )

# Colourblind-safe from http://colorbrewer2.org/#type=qualitative&scheme=Paired&n=4
col_lines = c('#b2df8a', '#a6cee3')
col_lines = c('#33a02c', '#1f78b4')

# Pick columns to plot
M = sim[,c("iteration", 
           "Date",
           "value", 
           "withFungus.density.total",
           "withFungus.infectious.cadavers.content")]
colnames(M) = c("Iteration", "Date", "CropGrowthStage", "Aphids", "Cadavers")
M1 = melt(M, id.vars = c("Iteration", "Date"), 
          measure.vars = c("Aphids", "Cadavers"),
          value.name="Density", variable.name="Population")
M2 = melt(M, id.vars = c("Iteration", "CropGrowthStage"), 
          measure.vars = c("Aphids", "Cadavers"),
          value.name="Density", variable.name="Population")
levels(M2$Population) = c("Live aphids", "Cadavers")

from_date = as.POSIXct(dmy("1/5/2001"))
to_date   = as.POSIXct(dmy("31/8/2001"))
P = grid.arrange(
    ggplot(M1, aes(x=Date, y=Density, colour=Population, group=interaction(Iteration,Population))) +
      geom_line(alpha=0.5) +
      scale_colour_manual(values=col_lines, name="") + 
      labs(x="Date", y="Aphid density (per tiller)") +
      scale_x_datetime(breaks = date_breaks('months'), labels = date_format('%b'), limits = c(from_date, to_date)) +
      theme_classic() +
      theme(legend.position = "none")
    ,
    ggplot(M2, aes(x=CropGrowthStage, y=Density, colour=Population, group=interaction(Iteration,Population))) +
      # geom_line(alpha=1, size=1.1) +
      geom_line(alpha=0.5) +
      scale_colour_manual(values=col_lines, name="") + 
      labs(x="Crop growth stage (Zadoks scale)", y="Aphid density (per tiller)") +
      scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
      theme_classic() +
      theme(legend.position = "none")
      # theme(legend.position = "bottom") 
    ,
    ncol=1
  )

print(P)
