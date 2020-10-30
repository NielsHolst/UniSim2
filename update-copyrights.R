rm(list=ls(all=TRUE))
library(plyr)
library(readtext)
library(stringr)

read_source_file = function(file_name) {
  readtext(file_name, verbosity=0)$text
}

has_initial_comment = function(s) {
  position = str_locate(s, "^/\\*")
  !is.na(position[1,1])
}
has_initial_comment("/* abc */\nxyz")

initial_comment_end = function(s) {
  str_locate(s, "(\\*/\\n)|(\\*/)")
}
initial_comment_end("/* abc */xyz")
initial_comment_end("/* abc */\nxyz")

extract_body_text = function(s) {
  ok = has_initial_comment(s)
  from = initial_comment_end(s)[1,2]+1
  if (ok) substring(s,from) else NA
}

replace_comment = function(s) {
  body = extract_body_text(s)
  if (is.na(body)) {
    body = s
    print("Original source had no initial comment")
  }
  paste(copyright, body, sep="\n") 
}

write_source_file = function(file_name, sub_dir, s) {
  if (!file.exists(sub_dir)) {
    dir.create(file.path(sub_dir))
  }
  f = file(paste(sub_dir, file_name, sep="/"), "wt", encoding="UTF-8")
  writeLines(s, f)
  close(f)
}

convert_source_file = function(file_name) {
  print(paste("Converting", file_name, "..."))
  s = read_source_file(file_name)
  # write_source_file(file_name, "commented", replace_comment(s)) # write to sub-dir to test output
  write_source_file(file_name, ".", replace_comment(s)) 
}

# s = read_source_file("accumulator.cpp")
# has_initial_comment(s)
# initial_comment_end(s)
# extract_body_text(s)
# replace_comment(s)
# convert_source_file("accumulator.cpp")

convert_all = function(folder) {
  setwd(folder)
  file_names = list.files(pattern="(.*\\.h)|(.*\\.cpp)")
  file_names = as.list(file_names)
  l_ply(file_names, convert_source_file)
}

copyright = 
"/* Copyright 2005-2020 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/awe")
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/base")
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/boxes")
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/bph")
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/command")
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/conductance")
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/io")

copyright = 
"/* Copyright 2018-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/aphid")

copyright = 
"/* Copyright 2018-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/apis")

copyright = 
"/* Copyright 2018-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/bmsb")

copyright = 
"/* Copyright 2005-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/cmbp")

copyright = 
"/* Copyright 2016-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/MusselBed")

copyright = 
"/* Copyright 2016-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/PestTox")

copyright = 
"/* Copyright 2017-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Andrew Antaya, University of Arizona [aantaya at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/pinacate")

copyright = 
"/* Copyright 2017-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Kayla Sale, University of Arizona [kayla.sale at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/pollnet")

copyright = 
"/* Copyright 2017-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/potato")

copyright = 
"/* Copyright 2017-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/resist")

copyright = 
"/* Copyright 2012-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/rvf")

copyright = 
"/* Copyright 2019-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Teis Boderskov, Aarhus University [tebo atbios.au.dk]
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/saccharina")

copyright = 
"/* Copyright 2017-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/tuta")

copyright = 
"/* Copyright 2017-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Emily Bick, University of California [enbick at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/vacuum")

copyright = 
"/* Copyright 2005-2020 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/"
convert_all("/Users/au152367/Documents/QDev/UniSim2/src/plugins/vg")



