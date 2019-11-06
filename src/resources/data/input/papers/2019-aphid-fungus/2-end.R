# Use stored or fresh simulation output
if (exists("use_stored_simulation")) {
  load("fig3.Rdata")
} else {
  sim = read_output(output_file_name)
}
sim$iteration = factor(sim$iteration)
sim$Date = as.POSIXct( days(yday(sim$date)), origin="2000-12-31", tz="GMT" )

# Colourblind-safe from http://colorbrewer2.org/#type=qualitative&scheme=Paired&n=4
col_lines = c('#33a02c', '#1f78b4')
bw_lines = c('black', 'grey')
col_line_size = c(1,1)
bw_line_size = c(1,1)

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

make_plot = function(tint_lines, line_size, a_theme) {
  from_date = as.POSIXct(dmy("1/5/2001"))
  to_date   = as.POSIXct(dmy("31/8/2001"))
  P = grid.arrange(
    ggplot(M1, aes(x=Date, y=Density, colour=Population, size=Population, group=interaction(Iteration,Population))) +
      geom_line(alpha=0.5) +
      scale_colour_manual(values=tint_lines, name="") + 
      scale_size_manual(values=line_size, name="") +
      labs(x="Date", y=NULL) +
      scale_x_datetime(breaks = date_breaks('months'), labels = date_format('%b'), limits = c(from_date, to_date)) +
      theme_classic() +
      a_theme +
      theme(legend.position = "none",
            plot.margin = unit(c(8,5,5,5), "mm")
           )
    ,
    ggplot(M2, aes(x=CropGrowthStage, y=Density, colour=Population, size=Population, group=interaction(Iteration,Population))) +
      geom_line(alpha=0.5) +
      scale_colour_manual(values=tint_lines, name="") + 
      scale_size_manual(values=line_size, name="") +
      labs(x="Crop growth stage (Zadoks scale)", y=NULL) +
      scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
      theme_classic() +
      a_theme +
      theme(legend.position = "bottom", 
            legend.direction = "horizontal",
            plot.margin = unit(c(0,5,5,5), "mm")
           )
    ,
    ncol=1
  )
  annotate_figure(P, left=text_grob("              Aphids per tiller", size=10, rot=90))
}

open_plot_window(3.2, 5.0)
grid.draw(make_plot(bw_lines, bw_line_size, theme1)) 

tiff("output/fig-3-bw.tiff", width=86, height=134, units="mm", res=1200, compression="zip")
grid.draw(make_plot(bw_lines, bw_line_size, theme1))
dev.off()

tiff("output/fig-3-col.tiff", width=86, height=134, units="mm", res=1200, compression="zip")
grid.draw(make_plot(col_lines, bw_line_size, theme1))
dev.off()
