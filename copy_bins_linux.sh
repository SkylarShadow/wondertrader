despath=$1
if test "$despath" = ""; then 
	despath="../VvtPy"
fi

echo "VvtPy path is $despath"

root="./src/build_all/build_x64/Release/bin"
folders=("Loader" "VvtBtPorter" "VvtDtPorter" "VvtPorter")
for folder in ${folders[@]}
do
	cp -rvf $root/$folder/*.so $despath/VvtPy/wrapper/linux
	for file in `ls $root/$folder`
	do
		if [ -d $root"/"$folder"/"$file ]
		then
			cp -rvf $root/$folder/$file/*.so $despath/VvtPy/wrapper/linux/$file
		fi
	done
done
