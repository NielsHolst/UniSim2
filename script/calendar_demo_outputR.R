solar = function(df) {
  windows(14, 10)
  grid.arrange(
    unisim_plot(df, c("dateTime", "dayLength")),
    unisim_plot(df, c("dateTime", "azimuth")),
    nrow = 1
  )
}
time = function(df) {
  windows(14, 10)
  grid.arrange(
    unisim_plot(df, c("dateTime", "trueSolarTime")),
    unisim_plot(df, c("dateTime", "sunrise", "sunset")),
    nrow = 2
  )
}
