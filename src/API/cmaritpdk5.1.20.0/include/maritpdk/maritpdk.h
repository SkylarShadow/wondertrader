#pragma once



#include <map>
#include <vector>
#include <string>
using std::string;
using std::map;
using std::vector;

#include "itpdk/itpdk_typedef.h"
#include "itpdk/itpdk_dict.h"
#include "maritpdk/maritpdk_struct.h"
using namespace MarItpdk;


//////////////////////////////////////////////////////////////////////////
//��ȡʱ������뼶����linux��winͬGetTickCount64��
ITPDK_API int64 MARITPDK_GetTickCount();
//��ȡʱ���΢�뼶����linux��winͬGetTickCount64��
ITPDK_API int64 MARITPDK_GetDoubleTickCount();
//��ѯϵͳ��������
ITPDK_API int64 MARITPDK_GetTradeDate();
////��ȡ������ϵͳ����
ITPDK_API int64 MARITPDK_GetSystemDate();
//��ȡ��������ʱ��(�����׷�����ʱ������)
ITPDK_API int64 MARITPDK_GetReviseTimeAsLong();
//��ȡ��������ʱ��(�����׷�����ʱ������)
ITPDK_API void MARITPDK_GetReviseTime(char *buffer);
//////////////////////////////////////////////////////////////////////////
//���������ļ�����·����ITPDK_Init ����ǰ������Ч
ITPDK_API void MARITPDK_SetProfilePath(const char *path);
// �����Ƿ�����ITPDK�Զ��������б����ܣ�ITPDK_Init ����ǰ������Ч
ITPDK_API void MARITPDK_SetAutoReconnect(bool breconnect);
//�����Ƿ��¼��־
ITPDK_API void MARITPDK_SetWriteLog(bool bLog);
//�����Ƿ��¼Fix��־
ITPDK_API void MARITPDK_SetFixWriteLog(bool bLog);

//��������־·����ITPDK_Init ����ǰ������Ч
ITPDK_API void MARITPDK_SetLogPath(const char *path);
//��ʼ��
ITPDK_API bool MARITPDK_Init(int commsize);
//�˳�
ITPDK_API void MARITPDK_Exit();
//��ȡ���������Ϣ�ʹ�����
ITPDK_API int64 MARITPDK_GetLastError(char *result_msg);
ITPDK_API int64 MARITPDK_GetLastError2(string& result_msg);
//�������´�����Ϣ
ITPDK_API void MARITPDK_SetLastError(int64 result_code, const char *result_msg, const char* result_text = nullptr, ITPDK_CusReqInfo* cusreqinfo = nullptr);

//��ȡ��ǰ��汾��Ϣ
ITPDK_API string MARITPDK_GetVersion();

/*
	������Ϣ�ص�����
		��Ϣ����pMsgΪjson��

		������Ϣ -- NOTIFY_PUSH_WITHDRAW
		�ϵ���Ϣ -- NOTIFY_PUSH_INVALID
		ȷ����Ϣ -- NOTIFY_PUSH_ORDER
            WTH		int64		ί�к�
			KHH		char[]		�ͻ���
            YYB     char[]      Ӫҵ��
            GDH     char[]      �ɶ���
			JYS		char[]		������
			ZQDM	char[]		֤ȯ����
            XWH     char[]      ϯλ��
            BZ      char[]      ����
            SBJG    int64        �걨���
            JYLB    int64        �������
            CXBZ    char        ������־
            CXWTH   int64        ����ί�к�
            CJBH    char[]      �ɽ����
            CJSJ    char[]      �ɽ�ʱ��
            CJSL    int64        �ɽ�����
            CJJG    double      �ɽ��۸�
            CJJE    doube       �ɽ����
		�ɽ���Ϣ -- NOTIFY_PUSH_MATCH
			KHH		char[]		�ͻ���
			JYS		char[]		������
			ZQDM	char[]		֤ȯ����
			WTH		int64		ί�к�
			CJSL	int64		�ɽ�����
			CJJG	double		�ɽ��۸�
		�ʲ��䶯��Ϣ -- NOTIFY_ASSETVARY
			KHH		char[]		�ͻ���
			ZJZH	char[]		�ʽ��˺�
			ZZC		double		���ʲ�
			KYZJ	double		�����ʽ�
			ZXSZ	double		������ֵ

	func		[in]		�ص�����
*/
ITPDK_API void MARITPDK_SetMsgCallback(pMessageCallbackMethod func);

// �첽�ص�����
ITPDK_API void MARITPDK_SetFuncCallback(pAsyncCallbackFunc func);

// ���ӵ��ߡ��ָ��¼��ص�����
/*
pConnEventCallback

pKhh		[in]		�ͻ���
pConnKey	[in]		����������
nEvent		[in]		�¼�:��0���ָ���1���Ͽ�
pData       [in]        �����ֶ�
*/
ITPDK_API void MARITPDK_SetConnEventCallback(pConnEventCallback func);


// ����վ����Ϣ
ITPDK_API bool MARITPDK_SetNode(const char *val);

// ����ί�з�ʽ
ITPDK_API bool MARITPDK_SetWTFS(const char *val);

/*
	���׵�¼
	lpKeyName	[in]		ʹ�õ�����Keyֵ
	khh			[in]		�ͻ���
	pwd			[in]		��������
	return				�ɹ�����>0��ʧ�ܷ���<=0
*/
ITPDK_API int64 MARITPDK_TradeLogin(const char *lpKeyName, const char *khh, const char *pwd);
/*
	���׵�¼
	lpKeyName	[in]		ʹ�õ�����Keyֵ
	cusreqinfo	[in/out]	�ͻ���Ϣ
	return				�ɹ�����>0��ʧ�ܷ���<=0
*/
ITPDK_API int64 MARITPDK_TradeLoginEx(const char* lpKeyName, ITPDK_CusReqInfo& cusreqinfo);

/*
	�ʽ��˺ŵ�¼
	lpKeyName	[in]		ʹ�õ�����Keyֵ
	zjzh		[in]		�ʽ��˺�
	yyb			[in]		Ӫҵ��
	pwd			[in]		��������
	sKHH		[out]		�ͻ��ţ���¼�ɹ���ͻ���ͨ���˲������أ�֮����ͨ���ͻ��Ž��н���
	return				�ɹ�����>0��ʧ�ܷ���<=0
*/

