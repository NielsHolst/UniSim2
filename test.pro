TEMPLATE = subdirs

SUBDIRS += base
base.subdir = src/plugins/base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base

SUBDIRS += command
command.subdir = src/plugins/command
command.depends = base

SUBDIRS += distribution
distribution.subdir = src/plugins/distribution
distribution.depends = base

SUBDIRS += demo
demo.subdir = src/plugins/demo
demo.depends = base

SUBDIRS += eq
eq.subdir = src/plugins/eq
eq.depends = base

SUBDIRS += savanna
savanna.subdir = src/plugins/savanna
savanna.depends = base

SUBDIRS += student
student.subdir = src/plugins/student
student.depends = base

#SUBDIRS += vg
#vg.subdir = src/plugins/vg
#vg.depends = base

SUBDIRS += test_boxes
test_boxes.subdir = src/plugins/test_boxes
test_boxes.depends = base

SUBDIRS += test
test.subdir = src/apps/test
test.depends = base
test.depends = boxes
test.depends = command
test.depends = distribution
test.depends = demo
test.depends = eq
test.depends = savanna
test.depends = student
#test.depends = vg
test.depends = test_boxes

