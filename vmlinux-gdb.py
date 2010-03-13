import gdb

class DumpLiveMemMapCommand (gdb.Command):
    "Dump a memory map of a system running uClinux."

    def __init__ (self):
        super (DumpLiveMemMapCommand, self).__init__ ("dlmm", 
                                                      gdb.COMMAND_SUPPORT,
                                                      gdb.COMPLETE_NONE, False)

    def invoke (self, arg, from_tty):
        with open (arg, 'w') as f:
            print >> f, "Hello World!"

DumpLiveMemMapCommand()