ITPDK_API int64 MARITPDK_TradeLoginByFundAcc(const char* lpKeyName, const char* zjzh, const char* yyb, const char* pwd, string& sKHH);
/*
	�ʽ��˺ŵ�¼
	lpKeyName	[in]		ʹ�õ�����Keyֵ
	cusreqinfo	[in/out]	�ͻ���Ϣ
	return				�ɹ�����>0��ʧ�ܷ���<=0
*/
ITPDK_API int64 MARITPDK_TradeLoginByFundAccEx(const char* lpKeyName, ITPDK_CusReqInfo& cusreqinfo);

/*
    �ǳ� 
    KhhOrZjzh			[in]		�ͻ���
*/
ITPDK_API int64 MARITPDK_TradeLogout(const char* KhhOrZjzh);

/*
	�����޸�

	lpKhh        [in]        �ͻ���
	oldpwd       [in]        ������
	newpwd       [in]        ������
	nType        [in]        �޸����ͣ�0 �޸Ľ������룩
	return                �ɹ�����true��ʧ�ܷ���false
*/
ITPDK_API bool MARITPDK_ModifyPwd(const char* lpKhh, const char* oldpwd, const char* newpwd, int nType);

//////////////////////////////////////////////////////////
//������ӿ�
//////////////////////////////////////////////////////////

