# quick python string grabbing stuff


import os
import sys
sys.path.insert(0, os.path.abspath('_extensions'))




project = 'fhiclcpp'
copyright = '2018'
author = 'artists'

# more general stuff
src=os.environ['MRB_SOURCE']
src_dir=os.environ['MRB_SOURCE']+'/'+project



with open(src_dir+'/ups/product_deps') as deps_file:
  for line in deps_file:
    val = line.split()
    if(val[0] == 'product'):
      print(line)
