sim = read_output(output_file_name)
sim = subset(sim, step==1)
# library(GGally)
# ggpairs(sim, columns=3:12)

# ggplot(subset(sim, step==2)) +
  # geom_density(aes(x=value))

print( mean(sim$value) )
print( sd(sim$value)  )

 