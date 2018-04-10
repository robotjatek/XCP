
TEMPLATE = subdirs

# Select here which component you want to build choices are: gui lib
CONFIG += gui lib

CONFIG(gui) {
    message(Building the Graphical User Interface.)
} else {
    message(Not building the GUI.)
}

CONFIG(lib) {
    message(Building the Library.)
} else {
    message(Not building the Library.)
}

gui {
  SUBDIRS += \
      QtGUI/QtGUI.pro
}

lib {
  SUBDIRS += \
      XCPLib/XCPLib.pro
}

