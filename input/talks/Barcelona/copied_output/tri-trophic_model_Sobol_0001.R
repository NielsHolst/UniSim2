page_1 <- function(df, ...) {
  open_plot_window(7, 7)
  grid.arrange(
    plot_facetted(df, c("Threshold_RH.end", "infected_immigrants.end", "virulence.end"), "iteration.end", c("density.sum", "d_cadaver.sum"), ytrans="", ncol=1, nrow=NULL)+geom_point(size=2, alpha=0.3) + geom_smooth(colour='yellow',size=1) + theme_classic(),
    top = "RH and inoculum",
    ...,
    nrow = NULL,
    ncol = NULL
  )
}
# X="Threshold_RH.end", "infected_immigrants.end", "virulence.end" Y="Threshold_RH.end", "infected_immigrants.end", "virulence.end"
# X="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end" Y="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end"
# X="Min.end", "ED50.end" Y="Min.end", "ED50.end"
# X="Threshold_RH.end", "infected_immigrants.end", "virulence.end" Y="Threshold_RH.end", "infected_immigrants.end", "virulence.end"
page_2 <- function(df, ...) {
  open_plot_window(7, 7)
  grid.arrange(
    plot_facetted(df, c("immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end"), "iteration.end", c("density.sum", "d_cadaver.sum"), ytrans="", ncol=1, nrow=NULL)+geom_point(size=2, alpha=0.3) + geom_smooth(colour='yellow',size=1) + theme_classic(),
    top = "infection parameters",
    ...,
    nrow = NULL,
    ncol = NULL
  )
}
# X="Threshold_RH.end", "infected_immigrants.end", "virulence.end" Y="Threshold_RH.end", "infected_immigrants.end", "virulence.end"
# X="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end" Y="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end"
# X="Min.end", "ED50.end" Y="Min.end", "ED50.end"
# X="Threshold_RH.end", "infected_immigrants.end", "virulence.end" Y="Threshold_RH.end", "infected_immigrants.end", "virulence.end"
# X="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end" Y="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end"
page_3 <- function(df, ...) {
  open_plot_window(7, 7)
  grid.arrange(
    plot_facetted(df, c("Min.end", "ED50.end"), "iteration.end", c("density.sum", "d_cadaver.sum"), ytrans="", ncol=1, nrow=NULL)+geom_point(size=2, alpha=0.3) + geom_smooth(colour='yellow',size=1) + theme_classic(),
    top = "wheat phenology uncertainty",
    ...,
    nrow = NULL,
    ncol = NULL
  )
}
# X="Threshold_RH.end", "infected_immigrants.end", "virulence.end" Y="Threshold_RH.end", "infected_immigrants.end", "virulence.end"
# X="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end" Y="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end"
# X="Min.end", "ED50.end" Y="Min.end", "ED50.end"
# X="Threshold_RH.end", "infected_immigrants.end", "virulence.end" Y="Threshold_RH.end", "infected_immigrants.end", "virulence.end"
# X="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end" Y="immunity_cost.end", "cadaver_longevity.end", "sporu_cada_longevity.end", "LTimmigrants.end", "LTAdult.end", "LTNymph.end"
# X="Min.end", "ED50.end" Y="Min.end", "ED50.end"
plot_all <- function(df) {
  page_1(df)
  page_2(df)
  page_3(df)
}
