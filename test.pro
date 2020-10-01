TEMPLATE = subdirs

SUBDIRS += test
test.subdir = src/apps/test

SUBDIRS += base
base.subdir = src/plugins/base
test.depends = base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base
test.depends = boxes

SUBDIRS += command
command.subdir = src/plugins/command
command.depends = base
test.depends = command

#SUBDIRS += distribution
#distribution.subdir = src/plugins/distribution
#distribution.depends = base

SUBDIRS += demo
demo.subdir = src/plugins/demo
demo.depends = base
test.depends = demo

#SUBDIRS += eq
#eq.subdir = src/plugins/eq
#eq.depends = base

#SUBDIRS += savanna
#savanna.subdir = src/plugins/savanna
#savanna.depends = base

#SUBDIRS += student
#student.subdir = src/plugins/student
#student.depends = base

#SUBDIRS += vg
#vg.subdir = src/plugins/vg
#vg.depends = base

#SUBDIRS += test_boxes
#test_boxes.subdir = src/plugins/test_boxes
#test_boxes.depends = base
