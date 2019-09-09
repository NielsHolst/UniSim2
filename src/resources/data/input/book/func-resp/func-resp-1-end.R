# Replace iteration numbers with labels
sim = read_output(output_file_name)
attacker = unique(sim$attacker)
searchEfficacy = round(unique(sim$searchEfficacy), 2)
sim$iteration = factor(sim$iteration)
levels(sim$iteration) = paste("Attacker density =", paste0(attacker,";"), " Search efficacy =", searchEfficacy)

sim$s = NA
ix = (sim$prey<999)
sim$s[ix] = sim$searchEfficacy[ix]*sim$prey[ix]

M = melt(sim, id.vars=c("iteration", "prey"), measure.vars=c("preyAttacked", "demand", "s"))
levels(M$variable)[1] = "G-B func. resp."
levels(M$variable)[3] = "L-V func. resp."

print(
  ggplot(M) +
    geom_line(aes(x=prey, y=value, colour=variable), size=1.1) +
    guides(colour = guide_legend(reverse=TRUE)) + 
    scale_y_continuous(breaks = 10*(0:10), limits=c(0, 1.1*max(sim$demand))) +
    labs(x="No. of prey", y="No. of prey killed") + 
    facet_wrap(~iteration, ncol=1)
)
