Cli_HeatingTemp = 20
Cli_VentTemp = 5
Cli_MaxHeatAddHighRH = 2
Cli_MaxVentDecHighRH = 2:3

print("\nLow RH")
paste("demandHeating    ", Cli_HeatingTemp)
paste("demandVentilation", Cli_HeatingTemp + Cli_VentTemp)

print("\nHigh RH")
paste("demandHeating    ", Cli_HeatingTemp + Cli_MaxHeatAddHighRH)
paste("demandVentilation", Cli_HeatingTemp + Cli_VentTemp + Cli_MaxHeatAddHighRH  - Cli_MaxVentDecHighRH)
