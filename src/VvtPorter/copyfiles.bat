set env=%1
set plat=%2

set folder="..\%plat%\%env%\VvtPorter\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\VvtPorter\traders\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\VvtPorter\executer\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserUDP.dll ..\%plat%\%env%\VvtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserShm.dll ..\%plat%\%env%\VvtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\VvtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderXTP.dll ..\%plat%\%env%\VvtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderMocker.dll ..\%plat%\%env%\VvtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPMini.dll ..\%plat%\%env%\VvtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPOpt.dll ..\%plat%\%env%\VvtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderFemas.dll ..\%plat%\%env%\VvtPorter\traders\ /C /Y

xcopy ..\%plat%\%env%\VvtExeFact.dll ..\%plat%\%env%\VvtPorter\executer\ /C /Y

xcopy ..\%plat%\%env%\VvtDataStorage.dll ..\%plat%\%env%\VvtPorter\ /C /Y
xcopy ..\%plat%\%env%\VvtDataStorageAD.dll ..\%plat%\%env%\VvtPorter\ /C /Y
xcopy ..\%plat%\%env%\VvtRiskMonFact.dll ..\%plat%\%env%\VvtPorter\ /C /Y
xcopy ..\%plat%\%env%\VvtMsgQue.dll ..\%plat%\%env%\VvtPorter\ /C /Y
