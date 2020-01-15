sim = read_output(output_file_name)
sim = tail(sim[, -(1:2)],1)
rownames(sim) = {}
print(sim)
