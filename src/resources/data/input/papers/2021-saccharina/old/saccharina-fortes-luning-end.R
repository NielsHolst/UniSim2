source(paste0(box_script_folder, "/end-functions.R"))
graphics.off()

save(sim, file=paste0(box_script_folder,"/sim-10-fortes-luning.RData"))

P = ggplot(sim, aes(rLength)) +
  geom_histogram(fill=pink) +
  geom_vline(xintercept=c(0.012,0.020), size=1, colour=brown)
print(P)

I = 10
M1 = subset(sim, inside(rLength, 0.012, 0.020))
plot_combined(sim, M1, "Restrict rLength", brown)

# I = 30
# M1 = subset(sim, inside(rLength, 0.028, 0.038))
# plot_combined(sim, M1, "Restrict rLength", brown)

# I = 110
# M1 = subset(sim, inside(rLength, 0.14, 0.18))
# plot_combined(sim, M1, "Restrict rLength", brown)

# M2 = subset(M1, C>0.6)
# plot_combined(M1, M2, "Restrict C", blue)

# M3 = subset(M2, C>0.75)
# plot_combined(M2, M3, "Restrict C", green)

plot_corr(M1)

# # save(M3, file=paste0(box_script_folder,"/M3-fortes-luning.RData"))

# # Model Pmax depending on g
# M4 = M3
# load(paste0(box_script_folder,"/M3-bolton-luning.RData"))
# co = c(-0.0012, 0.143)
# P = ggplot(rbind(M3,M4), aes(g, Pmax)) +
  # geom_point(colour=red) +
  # geom_abline(intercept=co[1], slope=co[2], colour=blue, size=1) +
  # xlim(NA,0.020) +
  # ylim(NA, 0.0018) +
  # annotate("label", x=0.008, y=0.0015, label="Big\nplants") +
  # annotate("label", x=0.015, y=0.0015, label="Small\nplants") +
  # theme_bw()
# windows()
# print(P)

