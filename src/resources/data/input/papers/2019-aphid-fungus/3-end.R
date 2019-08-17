sim = read_output(output_file_name)
colnames(sim) = unique_col_names(colnames(sim))

sim$iteration = factor(sim$iteration)
sim$yieldLossWithoutF = 100*(1 - sim$yieldWithoutF)
sim$yieldLossWithF   = 100*(1 - sim$yieldWithF)

# Colourblind-safe from http://colorbrewer2.org/#type=qualitative&scheme=Paired&n=4
col_fill = c('#33a02c', '#b2df8a', '#a6cee3')

density_plot = function(column_without, column_with, x_label) {
  M = data.frame(
    "WithoutFungus" = sim[,column_without],
    "WithFungus"    = sim[,column_with]
  )

  M$Difference = M$WithoutFungus - M$WithFungus
  M = melt(M)
  levels(M$variable) = c("Without fungus", "With fungus", "Difference")

  ggplot(M, aes(x=value, fill=variable)) +
    geom_density(alpha=0.5) +
    scale_fill_manual(values=col_fill, name="") +
    labs(x=x_label, y="") +
    theme_classic() +
    theme(
      axis.text.y = element_blank(),
      axis.ticks.y = element_blank(),
      # legend.position = "none"
      legend.position = "bottom"
    ) +
    guides(fill = guide_legend(reverse = TRUE))
}

simple_density_plot = function(column, x_label) {
  M = data.frame(
    X = sim[,column]
  )

  ggplot(M, aes(x=X)) +
    geom_density(alpha=0.5, fill=col_fill[2]) +
    labs(x=x_label, y="") +
    xlim(0,NA) +
    theme_classic() +
    theme(
      axis.text.y = element_blank(),
      axis.ticks.y = element_blank()
    )
}

print(
  grid.arrange(
    simple_density_plot("cadaverPressure", "Fungus pressure (cadaver-days)"),
    density_plot("aphidPressureWithoutF", "aphidPressureWithF", "Aphid pressure (aphid-days)"),
    density_plot("yieldLossWithoutF", "yieldLossWithF", "Yield loss (%)"),
    ncol=1
  )
)
  