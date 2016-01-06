TEMPLATE = subdirs

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes

SUBDIRS += savanna
savanna.subdir = src/plugins/savanna
savanna.depends = boxes

SUBDIRS += test
test.subdir = src/apps/test
test.depends = boxes
test.depends = savanna