/*
	���ý���

	lpKhh		[in]		�ͻ���
	lpJys		[in]		������
	lpZqdm		[in]		֤ȯ����
	nJylb		[in]		�������(JYLB_BUY��JYLB_SALE��JYLB_RZMR��JYLB_MQHK��JYLB_MQHQ��JYLB_RQMC��JYLB_DBWTJ��JYLB_DBWFH��JYLB_HQHZ��JYLB_ZJHK��JYLB_PHDJMR��JYLB_PHDJMC��JYLB_ZZZG��JYLB_PGJK��JYLB_PSSG��JYLB_ZZHS��JYLB_KJHZHG��JYLB_HSCX)
	lWtsl		[in]		ί������
	lfWtjg		[in]		ί�м۸�(��������Ϊ�м�ʱ���ò�������Ч���������Ϊ ֱ�ӻ���ʱ������������ֶ�)
	lDdlx		[in]		��������(DDLX_XJWT,DDLX_SJWT)
	lpGdh		[in]		�ɶ���
	nTCLX		[in]		ͷ������  1��ͨͷ�� 2ר��ͷ��(����: ��0 Ĭ��ר�� ,����: ��0 ������������)
	lpHYBH		[in]		ָ��������ծ��ˮ��
	nChlx		[in]		��������: 0-���˽᷽ʽ) 1-ֻ����Ϣ
	nFlag		[in]	    ����������־����ȯ��תҵ��ר�ã� 0����ʵʱӰ�츺ծ 1������ʵʱӰ�츺 -1����û�����볷��������־��ί��
	return				�ɹ�����ί�кţ�ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_MarginEntrust(const char *lpKhh,const char *lpJys,const char *lpZqdm,int nJylb,int64 lWtsl,double lfWtjg, int64 lDdlx,const char *lpGdh,int nTCLX,const char * lpHYBH,int nChlx,int nFlag);
ITPDK_API int64 MARITPDK_MarginEntrustEx(ITPDK_CusReqInfo& cusreqinfo, const char* lpJys, const char* lpZqdm, int nJylb, int64 lWtsl, double lfWtjg, int64 lDdlx, int nTCLX, const char* lpHYBH, int nChlx, int nFlag);

/*
	���ý��׽ӿ�(�첽)

	lpKhh		[in]		�ͻ���
	lpJys		[in]		������
	lpZqdm		[in]		֤ȯ����
	nJylb		[in]		�������(JYLB_BUY��JYLB_SALE��JYLB_RZMR��JYLB_MQHK��JYLB_MQHQ��JYLB_RQMC��JYLB_DBWTJ��JYLB_DBWFH��JYLB_HQHZ��JYLB_ZJHK��JYLB_PHDJMR��JYLB_PHDJMC��JYLB_ZZZG��JYLB_PGJK��JYLB_PSSG��JYLB_ZZHS��JYLB_KJHZHG��JYLB_HSCX)
	lWtsl		[in]		ί������
	lfWtjg		[in]		ί�м۸�(��������Ϊ�м�ʱ���ò�������Ч)
	lDdlx		[in]		��������(DDLX_XJWT,DDLX_SJWT)
	lpGdh		[in]		�ɶ���
	nTCLX		[in]		ͷ������  1��ͨͷ�� 2ר��ͷ��(����: ��0 Ĭ��ר�� ,����: ��0 ������������)
	lpHYBH		[in]		ָ��������ծ��ˮ��
	nChlx		[in]		��������: 0-���˽᷽ʽ) 1-ֻ����Ϣ
	nFlag		[in]	    ����������־����ȯ��תҵ��ר�ã� 0����ʵʱӰ�츺ծ 1������ʵʱӰ�츺
	nKFSBDBH    [in]        �����̱��ر�ţ���δ�ͣ�����itpdk�Զ�����(�Զ���ί�кţ���ȷ�ϡ��ɽ���������Ϣ�л������ֵ)
	return				�ɹ����ؿ����̱��ر�ţ�ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_MarginEntrust_ASync(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, int64 lWtsl, double lfWtjg, int64 lDdlx, const char* lpGdh, int nTCLX, const char* lpHYBH, int nChlx,  int nFlag, int64 nKFSBDBH = 0);
ITPDK_API int64 MARITPDK_MarginEntrustEx_ASync(ITPDK_CusReqInfo& cusreqinfo, const char* lpJys, const char* lpZqdm, int nJylb, int64 lWtsl, double lfWtjg, int64 lDdlx, int nTCLX, const char* lpHYBH,  int nChlx ,  int nFlag, int64 nKFSBDBH = 0);


/*
	����ί����ͨ����

	lpKhh		[in]		�ͻ���
	lpGdh		[in]		�ɶ���
	lWth		[in]		ί�к�
    nJylb       [in]       �������
	return				�ɹ�����>0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_MarginOrderWithdraw(const char *lpKhh,const char *lpGdh, int64 lWth,int nJylb);
ITPDK_API int64 MARITPDK_MarginOrderWithdrawEx(ITPDK_CusReqInfo& cusreqinfo, int64 lWth, int nJylb);
/*
	����ί����ͨ����(�첽)

	lpKhh		[in]		�ͻ���
	lpGdh		[in]		�ɶ���
	lWth		[in]		ί�к�
	nJylb       [in]       �������
	nKFSBDBH	[in]        ��ˮ�ţ�͸���ֶΣ��ڻص�������ͨ����ֵƥ����ã�
								��δ�͸��ֶΣ��ɽӿ��ڲ����ɡ�
	return				�ɹ�����>0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_MarginOrderWithdraw_ASync(const char* lpKhh, const char* lpGdh, int64 lWth, int nJylb, int64 nKFSBDBH = 0);
ITPDK_API int64 MARITPDK_MarginOrderWithdrawEx_ASync(ITPDK_CusReqInfo& cusreqinfo, int64 lWth, int nJylb, int64 nKFSBDBH = 0);
/*
	����ί�п����̴��볷��

	lpKhh		[in]		�ͻ���
	lpGdh		[in]		�ɶ���
	nCxkfsbdh	[in]		����ί�к�
	nJylb       [in]       �������
	return				�ɹ�����>0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_MarginOrderKFSDMWithdraw(const char* lpKhh, const char* lpGdh, int64 nCxkfsbdh, int nJylb);
ITPDK_API int64 MARITPDK_MarginOrderKFSDMWithdrawEx(ITPDK_CusReqInfo& cusreqinfo, int64 nCxkfsbdh, int nJylb);

/*
	����ί�п����̴��볷��(�첽)

	lpKhh		[in]		�ͻ���
	lpGdh		[in]		�ɶ���
	nCxkfsbdh	[in]		����ί�к�
	nJylb       [in]       �������
	nKFSBDBH	[in]        ��ˮ�ţ�͸���ֶΣ��ڻص�������ͨ����ֵƥ����ã�
								��δ�͸��ֶΣ��ɽӿ��ڲ����ɡ�
	return				�ɹ�����>0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_MarginOrderKFSDMWithdraw_ASync(const char* lpKhh, const char* lpGdh, int64 nCxkfsbdh, int nJylb, int64 nKFSBDBH = 0);
ITPDK_API int64 MARITPDK_MarginOrderKFSDMWithdrawEx_ASync(ITPDK_CusReqInfo& cusreqinfo, int64 nCxkfsbdh, int nJylb, int64 nKFSBDBH = 0);

/*
    ����Ʒ��ת��3rd�ӿڣ�

    lpKhh		[in]		�ͻ���
    lpJymm      [in]        ��������
    lpJys       [in]        ������
    lpZqdm      [in]        ֤ȯ����
    nJylb       [in]        �������
    lWtsl       [in]        ί������
    lDdlx       [in]        ��������
    lpDFGDH     [in]        �Է��ɶ���
    lpDFXW      [in]        �Է�ϯλ
    lpGdh		[in]		�ɶ���
    return				�ɹ�����>0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_MarginGuarantyTran(const char* lpKhh, const char* lpJymm, const char *lpJys, const char *lpZqdm, int nJylb, int64 lWtsl, int64 lDdlx, const char* lpDFGDH, const char* lpDFXW, const char *lpGdh);
ITPDK_API int64 MARITPDK_MarginGuarantyTranEx(ITPDK_CusReqInfo& cusreqinfo, const char *lpJys, const char *lpZqdm, int nJylb, int64 lWtsl, int64 lDdlx); 

/*
    ����Ʒ��ת(�첽)��3rd�ӿڣ�

    lpKhh		[in]		�ͻ���
    lpJymm      [in]        ��������
    lpJys       [in]        ������
    lpZqdm      [in]        ֤ȯ����
    nJylb       [in]        �������
    lWtsl       [in]        ί������
    lDdlx       [in]        ��������
    lpDFGDH     [in]        �Է��ɶ���
    lpDFXW      [in]        �Է�ϯλ
    lpGdh		[in]		�ɶ���
    nKFSBDBH	[in]        ��ˮ�ţ�͸���ֶΣ��ڻص�������ͨ����ֵƥ����ã�
                                ��δ�͸��ֶΣ��ɽӿ��ڲ����ɡ�
    return				�ɹ�����>0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_MarginGuarantyTran_ASync(const char* lpKhh, const char* lpJymm, const char *lpJys, const char *lpZqdm, int nJylb, int64 lWtsl, int64 lDdlx, const char* lpDFGDH, const char* lpDFXW, const char *lpGdh, int64 nKFSBDBH = 0);
ITPDK_API int64 MARITPDK_MarginGuarantyTranEx_ASync(ITPDK_CusReqInfo& cusreqinfo, const char *lpJys, const char *lpZqdm, int nJylb, int64 lWtsl, int64 lDdlx, int64 nKFSBDBH = 0);


/*
	���ÿ�ί����������
	lpKhh		[in]		�ͻ���
	lpJys		[in]		������
	lpZqdm		[in]		֤ȯ����
	nJylb		[in]		�������(JYLB_BUY��JYLB_SALE)
	lfWtjg		[in]		ί�м۸�(��������Ϊ�м�ʱ���ò�������Ч)
	lDdlx		[in]		��������(DDLX_XJWT,DDLX_SJWT)
	lpGdh		[in]		�ɶ���
	lpDfGdh		[in]		�Է��ɶ���
	nTCLX		[in]		ͷ������  1��ͨͷ�� 2ר��ͷ��
	nCHBZ       [in]        ������־��0-���� 1-��ͨ 2-ר��
	lpHYBH      [in]        ��Լ��ţ���ȯ��ת��ָ����Լ��ţ�

*/
ITPDK_API int64 MARITPDK_MarginTradableQty(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, double lfWtjg, int64 lDdlx, const char* lpGdh, const char* lpDfGdh, int nTCLX, int nCHBZ, const char* lpHYBH);
ITPDK_API int64 MARITPDK_MarginTradableQtyEx(ITPDK_CusReqInfo& cusreqinfo,const char *lpJys,const char *lpZqdm,int nJylb,double lfWtjg, int64 lDdlx ,int nTCLX, int nCHBZ, const char* lpHYBH);


