sim = read_output(output_file_name)
sim = simplify_col_names(sim)

M = melt(sim, id.vars=c("iteration","step"))
P = ggplot(M, aes(step, value, colour=variable, group=iteration)) +
  geom_line(alpha=0.3) +
  labs(y="") +
  facet_wrap(~variable, ncol=1, scales="free") +
  theme_bw() +
  theme(legend.position="none")
  
print(P)
  