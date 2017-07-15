# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ======================================================================
#

import os
import sys
import importlib

class Models(object):
    """
    Simple plugin-style object for managing a dictionary of models.
    """

    def __init__(self, models_dir):
        """Constructor.
        
        :param models_dir: Directory with Python files for models.
        """
        self.models = {}
        self.models_dir = models_dir
        return


    def initialize(self):
        """Create dictionary of models.
        """
        if not self.models_dir:
            raise ValueError("No models directory set.")
        mfiles = [f for f in os.listdir(self.models_dir) if f.endswith(".py")]
        sys.path.insert(0, self.models_dir)
        for mfile in mfiles:
            mod = importlib.import_module(mfile[:-3])
            model = mod.model()
            if model.KEY in self.models:
                raise ValueError("Models '%s' and '%s' attempting to register with the same key '%s'."
                                 % (self.models[model.KEY].name, model.name, model.KEY,))
            self.models[model.KEY] = model
        return


    def get(self, name):
        """Get model by name.
        
        :param name: Key of model to get.
        :return: Model corresponding to name.
        """
        if not name in self.models:
            msg = "Could not find model '%s' in models.\n" % name
            msg += "Available models:\n"
            for key,model in self.models.items():
                msg += "    '%s' - %s" % (key, model.DESCRIPTION)
            raise ValueError(msg)
        return self.models[name]


# End of file    
