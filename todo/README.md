# CPU SKU Selection

Raw data from geekbench.com, and ark.intel.com, and some sample data in machine file

Please unzip geek_data.tar.gz before use:

tar xzf geek_data.tar.gz

## Data format

### machine
[]

### Geekbench
['model number', 'freq', 'L2', 'L3', 'memory', 'bench', 'single/multicore', 'score', 'perf']

### Intel chips
['uarch','chip type','launch date','processor number','cache','bus speed','instruction set extensions','instruction set',
   'max memory size','memory types','# of memory channels','max memory bandwidth','ecc memory supported',
   'processor graphics','graphics base frequency','graphics max dynamic frequency','graphics video max memory','# of displays supported','graphics and imc lithography', 'pci express revision','pci express lanes', 'lithography',
   '# of cores','# of threads','base frequency','turbo frequency','tdp-up frequency','tdp-up','tdp-down frequency','tdp-down','tdp',
   'turbo boost technology','vpro','hyper-threading','(vt-x)','(vt-d)','tsx-ni','idle states','speedstep','thermal monitoring','identity protection', 'aes new instructions','secure key', 'trusted execution','execute disable bit','anti-theft', 'price']
