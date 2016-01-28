TEMPLATE = subdirs

SUBDIRS += base
base.subdir = src/plugins/base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base

SUBDIRS += command
command.subdir = src/plugins/command
command.depends = base

SUBDIRS += savanna
savanna.subdir = src/plugins/savanna
savanna.depends = base

SUBDIRS += test
test.subdir = src/apps/test
test.depends = boxes
test.depends = command
test.depends = savanna

SUBDIRS += unisim
unisim.subdir = src/apps/unisim
unisim.depends = boxes
unisim.depends = command
unisim.depends = savanna