//////////////////////////////////////////////////////////
//��ѯ��ӿ�
//////////////////////////////////////////////////////////


/*
	��ѯ�ʽ���Ϣ

	khh			[in]		�ͻ���
	arInfo		[out]		�������
	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryFundInfo(const char* khh, vector<ITPDK_XYZJZH>& arInfo);
ITPDK_API int64 MARITPDK_QueryFundInfoEx(ITPDK_CusReqInfo& cusreqinfo, vector<ITPDK_XYZJZH>& arInfo);

/*
	��ѯ�ɶ���Ϣ

	khh			[in]		�ͻ���
	arInfo		[out]		�������
	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryAccInfo(const char* khh, vector<ITPDK_XYGDH>& arInfo);
ITPDK_API int64 MARITPDK_QueryAccInfoEx(ITPDK_CusReqInfo& cusreqinfo, vector<ITPDK_XYGDH>& arInfo);

/*
	��ѯ���ø�ծ

	lpKhh		[in]		�ͻ���
	lpJys		[in]		������
	lpGdh		[in]		�ɶ���
	lpZqdm		[in]		֤ȯ����
	lpHybh		[in]		��Լ���
	nJylb		[in]		�������A5��̨Ϊ��Լ���0-ȫ����1-���ʣ�2-��ȯ��
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	nKsrq		[in]		��ʼ����
	nJsrq		[in]		��������
	lWth		[in]		ί�к�
	nType		[in]		�Ƿ��˽�(0-���к�Լ,1-���˽� 2-δ�˽᣻A5��̨Ϊ��0-���У�1-Ƿ�2-Ƿȯ��3-δ�˽ᣬ4-���˽�)
	nTclx		[in]		ͷ������(0 �������� 1 ��ͨ 2 ר��)
	arInfo		[out]		�������
	return  ��������

*/
ITPDK_API int64 MARITPDK_QueryDebts(const char* lpKhh, const char* lpJys, const char* lpGdh, const char* lpZqdm, const char* lpHybh, int64 nJylb, int64 nRowcount, const char* nBrowindex, int64 nKsrq, int64 nJsrq, int64 lWth, int64 nType, int64 nTclx, vector<ITPDK_XYFZ>& arInfo);
ITPDK_API int64 MARITPDK_QueryDebtsEx(ITPDK_CusReqInfo& cusreqinfo, const char * lpJys,const char * lpZqdm ,const char * lpHybh, int64 nJylb, int64 nRowcount, const char * nBrowindex, int64 nKsrq, int64 nJsrq, int64 lWth, int64 nType, int64 nTclx, vector<ITPDK_XYFZ>& arInfo);


