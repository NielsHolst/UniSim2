# Replace iteration numbers with location names
sim$iteration = factor(sim$iteration)
levels(sim$iteration) = 
  c("Flakkebjerg", "Braunschweig", 
    "Geneva", "Ljubljana")
plot_all(sim)