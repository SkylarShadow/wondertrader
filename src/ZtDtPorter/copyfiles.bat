set env=%1
set plat=%2

set folder="..\%plat%\%env%\ZtDtPorter\parsers\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ZtDataStorage.dll ..\%plat%\%env%\ZtDtPorter\ /C /Y
xcopy ..\%plat%\%env%\ZtDataStorageAD.dll ..\%plat%\%env%\ZtDtPorter\ /C /Y
xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\ZtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserXTP.dll ..\%plat%\%env%\ZtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserCTPMini.dll ..\%plat%\%env%\ZtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserCTPOpt.dll ..\%plat%\%env%\ZtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserFemas.dll ..\%plat%\%env%\ZtDtPorter\parsers\ /C /Y