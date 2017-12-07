sim = read_output(output_file_name)

Data1 <- "E:/Ph. D period/Sabbatical/ScientificWork/Data.csv"
MyData <- read.table (file = Data1, header = TRUE, sep = ",")


A1 = MyData$A1
A2 = MyData$A2
A3 = MyData$A3
A4 = MyData$A4
A = rowMeans(MyData[,1:4])

dataSimTuta = data.frame (date = as.Date(sim$date),
                          sim = sim$Tabsoluta.adult.content)
                          
dataObsAMean = data.frame (week = 1:25,
                           date = as.Date(dmy("30/6/2016") + 7*(0:24)),
                           obsA = A)
                           
dataObsA = data.frame (week = 1:25,
                       date = as.Date(dmy("30/6/2016") + 7*(0:24)),
                       obs1 = A1,
                       obs2 = A2,
                       obs3 = A3,
                       obs4 = A4)


combined = join(dataObsA, dataSimTuta, by = "date");
combined$Mean = rowMeans(combined[,3:6]);

      
plot1 = ggplot() +
        geom_line (aes (x = date, y = A1), data = combined, size = 0.7) +
        geom_line (aes (x = date, y = A2), data = combined, color="darkgrey", size = 0.7) +
        geom_line (aes (x = date, y = A3), data = combined, color="red", size = 0.7) +
        geom_line (aes (x = date, y = A4), data = combined, color="green", size = 0.7) +
        geom_line (aes (x = date, y = sim), data = combined, color = "blue", size =1.2) +
        theme (panel.background=element_rect(fill="white"), panel.border = element_rect(linetype = "solid", fill=NA)) +
        ggtitle ("Treatment A") + labs(y = "Population", x = "Date")


plot2 = ggplot() +
        geom_line (aes (x = date, y = Mean), data = combined, color = "red") +
        geom_line (aes (x = date, y = sim), data = combined, color = "blue") +
        theme (panel.background=element_rect(fill="white"), panel.border = element_rect(linetype = "solid", fill=NA)) +
        labs(y = "Population", x = "Date") 

        
plot3 = ggplot() +
        geom_point(data = combined, aes (x= sim, y= obs1)) +
        geom_point(data = combined, aes (x= sim, y= obs2), color = "darkgrey") +
        geom_point(data = combined, aes (x= sim, y= obs3), color = "red") +
        geom_point(data = combined, aes (x= sim, y= obs4), color = "green") +
        theme(panel.background=element_rect(fill="white"), panel.border = element_rect(linetype = "solid", fill=NA)) +
        geom_abline(size = 1.2) + labs(y = "Observed population size", x = "Simulated population size")
        
        
        # ggplot() +
        # geom_point(data = combined, aes (x= sim, y= Mean)) +
        # geom_abline(size = 1.2) + labs(y = "Observed population size", x = "Simulated population size")
        
AllPlot = grid.arrange(plot1, plot2, plot3, ncol=1)

print(AllPlot)
