sim = read_output(output_file_name)
sim$Location = factor(sim$iteration)
levels(sim$Location) = c("Agadez","Bilma","Diffa","Goure",
                         "Maine Soroa","N_guigni","Nouadhibou",
                         "Nouakchott","Podor","Saint_louis","Tahoua","Tillabery")
MCJ400 = ddply(sim, ~Location, summarise, 
               sum.individuals = sum(individuals.content),
               sum.individualsTJ = sum(individualsTJ.content),
               sum.MV2loss = sum(marucaInstar2.loss),
               sum.MV3loss = sum(marucaInstar3.loss),
               sum.MV2 = sum(instar2.content),
               sum.MV3 = sum(instar3.content),
               pourcentage = (((sum.MV2loss + sum.MV3loss)/(sum.MV2 + sum.MV3))*100))
print(MCJ400) 
write.csv(MCJ400, "~/Aarhus/M2/Mémoire/Result/MCJ400.csv")