set env=%1
set plat=%2

set folder="..\%plat%\%env%\ZtRunner\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\ZtRunner\traders\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\ZtRunner\executer\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\ZtRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserUDP.dll ..\%plat%\%env%\ZtRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\ZtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderXTP.dll ..\%plat%\%env%\ZtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderMocker.dll ..\%plat%\%env%\ZtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPMini.dll ..\%plat%\%env%\ZtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPOpt.dll ..\%plat%\%env%\ZtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderFemas.dll ..\%plat%\%env%\ZtRunner\traders\ /C /Y

xcopy ..\%plat%\%env%\ZtExeFact.dll ..\%plat%\%env%\ZtRunner\executer\ /C /Y

xcopy ..\%plat%\%env%\ZtDataStorage.dll ..\%plat%\%env%\ZtRunner\ /C /Y
xcopy ..\%plat%\%env%\ZtDataStorageAD.dll ..\%plat%\%env%\ZtRunner\ /C /Y
xcopy ..\%plat%\%env%\ZtRiskMonFact.dll ..\%plat%\%env%\ZtRunner\ /C /Y
xcopy ..\%plat%\%env%\ZtMsgQue.dll ..\%plat%\%env%\ZtPorter\ /C /Y
