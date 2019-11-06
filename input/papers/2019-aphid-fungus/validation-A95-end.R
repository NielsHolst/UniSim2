# Additional packages needed
library(ggpubr)
library(grid)

# Use stored or fresh simulation output
if (exists("use_stored_simulation")) {
  load("fig3.Rdata")
} else {
  sim = read_output(output_file_name)
  theme1 = NULL
}
sim$iteration = factor(sim$iteration)
sim$Date = as.POSIXct( days(yday(sim$date)), origin="2000-12-31", tz="GMT" )

load(paste0(box_script_folder, "/validation/A95.Rdata"))
obs = subset(A95, Species=="S.avenae")
obs$Date = as.POSIXct( days(yday(obs$Date)), origin="2000-12-31", tz="GMT" )
obs$Aphids = obs$Apterous + obs$Alate
obs


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
M2 = melt(obs, id.vars = c("Date"), 
          measure.vars = c("Aphids", "Cadavers"),
          value.name="Density", variable.name="Population")
M2$Iteration = 1

make_plot = function(tint_lines, line_size, a_theme) {
  from_date = as.POSIXct(dmy("1/5/2001"))
  to_date   = as.POSIXct(dmy("31/8/2001"))
  ggplot(M1, aes(x=Date, y=log10(Density+1), colour=Population, group=interaction(Iteration,Population))) +
    geom_line(alpha=0.5) +
    geom_point(data=M2, size=3, aes(shape=Population)) +
    scale_colour_manual(values=tint_lines, name="") + 
    scale_size_manual(values=line_size, name="") +
    labs(x="Date", y="log10(Density+1)") +
    scale_x_datetime(breaks = date_breaks('months'), labels = date_format('%b'), limits = c(from_date, to_date)) +
    theme_classic() +
    a_theme +
    theme(legend.position = "bottom", 
          legend.direction = "horizontal",
          plot.margin = unit(c(0,5,5,5), "mm")
         )
}

# open_plot_window(3.2, 5.0)
grid.draw(make_plot(col_lines, col_line_size, theme1)) 

if (exists("use_stored_simulation")) {
  tiff("output/fig-3-bw.tiff", width=86, height=134, units="mm", res=1200, compression="zip")
  grid.draw(make_plot(bw_lines, bw_line_size, theme1))
  dev.off()

  tiff("output/fig-3-col.tiff", width=86, height=134, units="mm", res=1200, compression="zip")
  grid.draw(make_plot(col_lines, bw_line_size, theme1))
  dev.off()
}
