source(paste0(box_script_folder, "/0-lab-end-functions.R"))
graphics.off()

P = plot_histogram(sim, violet) 
print(P)

print( nrow(subset(sim,C>0.9))/nrow(sim) )

print( nrow(subset(sim,r<0.06))/nrow(sim) )
