unisim__page <- function(df) {
  grid.arrange(
    unisim_plot_facetted(df, "step", c("content"), ncol=1, nrow=NULL),
    nrow = NULL,
    ncol = NULL
  )
}
unisim_plot_all <- function(df) {
  unisim__page(df)
}
