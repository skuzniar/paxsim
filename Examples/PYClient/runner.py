import importlib.util

from inspect import signature

class Runner:
    def __init__(self, fixapp, logmon):
        self.fixapp = fixapp
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

    def run(self, path):
        print("Running [%s]" % path)
        # Purge exchange messages that should be irrelevant at this point
        self.fixapp.purge()
        self.logmon.purge()

        module = self.load(path)

        sig = signature(module.run)
        if len(sig.parameters) == 1:
            module.run(self.fixapp)
        else:
            module.run(self.fixapp, self.logmon)

