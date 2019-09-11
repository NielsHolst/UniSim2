suffix = ".png"

output_path = function() {
  slashes = str_locate_all(output_file_name, "/")[[1]]
  last_slash = slashes[nrow(slashes)]
  substr(output_file_name, 1, last_slash-1)
}

insert_file_number = function(filePath, number) {
  paste0(
    # substr(filePath, 1, nchar(filePath)-4),
    filePath,
    "-", number, suffix)
}
 
save_figure_page = function(page, filePath) {
  print(paste("Saving figure:", filePath, "..."))
  ggsave(filePath, page$Grob, width=page$Width, height=page$Height, units="in") 
}

save_figure = function(figure) {
  filePath = figure$FilePath
  pages = figure$Pages[[1]]
  numPages = length(pages)
  if (numPages == 1) {
    save_figure_page(pages[[1]], paste0(filePath, suffix))
  } else {
    for (i in 1:numPages) {
      save_figure_page(pages[[i]], insert_file_number(filePath, i))
    }
  } 
}

work_dir = paste0(output_path(), "/batch_output")
if (!dir.exists(work_dir)) dir.create(work_dir)

setwd(work_dir)
l_ply(all_figures, save_figure)

print(paste("Figures written as PNG files to", work_dir))
