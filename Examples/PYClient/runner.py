import os
import sys
import importlib.util

class Runner:
    def __init__(self, session):
        self.session  = session

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
        module = self.load(path)
        module.run(self.session)