/*
	���ڲ�ѯ����ί��

	khh			[in]		�ͻ���
	nType		[in]		�Ƿ����ѯ�ɳ�ί�У�0��ѯȫ����1��ѯ�ɳ���
	nSortType	[in]		����ʽ����ǰ��֧������
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	jys			[in]		��������Ϊ��ʱ��ѯ���н�������
	zqdm		[in]		֤ȯ���루Ϊ��ʱ��ѯ���д��룩
	nJylb		[in]		�������
	sCxbz		[in]		������־
	lWth		[in]		ί�кţ�Ϊ��ʱ��ѯ����ί�кţ�
	arInfo		[out]		�������
	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryOrders_MR_DRWT(const char* khh, int nType, int nSortType, int nRowcount, int64 nBrowindex, const char* jys, const char* zqdm, int nJylb, const char* sCxbz, int64 lWth, vector<ITPDK_XYDRWT>& arInfo);
ITPDK_API int64 MARITPDK_QueryOrders_MR_DRVvTEx(ITPDK_CusReqInfo& cusreqinfo, int nType, int nSortType, int nRowcount, int64 nBrowindex, const char *jys, const char *zqdm, int nJylb, const char *sCxbz, int64 lWth, vector<ITPDK_XYDRWT>& arInfo);

/*
	���ڲ�ѯʵʱ�ɽ�


	khh			[in]		�ͻ���
	nType		[in]		�Ƿ��ѯ�ɳ���ί�в��֣�0��ѯȫ����1 ���� ��������CDSL>0���֣�  
	nSortType	[in]		����ʽ��0����1����
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	jys			[in]		��������Ϊ��ʱ��ѯ���н�������
	zqdm		[in]		֤ȯ���루Ϊ��ʱ��ѯ���д��룩
	nJylb		[in]		�������	
	lWth		[in]		ί�кţ�Ϊ��ʱ��ѯ����ί�кţ�
	arInfo		[out]		�������
	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryMatchs_MR_SSCJ(const char *khh, int nType, int nSortType, int nRowcount, int64 nBrowindex, const char *jys, const char *zqdm,int nJylb, int64 lWth, vector<ITPDK_XYSSCJ>& arInfo);
ITPDK_API int64 MARITPDK_QueryMatchs_MR_SSCJEx(ITPDK_CusReqInfo& cusreqinfo, int nType, int nSortType, int nRowcount, int64 nBrowindex, const char* jys, const char* zqdm, int nJylb, int64 lWth, vector<ITPDK_XYSSCJ>& arInfo);

/*
	���ڲ�ѯ�ֲ�


	khh			[in]		�ͻ���
	nFlag		[in]		��չ��־��0����չ��1��չ��
	nSortType	[in]		����ʽ����ǰ��֧������
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	gdh			[in]		�ɶ���
	jys			[in]		��������Ϊ��ʱ��ѯ���н�������
	zqdm		[in]		֤ȯ���루Ϊ��ʱ��ѯ���д��룩
	arInfo		[out]		�������
	
	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryPosition_MR_ZQGL(const char *khh, int nFlag, int nSortType, int nRowcount, int64 nBrowindex,const char * gdh, const char *jys, const char *zqdm, vector<ITPDK_XYZQGL>& arInfo);
ITPDK_API int64 MARITPDK_QueryPosition_MR_ZQGLEx(ITPDK_CusReqInfo& cusreqinfo, int nFlag, int nSortType, int nRowcount, int64 nBrowindex, const char* jys, const char* zqdm, vector<ITPDK_XYZQGL>& arInfo);

/*
	���ڲ�ѯ�����ʲ�


	khh			[in]		�ͻ���
	jys			[in]		������
	zqdm		[in]		֤ȯ����
	nFlag		[in]		��չ��־��0����չ��1��չ��
	
	arInfo		[out]		�������
	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_XYZC(const char *khh, const char* jys, const char* zqdm, int nFlag , vector<ITPDK_XYZC>& arInfo);
ITPDK_API int64 MARITPDK_QueryTradeData_MR_XYZCEx(ITPDK_CusReqInfo& cusreqinfo, const char* jys, const char* zqdm, int nFlag, vector<ITPDK_XYZC>& arInfo);


/*
	���ڲ�ѯ�ͻ�����ȯ��Ϣ


	khh			[in]		�ͻ���
	jys			[in]		������
	zqdm		[in]		֤ȯ����
	tcxz		[in]		ͷ����Դ(1 ��ͨͷ�� 2 ר��ͷ��)
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRQXX(const char* khh, const char* jys, const char* zqdm, int32 tcxz, int nRowcount, int64 nBrowindex, vector<ITPDK_RQZQ>& arInfo);
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRQXXEx(ITPDK_CusReqInfo& cusreqinfo, const char* jys, const char* zqdm, int32 tcxz, int nRowcount, int64 nBrowindex, vector<ITPDK_RQZQ>& arInfo);

/*
	���ڲ�ѯ�ͻ������ʽ���Ϣ


	khh			[in]		�ͻ���
	tcxz		[in]		ͷ����Դ(1 ��ͨͷ�� 2 ר��ͷ��)
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRZJXX(const char* khh, int32 tcxz, vector<ITPDK_KRZJXX>& arInfo);
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRZJXXEx(ITPDK_CusReqInfo& cusreqinfo, int32 tcxz, vector<ITPDK_KRZJXX>& arInfo);


/*
	���ڲ�ѯ�ͻ�����Ȩ��


	khh			[in]		�ͻ���
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_PSQY(const char* khh, vector<ITPDK_XYPSQY>& arInfo);
ITPDK_API int64 MARITPDK_QueryTradeData_MR_PSQYEx(ITPDK_CusReqInfo& cusreqinfo, vector<ITPDK_XYPSQY>& arInfo);

/*
	����ҵ����Ϲ�����

	lpKhh		[in]		�ͻ���
	lpJys		[in]		������
	lpZqdm		[in]		ETF����
	nJylb		[in]		�������(JYLB_PGJK��ɽɿ�,JYLB_PSSG�����깺,JYLB_BUY��������)
	lfWtjg		[in]		ί�м۸�(��������Ϊ�м�ʱ���ò�������Ч)
	lDdlx		[in]		��������(DDLX_XJWT,DDLX_SJWT)
	lpGdh		[in]		�ɶ���
	return				�ɹ����ؿ��깺������ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_FXYWTradableQty(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, double lfWtjg, int32 lDdlx, const char* lpGdh);
ITPDK_API int64 MARITPDK_FXYWTradableQtyEx(ITPDK_CusReqInfo& cusreqinfo, const char* lpJys, const char* lpZqdm, int nJylb, double lfWtjg, int32 lDdlx);

/*
	���ڲ�ѯ֤ȯ����


	jys			[in]		������
	zqdm		[in]		֤ȯ����
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_ZQDM(const char* jys, const char* zqdm, int nRowcount, int64 nBrowindex, vector<ITPDK_XYZQDM>& arInfo);

/*
	���ڲ�ѯ�ʸ�֤ȯ


	jys			[in]		������
	zqdm		[in]		֤ȯ����
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_ZGZQ(const char* jys, const char* zqdm, int nRowcount, int64 nBrowindex, vector<ITPDK_ZGZQ>& arInfo);


/*
	��ѯ���н��׿����ʽ�

	lpKhh		[in]		�ͻ���
	sJymm		[in]		��������
	sZjzh		[in]		�ʽ��˺�
	dKyzj       [out]       ��ѯ�ɹ�,���ؿ����ʽ�
	return				�ɹ�����>=0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_QueryCentreFundAvl(const char* sKhh, const char* sJymm, const char* sZjzh, double& dKyzj, const char* sGyh = nullptr, const char* sGymm = nullptr);

//�ʽ����
/*

	lpKhh		[in]		�ͻ���
	sJymm		[in]		��������
	dFsje		[in]		�������
	sZjzh		[in]		�ʽ��˺�
	sGyh        [in]		��Ա��
	sGymm        [in]		��Ա����
	sZy		    [in]		ժҪ
	return				�ɹ�����>=0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_FundTransIn(const char* sKhh, const char* sJymm, double dFsje, const char* sZjzh = nullptr, const char* sGyh = nullptr, const char* sGymm = nullptr, const char* sZy = nullptr);
//�ʽ����
ITPDK_API int64 MARITPDK_FundTransOut(const char* sKhh, const char* sJymm, double dFsje, const char* sZjzh = nullptr, const char* sGyh = nullptr, const char* sGymm = nullptr, const char* sZy = nullptr);

/*
��ѯA�ɿɵ�������ֲ֣�������


khh			[in]		�ͻ���
arInfo		[out]		�������

return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryPosition_DBHR_All(const char *khh, vector<ITPDK_XYDBHRSL>& arInfo);

/*
	ר��ͷ��֤ȯ���������ţ�
	sKhh		[in]		�ͻ���
	sJys		[in]		������
	sZqdm		[in]		֤ȯ����
	nFssl		[in]		��������
	nYysl		[in]		ԤԼ������������
	nFlag		[in]		��������,0ΪMDB��������̨��1λ��̨����MDB
	arInfo		[out]		�������

	return				�ɹ�����>0��ʧ�ܷ���<=0
*/
ITPDK_API int64 MARITPDK_SpecialPosition_ZQDB(const char* sKhh, const char* sJys, const char* sZqdm, int64 nFssl, int64 nYysl, int64 nFlag, vector<ITPDK_ZXTCZQDB>& arInfo);

