TEMPLATE = subdirs

SUBDIRS += base
base.subdir = src/plugins/base

#SUBDIRS += apis
#apis.subdir = src/plugins/apis
#apis.depends = base

#SUBDIRS += awe
#awe.subdir = src/plugins/awe
#awe.depends = base

SUBDIRS += aphid
aphid.subdir = src/plugins/aphid
aphid.depends = base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base

SUBDIRS += command
command.subdir = src/plugins/command
command.depends = base

SUBDIRS += demo
demo.subdir = src/plugins/demo
demo.depends = base

SUBDIRS += distribution
distribution.subdir = src/plugins/distribution
distribution.depends = base

#SUBDIRS += conductance
#conductance.subdir = src/plugins/conductance
#conductance.depends = base

SUBDIRS += eq
eq.subdir = src/plugins/eq
eq.depends = base

#SUBDIRS += PestTox
#PestTox.subdir = src/plugins/PestTox
#PestTox.depends = base

SUBDIRS += pollnet
pollnet.subdir = src/plugins/pollnet
pollnet.depends = base

SUBDIRS += student
student.subdir = src/plugins/student
student.depends = base

#SUBDIRS += tuta_absoluta
#tuta_absoluta.subdir = src/plugins/tuta_absoluta
#tuta_absoluta.depends = base

SUBDIRS += vg
vg.subdir = src/plugins/vg
vg.depends = base

SUBDIRS += unisim
unisim.subdir = src/apps/unisim
#unisim.depends = apis
#unisim.depends = awe
unisim.depends = aphid
unisim.depends = base
unisim.depends = boxes
unisim.depends = command
unisim.depends = demo
unisim.depends = distribution
#unisim.depends = conductance
unisim.depends = eq
unisim.depends = pollnet
#unisim.depends = PestTox
unisim.depends = student
#unisim.depends = tuta_absoluta
unisim.depends = vg
