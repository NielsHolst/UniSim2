TEMPLATE = subdirs

SUBDIRS += base
base.subdir = src/plugins/base
base.depends = boxes

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes

SUBDIRS += command
command.subdir = src/plugins/command
command.depends = boxes

SUBDIRS += console
console.subdir = src/plugins/console
console.depends = boxes

SUBDIRS += savanna
savanna.subdir = src/plugins/savanna
savanna.depends = boxes


SUBDIRS += test
test.subdir = src/apps/test
test.depends = base
test.depends = boxes
test.depends = console
test.depends = savanna

