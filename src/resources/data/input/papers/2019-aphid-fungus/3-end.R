# Use stored or fresh simulation output
if (exists("use_stored_simulation")) {
  load("sim-2048.Rdata")
} else {
  sim = read_output(output_file_name)
}

# Prepare data
sim$iteration = factor(sim$iteration)
sim$yieldLossUncontrolled = 100*(1 - sim$yieldUncontrolled.end)
sim$yieldLossControlled    = 100*(1 - sim$yieldControlled.end)

col_fill = c('#33a02c', '#b2df8a', '#a6cee3')
bw_fill = c('darkgray', 'lightgrey', 'white')

theme2 = theme(
  panel.background = element_rect(fill="white"),
  panel.grid = element_blank(),
  axis.text.y = element_blank(),
  axis.ticks.y = element_blank()
)

left_margin = 0

simple_density_plot = function(column, x_label, top_margin, bottom_margin, tint_fill) {
  M = data.frame(
    X = sim[,column]
  )

  ggplot(M, aes(x=X)) +
    geom_density(alpha=0.5, fill=tint_fill[2]) +
    geom_vline(xintercept=0) +
    labs(x=x_label, y="") +
    xlim(0,220) +
    theme1 +
    theme2 +
    theme(
      plot.margin = unit(c(top_margin,5,bottom_margin,left_margin), "mm")
    ) 
}

density_plot = function(column_without, column_with, x_label, legend_position, top_margin, bottom_margin, tint_fill) {
  M = data.frame(
    "WithoutFungus" = sim[,column_without],
    "WithFungus"    = sim[,column_with]
  )

  M$Difference = M$WithoutFungus - M$WithFungus
  M = melt(M)
  levels(M$variable) = c("Without fungus", "With fungus", "Difference")
  
  ggplot(M, aes(x=value, fill=variable)) +
    geom_density(alpha=0.5) +
    geom_vline(xintercept=0) +
    scale_fill_manual(values=tint_fill, name="") +
    labs(x=x_label, y="") +
    theme1 +
    theme2 +
    theme2 +
    theme(
      legend.position = legend_position,
      legend.direction = "horizontal",
      legend.box.margin = unit(c(2,0,0,-5), "mm"),
      plot.margin = unit(c(top_margin,5,bottom_margin,left_margin), "mm") 
    ) +
    guides(fill = guide_legend(reverse = TRUE))
}

make_plot = function(tint_fill) {
  P = grid.arrange(
    simple_density_plot("cadaverDays.end", "Fungus pressure (cadaver-days)", 5, 5, tint_fill),
    density_plot("aphidDaysUncontrolled.end", "aphidDaysControlled.end", "Aphid pressure (aphid-days)", "none", -1, 11, tint_fill),
    density_plot("yieldLossUncontrolled", "yieldLossControlled", "Yield loss (%)", "bottom", -7, 7, tint_fill),
    ncol=1
  )
  annotate_figure(P, left=text_grob("          Density distribution of simulation outcomes", size=10, rot=90))
}

# Show on screen
open_plot_window(3.2, 5.0)
make_plot(col_fill)

# Write two graphics files
tiff("output/fig-4-bw.tiff", width=86, height=134, units="mm", res=1200, compression="zip")
grid.draw(make_plot(bw_fill))
dev.off()

tiff("output/fig-4-col.tiff", width=86, height=134, units="mm", res=1200, compression="zip")
grid.draw(make_plot(col_fill))
dev.off()
