bin_dir=$1
if [ -z "$bin_dir" ] 
then
	bin_dir="build"
fi

if [ ! -d "$bin_dir" ]
then
	mkdir "$bin_dir"
fi
cd "$bin_dir"

if [ ! -f "conanbuildinfo.cmake" ]
then
	conan install ..
fi

