call "C:/Program Files (x86)/Microsoft Visual Studio/2017/Enterprise/VC/Auxiliary/Build/vcvars64.bat"
set PATH=D:/Qt/Qt5.11.2/5.11.2/msvc2017_64/bin;%PATH%;
cd /D D:/boost_new/out
qmake -r -tp vc "test_boost.pro"
cmd 

