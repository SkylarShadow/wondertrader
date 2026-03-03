set env=%1
set plat=%2

set folder="..\%plat%\%env%\VvtRunner\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\VvtRunner\traders\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\VvtRunner\executer\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\VvtRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserUDP.dll ..\%plat%\%env%\VvtRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\VvtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderXTP.dll ..\%plat%\%env%\VvtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderMocker.dll ..\%plat%\%env%\VvtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPMini.dll ..\%plat%\%env%\VvtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPOpt.dll ..\%plat%\%env%\VvtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderFemas.dll ..\%plat%\%env%\VvtRunner\traders\ /C /Y

xcopy ..\%plat%\%env%\VvtExeFact.dll ..\%plat%\%env%\VvtRunner\executer\ /C /Y

xcopy ..\%plat%\%env%\VvtDataStorage.dll ..\%plat%\%env%\VvtRunner\ /C /Y
xcopy ..\%plat%\%env%\VvtDataStorageAD.dll ..\%plat%\%env%\VvtRunner\ /C /Y
xcopy ..\%plat%\%env%\VvtRiskMonFact.dll ..\%plat%\%env%\VvtRunner\ /C /Y
xcopy ..\%plat%\%env%\VvtMsgQue.dll ..\%plat%\%env%\VvtPorter\ /C /Y
