sim = read_output(output_file_name)
setwd(box_script_folder)

obs_file_name = unique(sim$obsFile)
obs_trial = unique(sim$trial)

obs = read.table(obs_file_name, header=TRUE, sep="\t")
obs$dateTime = ymd_hms(paste(obs$Date, "12:0:0"))

p = ggplot(sim, aes(dateTime, area)) +
  geom_line(colour=unisim_colours[2]) +
  geom_point(colour=unisim_colours[1], size=3, data=obs) +
  labs(x="Months", y="Area (dm2)", title=obs_trial) +
  scale_x_datetime(breaks = date_breaks("months"),
                   labels = date_format("%m"))
print(p)

subfolder = "figures"
if (!file.exists(subfolder))
  dir.create(subfolder)

ggexport(p,
  filename = paste0(subfolder, "/", output_file_base_name(), ".png"), 
  width = 480,
  height = 320
)
