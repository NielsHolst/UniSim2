sim = read_output(output_file_name)

sim$ObsDiffuseFraction = 0
ix = sim$radiation>0
sim$ObsDiffuseFraction[ix] = sim$diffuseRadiation[ix]/sim$radiation[ix]


grid.arrange(
  plot_all(sim)
  ,
  ggplot(subset(sim,ObsDiffuseFraction>0), aes(x=ObsDiffuseFraction, y=fraction)) +
    geom_point(aes(colour=factor(eq), size=radiation, alpha=0.2)) +
    geom_smooth(colour="grey")
  ,
  ncol=1
)
