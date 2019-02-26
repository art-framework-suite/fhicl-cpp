# quick python string grabbing stuff


import os
import sys
sys.path.insert(0, os.path.abspath('_extensions'))


project = 'fhiclcpp'
copyright = '2018'
author = 'artists'

# more general stuff
src = os.environ['MRB_SOURCE']
src_dir = os.environ['MRB_SOURCE']+'/'+project




def get_deps(path2deps):

  products = 0 
  depends_on = []
  
  with open(path2deps) as deps_file:
    for line in deps_file:
      val = line.split()
      if(not val): continue
      if(val[0] == 'end_product_list'): products = 0 
      # product list
      if(products):
        item = [val[0], val[1]]
        depends_on.append(item)
      if(val[0] == 'product'): products = 1

  return depends_on

deps = get_deps(src+'/'+project+'/ups/product_deps')

with open(src+'/'+project+'/docs/depends.rst', 'w+') as deps_page:
#with open('depends.rst', 'w+') as deps_page:
  deps_page.write('|depends| depends')
  deps_page.write('\n=================\n')
  for pkg in deps:
    deps_page.write(pkg[0]+pkg[1]+'\n')
  #deps_page.seek(0)
  #deps_page.read()
