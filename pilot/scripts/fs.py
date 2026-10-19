import sys, os
sys.path.insert(0, os.path.expanduser("~/dev/driver1/tools"))
import fnscore
d = sys.argv.pop(1)
os.makedirs(d, exist_ok=True)
fnscore.B = d
sys.argv[0] = "fnscore.py"
fnscore.main()
