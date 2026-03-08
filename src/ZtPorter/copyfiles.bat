set env=%1
set plat=%2

set folder="..\%plat%\%env%\ZtPorter\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\ZtPorter\traders\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\ZtPorter\executer\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserUDP.dll ..\%plat%\%env%\ZtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserShm.dll ..\%plat%\%env%\ZtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\ZtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderXTP.dll ..\%plat%\%env%\ZtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderMocker.dll ..\%plat%\%env%\ZtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPMini.dll ..\%plat%\%env%\ZtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPOpt.dll ..\%plat%\%env%\ZtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderFemas.dll ..\%plat%\%env%\ZtPorter\traders\ /C /Y

xcopy ..\%plat%\%env%\ZtExeFact.dll ..\%plat%\%env%\ZtPorter\executer\ /C /Y

xcopy ..\%plat%\%env%\ZtDataStorage.dll ..\%plat%\%env%\ZtPorter\ /C /Y
xcopy ..\%plat%\%env%\ZtDataStorageAD.dll ..\%plat%\%env%\ZtPorter\ /C /Y
xcopy ..\%plat%\%env%\ZtRiskMonFact.dll ..\%plat%\%env%\ZtPorter\ /C /Y
xcopy ..\%plat%\%env%\ZtMsgQue.dll ..\%plat%\%env%\ZtPorter\ /C /Y
