sim = read_output(output_file_name)
colnames(sim) = unique_names(colnames(sim))  
sim = rename(sim, c("carbon.proportion"="C", "nitrogen.proportion"="N"))

setwd(box_script_folder)
obs = read.table("obs/alge-obs-2019-AVG-vandrette-liner-100m-CN.txt", TRUE, "\t")
miss = obs<0
obs[miss] = NA
obs = rename(obs, c("date"="dateTime"))
obs$dateTime = dmy_hms(paste0(obs$dateTime, " 12:00:00"))

colour = blue

param = c("dateTime","C","N","area","dryWeight", "Wstruct")
Msim = melt(sim[,c("iteration",param)], id.vars=c("iteration","dateTime"))
Mobs = melt(obs[,param], id.vars="dateTime")
P = ggplot(Msim, aes(dateTime, value)) +
  geom_line(aes(group=iteration), alpha=0.3, colour=colour) +
  geom_point(colour=red, size=2, data=Mobs) +
  facet_wrap(~variable, scales="free") +
  theme_bw()

param = c("dateTime","propAbsorbed","sdRatio", "P", "maturity", "maxGrowthRate", "growthRatePct", "fNitrogen", "fCurrent", "U",
          "demandCarbonRespiration", "demandCarbonExudation", "supplyCarbonStructure")
Msim = melt(sim[,c("iteration",param)], id.vars=c("iteration","dateTime"))
Q = ggplot(Msim, aes(dateTime, value)) +
  geom_line(aes(group=iteration), alpha=0.3, colour=colour) +
  facet_wrap(~variable, scales="free") +
  theme_bw()

# N = subset(sim, iteration==1)
# a = 0.116
# b = 0.114
# N$Expected = with(N, dryWeight/(a*dryWeight + b))
# ggplot(N, aes(dryWeight, area)) +
  # geom_point(colour=green, alpha=0.3) +
  # geom_line(aes(y=Expected), colour=blue) +
  # theme_bw()
  

  
windows()
print(Q )
windows()
print(P)

go = graphics.off
