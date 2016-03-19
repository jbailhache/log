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

NB. end of standard profile

mean =: +/ % #
shape =: $

matprod =: +/ . *
matinv =: %.
matdiv =: %.
det =: -/ . *
transpose =: |:

e =: ^1
exp =: ^
log =: ^.
i =: 0j1

count =: #
reverse =: |.

messagebox =: wdinfo

noun =: 0
adverb =: 1
conjunction =: 2
verb =: 3

f1 =: verb : '3 + 2 * y.' 
NB. defines a function f1 which gives 3 + 2 * y. where y. is the right argument

script =: 0

f2 =: verb : script
    messagebox 'Calculating f2'
	4 + 2 * y.
)

pos =: verb : script
	if. y. > 0 do. 1 else. 0 end.
)

fact =: verb : script
	f =. 1
	i =. 1
	while. i < y.+1 do.
		f =. f * i
		i =. i + 1
	end.
	f
)

factr =: verb : script
	if. y. < 2 do. 1
	else. y. * factr y. - 1 end.
)

of =: {

o =. @: 	NB. composition

replacebad =: verb : script	NB. does not work
	ne =. 0 { x.
	i =. 1 { x.
	nv =. (i. i { y.) , ne , ((1 + i) + i. ((($ y.) - i) - 1)) { y.
	nv
)

replacebad2 =: verb : script
	ne =. 0 { x.
	i =. 1 { x.
	n =. # y.
	nv =. ((0 = i = i. n) * y.) + (i = i. n) * ne
	nv
)

transpvect =: verb : script
	(($ y.), 1) $ y.
)

	