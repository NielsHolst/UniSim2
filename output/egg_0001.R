page_1 <- function(df) {
  grid.arrange(
    unisim_plot_facetted(df, "step", c("content"), ncol=1, nrow=NULL),
    nrow = NULL,
    ncol = NULL
  )
}
plot_all <- function(df) {
  page_1(df)
}
