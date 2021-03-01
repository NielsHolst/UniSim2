sim = read_output(output_file_name)
colnames(sim) = unique_names(colnames(sim))
setwd(box_script_folder)
sim = rename(sim, c("carbon.proportion"="propC", "nitrogen.proportion"="propN"))

plot_calibration = function(M) {
  obs_file_name = as.character(unique(M$obsFile))
  obs_trial = as.character(unique(M$trial))
  print(paste("Reading obs from", obs_file_name,"in",getwd()))
  obs = read.table(obs_file_name, header=TRUE, sep="\t")
  obs$dateTime = dmy_hms(paste(obs$date, "12:0:0"))
  obs = rename(obs, c("C"="propC", "N"="propN"))

  vars = c("dateTime","area","propN","propC","yield")
  obs = melt(obs[vars], id.vars="dateTime", value.name="Value", variable.name="Variable")
  obs$Value[obs$Value < 0] = NA

  M = melt(M  [vars], id.vars="dateTime", value.name="Value", variable.name="Variable")
  M$Variable = factor(M$Variable)
  levels(M$Variable)
  
  ggplot(M, aes(dateTime, Value, colour=Variable)) +
    geom_line() +
    geom_point(size=3, data=obs, colour=unisim_colours[5]) +
    labs(x="Months", y="Value", title=obs_trial) +
    scale_x_datetime(breaks = date_breaks("months"),
                     labels = date_format("%m")) +
    facet_wrap(~Variable, ncol=1, scales="free_y") +
    theme(legend.position='none')
}

plot_f_scales = function(M) {
  obs_trial = unique(M$trial)
  M = melt(M[c("dateTime", "fArea", "J", "V", "B", "fNitrogen", "fTemp", "T", "fSalinity", "fCurrent", "sdRatio")], id.vars="dateTime", value.name="Factor", variable.name="Scaling")
  
  ggplot(M, aes(dateTime, Factor, colour=Scaling)) +
    geom_line() +
    labs(x="Months", y="Scaling factor", title=obs_trial) +
    scale_x_datetime(breaks = date_breaks("months"),
                     labels = date_format("%m")) +
    facet_wrap(~Scaling, ncol=1, scales="free_y") +
    theme(legend.position='none')
}

plot_allocation_totals = function(M) {
  obs_trial = unique(M$trial)
  M = melt(M[c("dateTime", "growth", "exudation", "erosion", "respiration", "structuralLoss")], id.vars="dateTime", value.name="Total", variable.name="Allocation")
  
  ggplot(M, aes(dateTime, Total, colour=Allocation)) +
    geom_line() +
    labs(x="Months", y="Allocation accumulated total", title=obs_trial) +
    scale_x_datetime(breaks = date_breaks("months"),
                     labels = date_format("%m")) +
    facet_wrap(~Allocation, ncol=1, scales="free_y") +
    theme(legend.position='none')
}

plot_allocation_rates = function(M) {
  obs_trial = unique(M$trial)
  daily = subset(M, hour(sim$dateTime)==0)
  daily = daily[c("dateTime","dryWeight","growth", "exudation", "erosion", "respiration", "structuralLoss")]

  f = function(colName) {
    M = data.frame(
      Rate = diff(unlist(daily[colName]))
    )
    colnames(M) = paste0(colName, "Rate")
    M
  }

  rows = 1:(nrow(daily)-1)
  M = cbind(
    dateTime  = daily$dateTime[rows],
    dryWeight = daily$dryWeight[rows],
    f("dryWeight"),
    f("growth"),
    f("exudation"),
    f("erosion"),
    f("respiration"),
    f("structuralLoss")
  )
  M$expGrowthRate = log(1 + M$dryWeightRate/M$dryWeight)
  M = melt(M, id.vars="dateTime", , value.name="DailyRate", variable.name="Allocation")

  ggplot(M, aes(dateTime, DailyRate, colour=Allocation)) +
    geom_hline(yintercept=0, colour="darkgrey") +
    geom_line() +
    labs(x="Months", y="Allocation rate per day", title=obs_trial) +
    scale_x_datetime(breaks = date_breaks("months"),
                     labels = date_format("%m")) +
    facet_wrap(~Allocation, ncol=1, scales="free_y") +
    theme(legend.position='none')
}

plot_state = function(M) {
  obs_file_name = unique(M$obsFile)
  obs_trial = unique(M$trial)
  M = melt(M[c("dateTime", "dryWeight", "carbonPct", "nitrogenPct", 
                "propC", "propN")], 
                id.vars="dateTime", value.name="Value", variable.name="Variable")
  
  ggplot(M, aes(dateTime, Value, colour=Variable)) +
    geom_line() +
    labs(x="Months", y="Value", title=obs_trial) +
    scale_x_datetime(breaks = date_breaks("months"),
                     labels = date_format("%m")) +
    facet_wrap(~Variable, ncol=1, scales="free_y") +
    theme(legend.position='none')
}

plot_env = function(M) {
  obs_trial = unique(M$trial)
  M = melt(M[c("dateTime", "T", "I", "N", "S", "U")], 
                id.vars="dateTime", value.name="Value", variable.name="Variable")
  
  ggplot(M, aes(dateTime, Value, colour=Variable)) +
    geom_line() +
    labs(x="Months", y="Value", title=obs_trial) +
    scale_x_datetime(breaks = date_breaks("months"),
                     labels = date_format("%m")) +
    facet_wrap(~Variable, ncol=1, scales="free_y") +
    theme(legend.position='none')
}

plot_allocation = function(M) {
  obs_trial = unique(M$trial)
  M = melt(M[c("dateTime",
                "supplyCarbonStructure",
                "supplyCarbonExudation",
                "supplyCarbonReserves"
              )
            ], 
            id.vars="dateTime", value.name="Value", variable.name="Allocation"
  )
  
  ggplot(M, aes(dateTime, Value, colour=Allocation)) +
    geom_line() +
    labs(x="Months", y="Value", title=obs_trial) +
    scale_x_datetime(breaks = date_breaks("months"),
                     labels = date_format("%m")) +
    facet_wrap(~Allocation, ncol=1, scales="free_y") +
    theme(legend.position='none')
}

plot_growth_rate = function(M) {
  obs_trial = unique(M$trial)
  ggplot(M, aes(dateTime, growthRatePct)) +
    geom_line(colour=green) +
    labs(x="Months", y="Growth rate of dry weight (%/day)\n", title=obs_trial) +
    scale_x_datetime(breaks = date_breaks("months"),
                     labels = date_format("%m")) +
    theme_minimal()
}  

print_plots = function(plots) {
  open_plot_window()
  print( ggarrange(plotlist=plots, align="hv") )
}

print_plots( dlply(sim, .(iteration), plot_allocation_totals) )
print_plots( dlply(sim, .(iteration), plot_state) )
print_plots( dlply(sim, .(iteration), plot_f_scales) )
print_plots( dlply(sim, .(iteration), plot_allocation_rates) )
print_plots( dlply(sim, .(iteration), plot_env) )
print_plots( dlply(sim, .(iteration), plot_allocation) )
print_plots( dlply(sim, .(iteration), plot_growth_rate) )
print_plots( dlply(sim, .(iteration), plot_calibration) )


