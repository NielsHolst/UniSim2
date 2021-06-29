source(paste0(box_script_folder, "/end-functions.R"))
graphics.off()

# M1 = subset(sim, inside(rLength, 0.17,0.22))
# plot_combined(sim, M1, "Restrict rLength", brown)

# .nitrogen = 0.003
M1 = subset(sim, inside(rLength, 0.08,0.11))
plot_combined(sim, M1, "Restrict rLength", brown)

M2 = subset(M1, inside(carbonPct, 25,30))
plot_combined(M1, M2, "Restrict carbonPct", blue)

M3 = subset(M2, inside(nitrogenPct, 1,1.5))
plot_combined(M2, M3, "Restrict nitrogenPct", green)

print(summary(M3$beta))

plot_corr(M3)

P = ggplot(M3, aes(alpha, Pmax)) +
  geom_point(colour=red) +
  geom_vline(xintercept=0.018, colour=blue, size=1) +
  theme_bw()
# windows()
print(P)

M4 = subset(M3, alpha<0.018)
plot_combined(M3, M4, "Restrict alpha", violet)

plot_corr(M4)

# model = rq(fNitrogenMin~alpha, M4, tau=0.90)
# co = (coef(model))

P = ggplot(M4, aes(alpha, fNitrogenMin)) +
  geom_point(colour=red) +
  geom_abline(intercept=c(0.68,0.86), slope=-30, colour=blue, size=1) +
  theme_bw()
windows()
print(P)
