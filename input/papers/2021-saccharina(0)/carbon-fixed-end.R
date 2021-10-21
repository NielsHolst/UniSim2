setwd("/Users/au152367/Documents/QDev/UniSim2/input")

sim = read_output(output_file_name)

obs = read.table("papers/2021-saccharina/obs/alge-obs-2019-AVG-vandrette-liner-100m-CN.txt", header=TRUE, sep="\t")[3:7,]
obs$date = as.POSIXct(dmy(obs$date))
Cstruct = 0.21
obs$Wcarbon = 400*with(obs, Wstruct*(Cstruct + C))

P = ggplot() +
  geom_line(aes(date, content), colour=blue, data=sim) +
  geom_point(aes(date, Wcarbon), colour=red, size=3, data=obs) +
  labs(x="", y="Wcarbon (g C per m line)") +
  theme_bw()
print(P)