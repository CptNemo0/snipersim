
import sys
from importlib import util
def load_file_as_module(name, location):
    sys.path.insert(0,location.rsplit('/', 1)[0])
    spec = util.spec_from_file_location(name, location)
    module = util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module
sys.argv = [ "/home/pawel/snipersim/scripts/periodic-stats.py", "1000:2000" ]
load_file_as_module("periodic-stats","/home/pawel/snipersim/scripts/periodic-stats.py")

sys.argv = [ "/home/pawel/snipersim/scripts/markers.py", "markers" ]
load_file_as_module("markers","/home/pawel/snipersim/scripts/markers.py")

