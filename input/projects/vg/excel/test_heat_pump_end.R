sim = read_output(output_file_name)

M = melt(sim[,-1],id.vars=1:2)

ggplot(M, aes(indoorsRh, value, colour=factor(indoorsTemperature))) +
  geom_line() +
  facet_wrap(~variable, scales="free")
  