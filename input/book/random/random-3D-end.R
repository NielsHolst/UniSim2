sim = read_output(output_file_name)


plots = list(
  ggplot(sim, aes(x=uniform1.value, y=uniform2.value)) +
    coord_fixed() + 
    geom_point(color="#1b9e77", size=2)
  ,
  ggplot(sim, aes(x=uniform1.value, y=uniform3.value)) +
    coord_fixed() + 
    geom_point(color="#1b9e77", size=2)
  ,
  ggplot(sim, aes(x=uniform2.value, y=uniform3.value)) +
    coord_fixed() + 
    geom_point(color="#1b9e77", size=2)
)

windows(6,2)
print( grid.arrange(grobs=plots, nrow=1, ncol=3) )