/*
	ר��ͷ���ʽ���������ţ�
	sKhh		[in]		�ͻ���
	dFsje		[in]		�������
	nFlag		[in]		��������,0ΪMDB��������̨��1λ��̨����MDB
	arInfo		[out]		�������

	return				�ɹ�����>0��ʧ�ܷ���<=0
*/
ITPDK_API int64 MARITPDK_SpecialPosition_ZJDB(const char* sKhh, double dFsje, int64 nFlag);

/*
	��ѯ֤ȯ������
	sJys		[in]		������
	sXysx		[in]		�������� �������� "01" - ���ж�  "02" - ������
	nType		[in]		����(1-���ж�֤ȯ���� 2-���ж�֤ȯ��� 3-֤ȯ������֤ȯ����)
	arInfo		[out]		�������

	return				�ɹ�����>0��ʧ�ܷ���<=0
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_ZQHMD(const char* sJys, const char* sXysx, long nType, vector<ITPDK_ZQHMD>& arInfo);

/*
	��ѯ�¹��깺��Ϣ
	sJys		[in]		������
	sZqdm		[in]		֤ȯ����
	nRgrq		[in]		�Ϲ�����
	sFxfs		[in]		���з�ʽ
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryNewStockInfo(const char* sJys, const char* sZqdm, int64 nRgrq, const char* sFxfs, vector<ITPDK_XGSG>& arInfo);

/*
	��ѯ��ծ�䶯��ϸ
	sKhh		[in]		�ͻ���
	sJys		[in]		������
	sZqdm		[in]		֤ȯ����
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_DRBD(const char* sKhh, const char* sJys, const char* sZqdm, int nRowcount, int nBrowindex, vector<ITPDK_FZBDMX>& arInfo);

/*
	���ʲֵ���ϸ��ѯ�����֤ȯ��
	sKhh		[in]		�ͻ���
	sJymm		[in]		��������
	sYyb		[in]		Ӫҵ��
	sJys		[in]		������
	sBz			[in]		����
	sKsrq		[in]		��ʼ����
	sJsrq		[in]		��������
	sWtfs		[in]		ί�з�ʽ
	sRqlx		[in]		�����������ͣ�0:�ֵ��������� 1:�ֵ��������ڣ�Ĭ��Ϊ 0��
	nLjbs		[in]		�˽��ʶ
	sFlag		[in]		�Ƿ����ʵʱί�У�0���:�� 1:�ǣ�
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_RZCDMX(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sBz, const char* sKsrq, const char* sJsrq, const char* sWtfs, const char* sRqlx, int nLjbs, const char* sFlag, vector<ITPDK_RZCDMX>& arInfo);

/*
	��ȯ�ֵ���ϸ��ѯ�����֤ȯ��
	sKhh		[in]		�ͻ���
	sJymm		[in]		��������
	sYyb		[in]		Ӫҵ��
	sJys		[in]		������
	sBz			[in]		����
	sKsrq		[in]		��ʼ����
	sJsrq		[in]		��������
	sWtfs		[in]		ί�з�ʽ
	sRqlx		[in]		�����������ͣ�0:�ֵ��������� 1:�ֵ��������ڣ�Ĭ��Ϊ 0��
	nLjbs		[in]		�˽��ʶ
	sFlag		[in]		�Ƿ����ʵʱί�У�0���:�� 1:�ǣ�
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_RQCDMX(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sBz, const char* sKsrq, const char* sJsrq, const char* sWtfs, const char* sRqlx, int nLjbs, const char* sFlag, vector<ITPDK_RQCDMX>& arInfo);

/*
	������ȯչ�ڣ����֤ȯ��
	sKhh		[in]		�ͻ���
	sJymm		[in]		��������
	sYyb		[in]		Ӫҵ��
	sJys		[in]		������
	sCdh		[in]		�ֵ���
	sHylx		[in]		��Լ����
	sKcrq		[in]		��������
	sKsrq		[in]		��ʼ����
	sJsrq		[in]		��������
	sWtfs		[in]		ί�з�ʽ

	return				�ɹ�����>=0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_ContractExtension(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sCdh, const char* sHylx, const char* sKcrq, const char* sKsrq, const char* sJsrq, const char* sWtfs);

/*
	�ֵ��������ܾ�ԭ���ѯ�����֤ȯ��
	sKhh		[in]		�ͻ���
	sJymm		[in]		��������
	sYyb		[in]		Ӫҵ��
	sJys		[in]		������
	sKsrq		[in]		��ʼ����
	sJsrq		[in]		��������
	sWtfs		[in]		ί�з�ʽ
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_CDBGSQ_REFUSE(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sKsrq, const char* sJsrq, const char* sWtfs, vector<ITPDK_CDBGSQJJYYCX>& arInfo);

/*
	�ֵ���������ѯ�����֤ȯ��
	sKhh		[in]		�ͻ���
	sJymm		[in]		��������
	sYyb		[in]		Ӫҵ��(Դ)
	sJys		[in]		������
	sJgdm		[in]		Ӫҵ��
	sZqdm		[in]		֤ȯ����
	sKsrq		[in]		��ʼ����
	sJsrq		[in]		��������
	sHylx		[in]		��Լ����
	sTzlx		[in]		��������
	sCpbh		[in]		��Ʒ���
	sSqzt		[in]		����״̬
	sWtfs		[in]		ί�з�ʽ
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_CDBGSQ(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sJgdm, const char* sZqdm, const char* sKsrq, const char* sJsrq, const char* sHylx, const char* sTzlx, const char* sCpbh, const char* sSqzt, const char* sWtfs, vector<ITPDK_CDBGSQCX>& arInfo);


/*
	��ѯ�¹���ǩ�����֤ȯ��

	sKhh		[in]        �ͻ���
	sJymm		[in]        ��������
	sYyb		[in]		Ӫҵ��
	sJys		[in]        �г�����
	sZqdm		[in]        ֤ȯ����
	sKsrq		[in]        ��ʼ����
	sJsrq		[in]        ��������
	nRowCount   [in]        ��ѯ����
	sSyrq		[in]        ��������
	sSyYybdm	[in]        ����Ӫҵ������
	sBrowindex	[in]        ������ˮ��
	sWtfs		[in]        ί�з�ʽ
	arInfo      [out]       �������

	return                  ���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_XGZQ(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sZqdm, const char* sKsrq, const char* sJsrq, int nRowCount, const char* sSyrq, const char* sSyYybdm, const char* sBrowindex, const char* sWtfs, vector<ITPDK_XGZQ>& arInfo);

/*
	��Ų�ѯ�����֤ȯ��

	sKhh		[in]        �ͻ���
	sJymm		[in]        ��������
	sYyb		[in]        Ӫҵ��
	sJys		[in]        �г�����
	sGdh		[in]        �ɶ�����
	sPhdm		[in]        ��Ŵ���
	sKsrq		[in]        ��ʼ����
	sJsrq		[in]        ��������
	nRowCount   [in]        ��ѯ����
	sBrowindex	[in]        ������ˮ��
	sWtfs		[in]        ί�з�ʽ

	arInfo      [out]       �������
	return                  ���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_PHCX(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sGdh, const char* sPhdm, const char* sKsrq, const char* sJsrq, int nRowCount, const char* sBrowindex, const char* sWtfs, vector<ITPDK_PHCX>& arInfo);

/*
	��ѯ��֤ҵ����ˮ�����֤ȯ��

	sKhh		[in]        �ͻ���
	sJymm		[in]        ��������
	sYyb		[in]		Ӫҵ��
	sScdm		[in]        �г�����
	sYhdm		[in]        ���д���
	sYhzh		[in]        �����˺�
	nRowCount   [in]        ��ѯ����
	sLsh		[in]        ��ˮ��
	sWtfs		[in]        ί�з�ʽ
	sBrowindex	[in]		������ˮ��
	nFlag		[in]        �Ƿ�ȡ���˻���ˮ��0:�� 1:�ǣ�Ĭ��Ϊ 0��

	arInfo      [out]       �������
	return                  ���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_YZYWLS(const char* sKhh, const char* sJymm, const char* sYyb, const char* sScdm, const char* sYhdm, const char* sYhzh, int nRowCount, const char* sLsh, const char* sWtfs, const char* sBrowindex, int nFlag, vector<ITPDK_YZYWLS>& arInfo);

/*
	��֤ת��ҵ�񣨹��֤ȯ��

	sKhh		[in]        �ͻ���
	sJymm		[in]        ��������
	sZjzh		[in]        �ʽ��˺�
	sYyb		[in]		Ӫҵ��
	sScdm		[in]        �г�����
	sYhdm		[in]        ���д���
	sYhzh		[in]        �����˺�
	nYwlb       [in]        ҵ�����1:����ת֤ȯ��2:֤ȯת���У�
	dZzje		[in]        ת�˽��
	sYhmm		[in]        ��������
	sQkmm		[in]        ȡ������
	sWtfs		[in]        ί�з�ʽ

	return				�ɹ�����>=0��ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_BankBusiness(const char* sKhh, const char* sJymm, const char* sZjzh, const char* sYyb, const char* sScdm, const char* sYhdm, const char* sYhzh, int nYwlb, double dZzje, const char* sYhmm, const char* sQkmm, const char* sWtfs);

/*
	��ѯ�ͻ����д��루���֤ȯ��

	sKhh		[in]        �ͻ���
	sJymm		[in]        ��������
	sYyb		[in]		Ӫҵ��
	sScdm		[in]        �г�����
	sWtfs		[in]        ί�з�ʽ
	nYhlb		[in]		�������
	nFlag		[in]		�Ƿ�ȡ���˻���ˮ��0:��1:�ǣ�Ĭ��Ϊ0��

	arInfo      [out]       �������
	return                  ���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_YHDM(const char* sKhh, const char* sJymm, const char* sYyb, const char* sScdm, const char* sWtfs, int nYhlb, int nFlag, vector<ITPDK_YHDM>& arInfo);

/*
	��ѯ���������֤ȯ��

	sKhh		[in]        �ͻ���
	sJymm		[in]        ��������
	sZjzh		[in]        �ʽ��˺�
	sYhdm		[in]        ���д���
	sYhzh		[in]        �����˺�
	sYhmm		[in]        ��������
	sQkmm		[in]        ȡ������

	arInfo      [out]       �������
	return                  ���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_YHYE(const char* sKhh, const char* sJymm, const char* sZjzh, const char* sYhdm, const char* sYhzh, const char* sYhmm, const char* sQkmm, vector<ITPDK_YHYE>& arInfo);

/*
	��ѯ�ͻ���Ϣ

	sKhh        [in]        �ͻ���

	arInfo      [out]       �������
	return                  ���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_KHXX(const char* sKhh, vector<ITPDK_KHXX>& arInfo);

/*
	��ѯ�ͻ��ʽ𶳽���ϸ

	sKhh		[in]        �ͻ���
	sJymm		[in]        ��������
	sZjzh		[in]        �ʽ��˺�
	nDjlb		[in]        �������
	sLsh		[in]        ��ˮ��
	nRowCount   [in]        ��ѯ����
	nBrowindex	[in]		��ҳ����ֵ

	arInfo      [out]       �������
	return                  ���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_ZJDJMX(const char* sKhh, const char* sJymm, const char* sZjzh, int64 nDjlb, const char* sLSH, int64 nRowCount, int64 nBrowIndex, vector<ITPDK_ZJDJMX>& arInfo);

/*
	�ͻ�������ծ����(����֤ȯ)

	lpKeyName	[in]		ʹ�õ�����Keyֵ
	sKhh		[in]		�ͻ���
	sLogin		[in]		��Ա�˺�
	sLoginPwd	[in]		��Ա����
	dQtfy		[in]		������ծ�䶯���

	return				�ɹ�����>0��ʧ�ܷ���<=0
*/
ITPDK_API int64 MARITPDK_ModifyQTFY(const char* lpKeyName, const char* sKhh, const char* sLogin, const char* sLoginPwd, double dQtfy);

