import socket
import errno
import json

#--------------------------------------------------------------------------------------------------
# Simulator controller. Connects to simulator control port and sends controller commands.
#--------------------------------------------------------------------------------------------------
class Control:
    def __init__(self, host , port):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def start(self):
        pass
        '''
        try:
            self.sock.connect((self.host, self.port))
        except socket.error:
            pass
        '''

    def stop(self):
        self.sock.close()

    def send(self, command):
        if not self.connected():
            self.sock.connect((self.host, self.port))
        string = json.dumps(command)
        self.sock.send(bytes(string + '\0', "utf-8"))

        response = self.sock.recv(1024)
        if response:
            print(response.decode("utf-8"))

    def connected(self):
        try:
            self.sock.getpeername()
            return True
        except socket.error as err:
            if err.errno in (errno.ENOTCONN, errno.EBADF):
                return False
            raise  # Re-raise unexpected errors   

    # Convenience classes and static methods to simplify constructing controller commands.
    @classmethod
    def form(cls, name, selector, qualifier=None):
        return {name:[{"Selector":selector, "Qualifier":qualifier}]} if qualifier else {name:[{"Selector":{}, "Qualifier":selector}]}

    class Admin:
        @classmethod
        def wrap(cls, name, command):
            ret = {name: command}
            for n in reversed(cls.__qualname__.split('.')):
                ret = {n: ret}
            return ret

        @classmethod
        def Command(cls, command):
            return cls.wrap("Command", command)

    class Order:
        @classmethod
        def wrap(cls, name, selector, qualifier=None):
            ret = Control.form(name, selector, qualifier)
            for n in reversed(cls.__qualname__.split('.')):
                ret = {n: ret}
            return ret

        #--- Reject incoming order messages
        class Reject:
            @classmethod
            def wrap(cls, name, selector, qualifier=None):
                ret = Control.form(name, selector, qualifier)
                for n in reversed(cls.__qualname__.split('.')):
                    ret = {n: ret}
                return ret

            @classmethod
            def Enter(cls, selector, qualifier=None):
                return cls.wrap("Enter", selector, qualifier)

            @classmethod
            def Replace(cls, selector, qualifier=None):
                return cls.wrap("Replace", selector, qualifier)

            @classmethod
            def Cancel(cls, selector, qualifier=None):
                return cls.wrap("Cancel", selector, qualifier)

        @classmethod
        def Cancel(cls, selector, qualifier=None):
            return cls.wrap("Cancel", selector, qualifier)

        @classmethod
        def Fill(cls, selector, qualifier=None):
            return cls.wrap("Fill", selector, qualifier)

    class Fill:
        @classmethod
        def wrap(cls, name, selector, qualifier=None):
            ret = Control.form(name, selector, qualifier)
            for n in reversed(cls.__qualname__.split('.')):
                ret = {n: ret}
            return ret

        @classmethod
        def Cancel(cls, selector, qualifier=None):
            return cls.wrap("Cancel", selector, qualifier)

        @classmethod
        def Correct(cls, selector, qualifier=None):
            return cls.wrap("Correct", selector, qualifier)


#--- Only export the alias
SimCtl = Control
__all__ = ['SimCtl'] 

