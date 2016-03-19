NB. standard j profile

NB. set box display:
9!:3 [ 2

NB. bootstrap script loader:
boot_z_=: 3 : '0!:0 <''main\'',y.'

NB. main libraries
NB. this section should not need modifying:
boot_z_ 'stdlib.js'
boot_z_ 'winlib.js'
boot_j_ 'jadelib.js'

NB. user definitions - modify as required:
boot_z_ 'winutil.js'
boot_z_ 'jadeutil.js'
boot_z_ 'myutil.js'

NB. delete bootstrap:
4!:55 <'boot_z_'

