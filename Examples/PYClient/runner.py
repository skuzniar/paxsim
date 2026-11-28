import importlib.util

from inspect import signature

class Runner:
    def __init__(self, client, simctl, logmon):
        self.client = client
        self.simctl = simctl
        self.logmon = logmon

    def load(self, path):
        # Create a module spec from the given path
        spec = importlib.util.spec_from_file_location("module_name", path)

        # Load the module from the created spec
        module = importlib.util.module_from_spec(spec)

        # Execute the module to make its attributes accessible
        spec.loader.exec_module(module)

        # Return the imported module
        return module

    #--- Build argument list from the argument names
    def args(self, sig):
        args = []
        for argname in list(arg.lower() for arg in sig.parameters.keys()):
            if argname == 'client':
                args.append(self.client)
            elif argname == 'simctl':
                args.append(self.simctl)
            elif argname == 'logmon':
                args.append(self.simctl)
            else:
                argnames = ['client', 'simctl', 'logmon'] 
                raise Exception(f"Invalid argument name: {argname}. Supported argument names are: {argnames}")
        return args

    def run(self, path):
        print("Running [%s]" % path)
        # Purge exchange messages that should be irrelevant at this point
        self.client.purge()
        self.logmon.purge()

        module = self.load(path)

        # By enforcing argument naming we gain the flexibility of which arguments to use
        args = self.args(signature(module.run))

        module.run(*args)