/*
	���ڲ�ѯ�ͻ�����ȯ��Ϣ(����֤ȯ)

	lpKeyName	[in]		ʹ�õ�����Keyֵ
	khh			[in]		�ͻ���
	sLogin		[in]		��Ա�˺�
	sLoginPwd	[in]		��Ա����
	jys			[in]		������
	zqdm		[in]		֤ȯ����
	tcxz		[in]		ͷ����Դ(1 ��ͨͷ�� 2 ר��ͷ��)
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRQXX_ZX(const char* lpKeyName, const char* khh, const char* sLogin, const char* sLoginPwd, const char* jys, const char* zqdm, int32 tcxz, int nRowcount, int64 nBrowindex, vector<ITPDK_RQZQ>& arInfo);

/*
	���ڲ�ѯ�ͻ������ʽ���Ϣ(����֤ȯ)

	lpKeyName	[in]		ʹ�õ�����Keyֵ
	khh			[in]		�ͻ���
	sLogin		[in]		��Ա�˺�
	sLoginPwd	[in]		��Ա����
	tcxz		[in]		ͷ����Դ(1 ��ͨͷ�� 2 ר��ͷ��)
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRZJXX_ZX(const char* lpKeyName, const char* khh, const char* sLogin, const char* sLoginPwd, int32 tcxz, vector<ITPDK_KRZJXX>& arInfo);

/*
	ר��ͷ��֤ȯ����������֤ȯ��

	lpKeyName	[in]		ʹ�õ�����Keyֵ
	sKhh		[in]		�ͻ���
	sLogin		[in]		��Ա�˺�
	sLoginPwd	[in]		��Ա����
	sJys		[in]		������
	sZqdm		[in]		֤ȯ����
	nFssl		[in]		��������
	nYysl		[in]		ԤԼ������������
	nFlag		[in]		��������,0ΪMDB��������̨��1λ��̨����MDB
	arInfo		[out]		�������

	return				�ɹ�����>0��ʧ�ܷ���<=0
*/
ITPDK_API int64 MARITPDK_SpecialPosition_ZQDB_ZX(const char* lpKeyName, const char* sKhh, const char* sLogin, const char* sLoginPwd, const char* sJys, const char* sZqdm, int64 nFssl, int64 nYysl, int64 nFlag, vector<ITPDK_ZXTCZQDB>& arInfo);

