# Universal Simulator main project file
#

TEMPLATE = subdirs

#
# Required modules
#

SUBDIRS += unisim
unisim.subdir = src/apps/unisim

SUBDIRS += base
base.subdir = src/plugins/base
unisim.depends = base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base
unisim.depends = boxes

SUBDIRS += cmbp
cmbp.subdir = src/plugins/cmbp
cmbp.depends = base
unisim.depends = cmbp

SUBDIRS += command
command.subdir = src/plugins/command
command.depends = base
unisim.depends = command

SUBDIRS += demo
demo.subdir = src/plugins/demo
demo.depends = base
unisim.depends = demo

SUBDIRS += distribution
distribution.subdir = src/plugins/distribution
distribution.depends = base
unisim.depends = distribution

SUBDIRS += eq
eq.subdir = src/plugins/eq
eq.depends = base
unisim.depends = eq

SUBDIRS += student
student.subdir = src/plugins/student
student.depends = base
unisim.depends = student

SUBDIRS += vg
vg.subdir = src/plugins/vg
vg.depends = base
unisim.depends = vg

#
# Optional plug-ins
#

SUBDIRS += aphid
aphid.subdir = src/plugins/aphid
aphid.depends = base
unisim.depends = aphid

#SUBDIRS += apis
#apis.subdir = src/plugins/apis
#apis.depends = base
#unisim.depends = apis

#SUBDIRS += ave
#ave.subdir = src/plugins/ave
#ave.depends = base
#unisim.depends = ave

#SUBDIRS += bph
#bph.subdir = src/plugins/bph
#bph.depends = base
#unisim.depends = bph

#SUBDIRS += bmsb
#bmsb.subdir = src/plugins/bmsb
#bmsb.depends = base
#unisim.depends = bmsb

#SUBDIRS += conductance
#conductance.subdir = src/plugins/conductance
#conductance.depends = base
#unisim.depends = conductance

#SUBDIRS += io
#io.subdir = src/plugins/io
#io.depends = base
#unisim.depends = io

#SUBDIRS += MusselBed
#MusselBed.subdir = src/plugins/MusselBed
#MusselBed.depends = base
#unisim.depends = MusselBed

#SUBDIRS += PestTox
#PestTox.subdir = src/plugins/PestTox
#PestTox.depends = base
#unisim.depends = PestTox

#SUBDIRS += pinacate
#pinacate.subdir = src/plugins/pinacate
#pinacate.depends = base
#unisim.depends = pinacate

#SUBDIRS += pollnet
#pollnet.subdir = src/plugins/pollnet
#pollnet.depends = base
#unisim.depends = pollnet

#SUBDIRS += potato
#potato.subdir = src/plugins/potato
#potato.depends = base
#unisim.depends = potato

#SUBDIRS += resist
#resist.subdir = src/plugins/resist
#resist.depends = base
#unisim.depends = resist

#SUBDIRS += tuta
#tuta.subdir = src/plugins/tuta
#tuta.depends = base
#unisim.depends = tuta

#SUBDIRS += vacuum
#vacuum.subdir = src/plugins/vacuum
#vacuum.depends = base
#unisim.depends = vacuum



