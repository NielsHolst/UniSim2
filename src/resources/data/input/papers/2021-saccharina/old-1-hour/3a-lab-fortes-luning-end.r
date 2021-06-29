# sim = read_output(output_file_name)
# sim = simplify_col_names(sim)
# print(sim)

reuse_sim = FALSE

if (reuse_sim) {
  box_script_folder = "C:/Users/au152367/Documents/QDev/UniSim2/input/papers/2021-saccharina"
  load(paste0(box_script_folder,"/3a-lab-fortes-luning-sim.Rdata"))
}
source(paste0(box_script_folder, "/0-lab-end-functions.R"))
if (!reuse_sim) {
   save(sim, file=paste0(box_script_folder,"/3a-lab-fortes-luning-sim.Rdata"))
}
graphics.off()

load(paste0(box_script_folder,"/1a-lab-bolton-luning-M2.Rdata"))
young = M2[, c("Pmax","g")]
young$Stage = "Young"

M1 = subset(sim, inside(r,0.05,0.06))
plot_combined(sim, M1, "", brown)

M2 = subset(M1, C>1.0)
plot_combined(M1, M2, "", blue)

plot_corr(M2)

P = ggplot(M2, aes(Pmax, uCinit)) +
  geom_point(colour=red) +
  geom_abline(slope=-1750, intercept=2.95, colour=blue, size=1) +
  theme_bw()
# windows()
print(P)

old = M2[, c("Pmax","g")]
old$Stage = "Mature"

M = rbind(young,old)
P = ggplot(M, aes(g, Pmax)) +
  geom_point(aes(colour=Stage)) +
  scale_colour_manual(values=c(red, grey)) +
  theme_bw()
windows()
print(P)

g_young = median(young$g)
g_old   = median(old$g)
c_age   = g_old/g_young

print(c(
  g_young = g_young,
  g_old   = g_old,  
  c_age   = c_age,
  c_age2  = 1/c_age
))

young$g = young$g*c_age
M = rbind(young,old)

P = ggplot(M, aes(g, Pmax)) +
  geom_point(aes(colour=Stage)) +
  geom_abline(slope=0.28, intercept=-0.00027, colour=blue, size=1) +
  scale_colour_manual(values=c(red, grey)) +
  scale_x_continuous(breaks=0.004+0.0002*(1:20)) +
  labs(x="g_mature") +
  theme_bw()
#windows()
print(P)

