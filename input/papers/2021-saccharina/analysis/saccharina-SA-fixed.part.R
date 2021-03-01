page_1 <- function(df, ...) {
  open_plot_window(10, 4)
  grid.arrange(
    plot_facetted(df[1:500,], c("alpha.value.end", "beta.value.end", "gamma.value.end", "g.value.end", "eMax.value.end", "e50.value.end", "fNitrogenMin.value.end", "aMax.value.end", "a50.value.end", "Winit.value.end", "resMaxRelDemand.value.end"), "iteration.end", c("areaSumSq.end", "yieldSumSq.end", "pctNSumSq.end", "pctCSumSq.end"), ytrans="", ncol=1, nrow=NULL)+geom_smooth(colour='yellow'),
    ...,
    nrow = NULL,
    ncol = NULL
  )
}
page_2 <- function(df, ...) {
  open_plot_window(10, 4)
  grid.arrange(
    plot_sobol_convergence()+ggplot_theme(11),
    ...,
    nrow = NULL,
    ncol = NULL
  )
}
page_3 <- function(df, ...) {
  open_plot_window(5, 7)
  grid.arrange(
    grobs= plot_sobol_indices(in_file="papers/2021-saccharina/analysis/saccharina-SA-fixed-S.Rdata"),
    ...,
    nrow = NULL,
    ncol = NULL
  )
}
plot_all <- function(df) {
  page_1(df)
  page_2(df)
  page_3(df)
}

figures <- function(df) {
  Pages = list(Page = list(Grob=page_1(df), Width=10, Height=4), Page = list(Grob=page_2(df), Width=10, Height=4), Page = list(Grob=page_3(df), Width=5, Height=7))
}

sobol_k = 11; sobol_N = 512; sobol_B = 10000
source("C:/Users/au152367/Documents/QDev/UniSim2/input/scripts/begin-sobol.R")

