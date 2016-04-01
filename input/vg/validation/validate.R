# Initialize
library(ggplot2)
library(gridExtra)
graphics.off()
# rm(list=ls(all=TRUE))
source("c:/data/qdev/unisim2/script/common.R")
setwd("C:/data/QDev")

compare = function(colname1, colname2) {
	print(paste(colname1, nrow(old), colname2, nrow(new)))
	n = min(nrow(old), nrow(new))
	col1 = old[1:n, colname1]
	col2 = new[1:n, colname2]
	diff = col2-col1
	sum = cumsum(diff)
	data.frame(Variable=colname2, Row=1:n, Diff=col2-col1)
}

# old = read.table(file="UniSim2/input/vg/validation/dvv_unisim_0001_year.txt", header=TRUE, sep="\t")
new = read_unisim_output("UniSim2/tmp/output/TestOKO1_tr_0025.txt")
colnames(old)
colnames(new)
nrow(old)
nrow(new)

M = rbind(
	 compare("outdoorsWindspeed", "windSpeed")
	,compare("outdoorsRh", "rh")
	,compare("indoorsWindspeed", "windSpeed.value")
	,compare("indoorsRh", "indoorsRh")
	,compare("indoorsTemperature", "indoorsTemperature")
	,compare("coverTemperature", "coverTemperature")
	,compare("screenTemperature", "screensTemperature")
	,compare("floorTemperature", "floorTemperature")
)
M$Variable = factor(M$Variable)

ggplot(M) +
	geom_linerange(aes(x=Row, ymin=0,ymax=Diff), colour="steelblue") +
	facet_wrap(~Variable, ncol=4)


