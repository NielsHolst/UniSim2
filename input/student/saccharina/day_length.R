sim = read_output(output_file_name)
sim$Change = c(diff(sim$dayLength),0)
M = ddply(sim, .(latitude), summarise, MaxChange=max(abs(Change)))
M

ggplot(subset(M, latitude>49 & latitude<65), aes(latitude, log(MaxChange))) +
  geom_point() +
  stat_smooth(method='lm', formula = y~poly(x,2))
  
