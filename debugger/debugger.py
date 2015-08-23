from ctypes import cdll
simple_vm = cdll.LoadLibrary('../lib/libsimplevm-vmbindings.so')
assembler = cdll.LoadLibrary('../lib/libsimplevm-assemblerbindings.so')


class SimpleVM(object):
    def __init__(self):
        simple_vm.init()
        return

    def dump_registers(self):
        simple_vm.dump_registers()
        return

class Assembler(object):
    def __init__(self):
        return;



def debugger():
    asm = Assembler()
    vm = SimpleVM()
    vm.dump_registers()


if __name__ == "__main__":
    debugger()
       
