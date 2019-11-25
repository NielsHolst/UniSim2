sim = read_output(output_file_name)

sim$vapourFluxSum = rowSums(sim[
                        c("vapourFluxTranspiration", 
                          "vapourFluxCondensationCover", 
                          "vapourFluxCondensationsScreens",
                          "vapourFluxLeakage", 
                          "vapourFluxVents")])
summary(sim)

grid.arrange(
  ggplot(sim, aes(x=vapourFluxSum, y=vapourFluxNet)) +
    geom_point() +
    geom_abline(slope=1, intercept=0, colour="red")
  ,
  ggplot(sim, aes(x=-vapourFluxVents, y=-vapourFluxLeakage)) +
    geom_point() +
    geom_abline(slope=1, intercept=0) +
    geom_abline(slope=1, intercept=0, colour="red")
  ,
  ncol=1
)

mean(sim$vapourFluxTranspiration)*3600*24*365
mean(sim$vapourFluxCondensationCover)*3600*24*365
mean(sim$vapourFluxCondensationsScreens)*3600*24*365
mean(sim$vapourFluxLeakage)*3600*24*365
mean(sim$vapourFluxVents)*3600*24*365


