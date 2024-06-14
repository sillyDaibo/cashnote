$PresentPath = pwd
$QtPath = "D:/qt/6.6.2/mingw_64/bin"  # Replace it with your own QtPath
$ExePath = $PresentPath.ToString() + "/build/Desktop_Qt_6_6_2_MinGW_64_bit-Debug/cashnote.exe"

cd $QtPath
Start-Process $ExePath
cd $PresentPath

