sim = read_output(output_file_name)
colnames(sim)

sim$month = month(sim$dateTime)
sim$hour = hour(sim$dateTime)
  
ggplot(sim) +
  geom_point(aes(x=outdoorsLight, y=cropGrowthRate, colour=indoorsT), alpha=0.3)
  
M = subset(sim,cropGrowthRate>0)
# model = data.frame(spline(M$outdoorsLight, M$cropGrowthRate))

# windows()
# ggplot(model) +
  # geom_point(aes(x=x, y=y))

model = nls(cropGrowthRate~ymax*(1-exp(-slope*outdoorsLight)), M, list(ymax=2, slope=0.5/100))
summary(model

E = data.frame(outdoorsLight=seq(0,1000,length.out=30))
predict(model, E)

