TEMPLATE = subdirs

SUBDIRS += base
base.subdir = src/plugins/base

#SUBDIRS += apis
#apis.subdir = src/plugins/apis
#apis.depends = base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base

SUBDIRS += command
command.subdir = src/plugins/command
command.depends = base

#SUBDIRS += conductance
#conductance.subdir = src/plugins/conductance
#conductance.depends = base

SUBDIRS += eq
eq.subdir = src/plugins/eq
eq.depends = base

SUBDIRS += student
student.subdir = src/plugins/student
student.depends = base

#SUBDIRS += savanna
#savanna.subdir = src/plugins/savanna
#savanna.depends = base

SUBDIRS += vg
vg.subdir = src/plugins/vg
vg.depends = base

#SUBDIRS += test_boxes
#test_boxes.subdir = src/plugins/test_boxes
#test_boxes.depends = base

#SUBDIRS += test
#test.subdir = src/apps/test
#test.depends = base
#test.depends = boxes
#test.depends = command
#test.depends = conductance
#test.depends = eq
#test.depends = savanna
##test.depends = vg
#test.depends = test_boxes

SUBDIRS += unisim
unisim.subdir = src/apps/unisim
#unisim.depends = apis
unisim.depends = base
unisim.depends = boxes
unisim.depends = command
#unisim.depends = conductance
unisim.depends = eq
unisim.depends = student
#unisim.depends = savanna
unisim.depends = vg
