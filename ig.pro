TEMPLATE = subdirs

#SUBDIRS += base
#base.subdir = src/plugins/base

SUBDIRS += iglib
iglib.subdir = src/lib/iglib
#iglib.depends = base

SUBDIRS += igclient
igclient.subdir = src/apps/igclient
igclient.depends = iglib
#igclient.depends = base
