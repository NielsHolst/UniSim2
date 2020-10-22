sim = read_output(output_file_name)
M = sim[c("dateTime","spHeatingTemperature","indoorsT","spVentilationTemperature")]
M = melt(M, id.vars="dateTime")

ggplot(M, aes(dateTime, value, colour=variable)) +
  geom_line() +
  theme_minimal()
  
