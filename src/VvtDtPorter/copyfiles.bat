set env=%1
set plat=%2

set folder="..\%plat%\%env%\VvtDtPorter\parsers\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\VvtDataStorage.dll ..\%plat%\%env%\VvtDtPorter\ /C /Y
xcopy ..\%plat%\%env%\VvtDataStorageAD.dll ..\%plat%\%env%\VvtDtPorter\ /C /Y
xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\VvtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserXTP.dll ..\%plat%\%env%\VvtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserCTPMini.dll ..\%plat%\%env%\VvtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserCTPOpt.dll ..\%plat%\%env%\VvtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserFemas.dll ..\%plat%\%env%\VvtDtPorter\parsers\ /C /Y