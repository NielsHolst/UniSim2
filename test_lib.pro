TEMPLATE = subdirs

SUBDIRS += base
base.subdir = src/plugins/base

SUBDIRS += boxes
boxes.subdir = src/plugins/boxes
boxes.depends = base

SUBDIRS += student
student.subdir = src/plugins/student
student.depends = base

SUBDIRS += unisimlib_test
unisimlib_test.subdir = src/lib/unisimlib_test
unisimlib_test.depends = base

SUBDIRS += unisimlib_test_client
unisimlib_test_client.subdir = src/apps/unisimlib_test_client
unisimlib_test_client.depends = unisimlib_test
unisimlib_test_client.depends = boxes
unisimlib_test_clientunisim.depends = student