/*
	���ڲ�ѯ��ȯ��ծ����
	sKhh		[in]		�ͻ���
	sJys		[in]		����������֤ȯ����ͬʱ����Ч,�����ѯȫ����
	sZqdm		[in]		֤ȯ���루�뽻����ͬʱ����Ч,�����ѯȫ����
	nRowcount	[in]		��������
	nBrowindex	[in]		��ҳ������
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_RQFZHZ(const char* sKhh, const char* sJys, const char* sZqdm, int64 nRowCount, int64 nBrowIndex, vector<ITPDK_XYRQFZHZ>& arInfo);

/*
	A5���ڲ�ѯ�ʸ�֤ȯ


	sJys		[in]		������
	sZqdm		[in]		֤ȯ����
	nRzzt		[in]		����״̬
	nRqzt		[in]		��ȯ״̬
	nRowcount	[in]		��������
	sBrowindex	[in]		��ҳ������
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_A5MR_ZGZQ(const char* sJys, const char* sZqdm, int64 nRzzt, int64 nRqzt, int64 nRowcount, const char *sBrowindex, vector<ITPDK_A5ZGZQ>& arInfo);

/*
	A5���ڲ�ѯ����֤ȯ


	sJys		[in]		������
	sZqdm		[in]		֤ȯ����
	nJyzt		[in]		����״̬
	nRowcount	[in]		��������
	sBrowindex	[in]		��ҳ������
	arInfo		[out]		�������

	return				���ز�ѯ���ļ�¼��
*/
ITPDK_API int64 MARITPDK_QueryTradeData_A5MR_DBZQ(const char* sJys, const char* sZqdm, int64 nJyzt, int64 nRowcount, const char* sBrowindex, vector<ITPDK_A5DBZQ>& arInfo);

/*
	���ڽ���ί��

	lpKhh		[in]		�ͻ���
	lpJys		[in]		������
	lpZqdm		[in]		֤ȯ����
	nJylb		[in]		�������
	lWtsl		[in]		ί������
	lfWtjg		[in]		ί�м۸�
	lDdlx		[in]		��������
	lpGdh		[in]		�ɶ���

	lpDfgdh		[in]		�Է��ɶ���
	lpDfxw		[in]		�Է�ϯλ
	lpLxr		[in]		��ϵ��
	lpLxdh		[in]		��ϵ�绰
	lpPdcj		[in]		�Ƿ���Գɽ���1Ϊ��Գɽ�
	lpYdh		[in]		Լ����
	lTdbz		[in]		�ض��ɷݴ��ڽ��ױ�־
	return				�ɹ�����ί�кţ�ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_BlockTrading(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, long lWtsl, double lfWtjg, long lDdlx, const char* lpGdh, const char* lpDfgdh, const char* lpDfxw, const char* lpLxr, const char* lpLxdh, const char* lpPdcj, const char* lpYdh, long lTdbz);

/*
	���ڽ��׿�������������

	lpKhh		[in]		�ͻ���
	lpJys		[in]		������
	lpZqdm		[in]		֤ȯ����
	nJylb		[in]		�������
	lfWtjg		[in]		ί�м۸�
	lDdlx		[in]		��������
	lpGdh		[in]		�ɶ���

	lTdbz		[in]		�ض��ɷݴ��ڽ��ױ�־
	return				�ɹ����ؿ�ί��������ʧ�ܷ���<0
*/
ITPDK_API int64 MARITPDK_BlockTrading_TradableQty(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, double lfWtjg, long lDdlx, const char* lpGdh, long lTdbz);
