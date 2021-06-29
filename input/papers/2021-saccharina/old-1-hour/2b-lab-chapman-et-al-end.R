source(paste0(box_script_folder, "/0-lab-end-functions.R"))
graphics.off()

windows()
P = plot_histogram(sim, violet) 
print(P)

save(sim, file=paste0(box_script_folder,"/2b-lab-chapman-et-al-sim.Rdata"))
