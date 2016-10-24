if [ $# = 0 ] 
then
	echo Missing version argument!
else
	git push origin refs/tags/$1
fi
