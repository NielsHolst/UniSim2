# sim = read_output(output_file_name)
# sim = simplify_col_names(sim)
# print(sim)

source(paste0(box_script_folder, "/0-lab-end-functions.R"))
graphics.off()

M1 = subset(sim, inside(nitrogenPct,2,3))

plot_combined(sim, M1, "", brown)
plot_corr(M1)


plot_g_beta = function(slope1, intercept1, slope2, intercept2) {
  windows()
  ggplot(M1, aes(g, beta)) +
    geom_point(colour=red) +
    geom_abline(intercept=intercept1, slope=slope1, colour=blue, size=1) +
    geom_abline(intercept=intercept2, slope=slope2, colour=blue, size=1) +
    theme_bw()
}

print(
  plot_g_beta(
    0.35,  -0.0008, 
    0.25,  -0.0010 
  )
)

plot_Pmax_beta = function(slope, intercept) {
  # windows()
  ggplot(M1, aes(Pmax, beta)) +
    geom_point(colour=red) +
    geom_abline(intercept=intercept, slope=slope, colour=blue, size=1) +
    theme_bw()
}


print(plot_Pmax_beta(4.17, -0.0002))
