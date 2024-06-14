$CurrentDirectory = pwd
$BuildDirectory = $CurrentDirectory.ToString() + "/build/Desktop_Qt_6_6_2_MinGW_64_bit-Debug/"
$CMakeDirectory = $CurrentDirectory.ToString()
$QtPath = "D:/qt/6.6.2/mingw_64/bin"   # Replace it with your own QtPath

$OldPath = $env:Path
$env:Path += ";" + $QtPath
cd $BuildDirectory
cmake -G Ninja $CMakeDirectory
cmake --build .
cd $CurrentDirectory

$env:Path = $OldPath
