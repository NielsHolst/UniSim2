box_script_folder = "C:/Users/au152367/Documents/QDev/UniSim2/input/papers/2021-saccharina"
load(paste0(box_script_folder,"/M3-fortes-luning.RData"))
big = M3
load(paste0(box_script_folder,"/M3-bolton-luning.RData"))
small = M3

summary(big$g)
summary(small$g)

summary(small$g)/summary(big$g)

summary(big$Pmax)
