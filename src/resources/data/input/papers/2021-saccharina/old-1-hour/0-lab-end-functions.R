library(psych)
library(quantreg)

if (!exists("reuse_sim") || !reuse_sim) {
  sim = read_output(output_file_name)
  sim = simplify_col_names(sim)
}

has_pct = "carbonPct" %in% colnames(sim)
env = if ("irradiation" %in% colnames(sim)) "irradiation" else NULL

parameters = c(
  "alpha"       ,
  "beta"        ,
  "gamma"       ,
  "g"           ,
  "fNitrogenMin",
  "Pmax"        ,
  "uCinit", 
  "uNinit"
)
parameters = intersect(parameters, colnames(sim))

responses = c(
  "r",
  "C",
  "N"
)


if (has_pct) {
  responses = c(responses, "carbonPct", "nitrogenPct")
} 

sim = sim[,c(env, parameters, responses)]

inside = function(x, xmin, xmax) {
  x>xmin & x<xmax
}

reorder_levels = function(the_factor, new_order) {
  factor(the_factor,levels(the_factor)[new_order])
}

plot_limits = function(M1, M2, colour) {
  X = ddply(melt(M1), .(variable), summarize, 
    xmin=min(value), xmax=max(value))
  Y = ddply(melt(M2),   .(variable), summarize,
    ymin=min(value), ymax=max(value))

  Z = join(X,Y)

  Z$x = (Z$xmax+Z$xmin)/2
  Z$y = (Z$ymax+Z$ymin)/2


  f = function(z) {
    ggplot(z, aes(x,y)) +
      geom_errorbarh(aes(xmin=xmin, xmax=xmax), height=0, size=2, colour=colour) +
      geom_errorbar (aes(ymin=ymin, ymax=ymax), width=0, size=2, colour=colour) +
      coord_fixed() +
      labs(x=z$variable, y="") +
      theme_bw()
  }
  plots = dlply(Z, .(variable), f)
  ggarrange(plotlist=plots)
}

plot_histogram = function(M, colour) {
  ggplot(melt(M), aes(value)) +
    geom_histogram(fill=colour, alpha=0.6) +
    coord_flip() +
    facet_wrap(~variable, scales="free") +
    labs(x="", y="") +
    theme_bw() +
    theme(
      axis.text.x  = element_blank(),
      axis.ticks.x = element_blank()
    )
}

plot_combined = function(M1, M2, text, colour) {
  A = annotate_figure(
    ggarrange(
      plot_limits(M1, M2, colour),
      plot_histogram(M2, colour),
      ncol=1
    ),
    top = text_grob(text, size=14)
  )
  windows()
  print(A)
}

plot_corr = function(M) {
  windows()
  pairs.panels(
    M[, parameters], 
    method = "pearson", 
    hist.col = pink,
    smooth = FALSE,
    density = FALSE,
    ellipses = FALSE,
    cor = FALSE
  )
}

