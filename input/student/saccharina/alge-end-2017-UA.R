sim = read_output(output_file_name)
head(sim)

setwd(box_script_folder)
obs = read.table("alge-obs-2017.txt", header=TRUE, sep="\t")
obs$dateTime = ymd_hms(paste(obs$Date, "12:0:0"))

p = ggplot(sim, aes(dateTime, log10(A))) +
  geom_line(aes(group=iteration), alpha=0.3, colour=unisim_colours[2]) +
  geom_point(colour=unisim_colours[1], size=3, data=obs)
print(p)
