files = data.frame(
  FileNumber=1:40, 
  FileName=c( 
    "Aarnes_2004.txt",
    "Aarnes_2005.txt",
    "Aarnes_2006.txt",
    "Aarnes_2012.txt",
    "Aarnes_2013.txt",
    "Aarnes_2014.txt",
    "Aarnes_2015.txt",
    "Aarnes_2016.txt",
    "Aarnes_2017.txt",
    "Aarnes_2018.txt",
    "Ilseng_2004.txt",
    "Ilseng_2005.txt",
    "Ilseng_2006.txt",
    "Ilseng_2012.txt",
    "Ilseng_2013.txt",
    "Ilseng_2014.txt",
    "Ilseng_2015.txt",
    "Ilseng_2016.txt",
    "Ilseng_2017.txt",
    "Ilseng_2018.txt",
    "Ramnes_2004.txt",
    "Ramnes_2005.txt",
    "Ramnes_2006.txt",
    "Ramnes_2012.txt",
    "Ramnes_2013.txt",
    "Ramnes_2014.txt",
    "Ramnes_2015.txt",
    "Ramnes_2016.txt",
    "Ramnes_2017.txt",
    "Ramnes_2018.txt",
    "Rygge_2004.txt",
    "Rygge_2005.txt",
    "Rygge_2006.txt",
    "Rygge_2012.txt",
    "Rygge_2013.txt",
    "Rygge_2014.txt",
    "Rygge_2015.txt",
    "Rygge_2016.txt",
    "Rygge_2017.txt",
    "Rygge_2018.txt"
  ),
  stringsAsFactors = FALSE
)

split_at = str_locate(files$FileName,"_")
files$Location = factor(substr(files$FileName, 1, split_at-1))
files$Year = factor(substr(files$FileName, split_at+1, split_at+4))
summary(files)

sim = read_output(output_file_name)
colnames(sim)[2] = "FileNumber"
sim = join(sim, files)

setwd(box_script_folder)
pdf("heatmaps.pdf", 8,2.5)

M = ddply(sim, .(Location, Year), summarize, AphidDaysImprovement=median(aphidDaysImprovement.end))
ggplot(M, aes(y=Location, x=Year, fill=AphidDaysImprovement)) +
  scale_fill_continuous() +
  geom_tile()

M = ddply(sim, .(Location, Year), summarize, AphidDaysUncontrolled=median(aphidDaysUncontrolled.end))
ggplot(M, aes(y=Location, x=Year, fill=AphidDaysUncontrolled)) +
  scale_fill_continuous() +
  geom_tile()

dev.off()

  



