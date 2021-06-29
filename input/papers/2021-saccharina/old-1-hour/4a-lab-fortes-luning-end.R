source(paste0(box_script_folder, "/0-lab-end-functions.R"))
graphics.off()

M1 = subset(sim, inside(r,0.014,0.020))

plot_combined(sim, M1, "", brown)
plot_corr(M1)

save(M1, file=paste0(box_script_folder, "/4a-lab-fortes-luning-M1.Rdata"))

