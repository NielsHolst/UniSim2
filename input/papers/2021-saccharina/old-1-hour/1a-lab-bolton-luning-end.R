# sim = read_output(output_file_name)
# sim = simplify_col_names(sim)
# print(sim)

source(paste0(box_script_folder, "/0-lab-end-functions.R"))
graphics.off()

M1 = subset(sim, inside(r, 0.14,0.18))
plot_combined(sim, M1, "", brown)

M2 = subset(M1, C>0.20)
plot_combined(M1, M2, "", blue)

plot_corr(M2)

P = ggplot(M2, aes(g, Pmax)) +
  geom_point(colour=red) +
  geom_abline(intercept=0.00016, slope=0.0533, colour=blue, size=1) +
  theme_bw()
windows()
print(P)

save(M2 , file=paste0(box_script_folder,"/1a-lab-bolton-luning-M2.Rdata"))
