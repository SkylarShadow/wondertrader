set env=%1
set plat=%2

set folder="..\%plat%\%env%\ZtUftRunner\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\ZtUftRunner\traders\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\ZtUftRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\ZtUftRunner\traders\ /C /Y