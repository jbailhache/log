cd ..
for f in `cat actions/dist.txt`
do
 zip -r actions/$1 actions/$f
done
cd actions

