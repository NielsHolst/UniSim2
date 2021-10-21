sim = read_output(output_file_name)
sim$lnDryWeight = log(sim$dryWeight)
sim$lnArea      = log(sim$area) 
sim$lnLength    = log(sim$length) 

M = melt(sim, id.vars=c("iteration","step"), 
         measure.vars=c("lnDryWeight","lnArea", "lnLength", "Iabsorbed", "fNitrogen", "sdRatio", "C", "N")
)

theme_set(theme_bw())

P = ggplot(subset(M, step>0), aes(step, value, colour=variable, group=iteration)) +
  geom_line(alpha=0.4) +
  labs(x="Hour") +
  scale_x_continuous(breaks = 24*(0:7)) +
  facet_wrap(~variable, scales="free") +
  theme(
    legend.position = 'none'
  )

G = ddply(subset(sim, step>0), .(iteration), summarise,
  alpha = unique(alpha),
  g = unique(g),
  Pmax = unique(Pmax),
  rLength = tail(rLength,1),
  Winit = head(dryWeight,1),
  Wend =  tail(dryWeight,1),
  Ainit = head(area,1),
  Aend =  tail(area,1),
  Linit = head(length,1),
  Lend =  tail(length,1),
  Cend =  tail(C,1),
  Nend =  tail(N,1)
)

G$rW = with(G, log(Wend/Winit)/7)
G$rA = with(G, log(Aend/Ainit)/7)
G$rL = with(G, log(Lend/Linit)/7)

responses = c("rW", "rA", "rL", "Cend", "Nend")

M = melt(G, id.vars=c("iteration", "g"), measure.vars=responses)
Q = ggplot(M, aes(g, value, colour=variable)) +
  geom_point() +
  labs(y="Relative growth rate (per day)") +
  geom_hline(colour=brown, yintercept = c(0.15,0.175)) +
  facet_wrap(~variable, scales="free", nrow=1) +
  theme(
    legend.position = 'none'
  )

M = melt(G, id.vars=c("iteration", "alpha"), measure.vars=responses)
R = ggplot(M, aes(alpha, value, colour=variable)) +
  geom_point() +
  labs(y="Relative growth rate (per day)") +
  geom_hline(colour=brown, yintercept = c(0.15,0.175)) +
  facet_wrap(~variable, scales="free", nrow=1) +
  theme(
    legend.position = 'none'
  )
  
M = melt(G, id.vars=c("iteration", "Pmax"), measure.vars=responses)
S = ggplot(M, aes(Pmax, value, colour=variable)) +
  geom_point() +
  labs(y="Relative growth rate (per day)") +
  geom_hline(colour=brown, yintercept = c(0.15,0.175)) +
  facet_wrap(~variable, scales="free", nrow=1) +
  theme(
    legend.position = 'none'
  )
  
print(ggarrange(P,Q,R,S,ncol=1))

