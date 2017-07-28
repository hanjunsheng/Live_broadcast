#include "MyDao.h"

CMyDao::CMyDao()
{
	m_binitComFlag = FALSE;
	InitCom();
}
CMyDao::~CMyDao()
{
	releaseCom();
}
BOOL CMyDao::InitCom()
{
	#if _WIN32_WINNT > 0x500
		//保留字（必须为null） ，加载方式 COINIT_MULTITHREADED多线程的方式加载
		// 以多线程方式打开com通道
		::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	#else
		::CoInitialize(NULL);
	#endif
		m_binitComFlag = TRUE;
	return TRUE;
}
void CMyDao::releaseCom()
{
	if (m_binitComFlag)
	{
		::CoUninitialize();
	}
}
////////////////////////////////////////////////////////////////////
//初始化数据库
BOOL CMyDao::OpenDateBase(TCHAR* strDateBaseName,int nDataType,TCHAR* strUser,TCHAR* strPassWord,TCHAR* strIP)
{
	//"colin",DATEBASE_TYPE_SQL2005,"sa","sa","127.0.0.1,2433"
	if (nDataType == DATEBASE_TYPE_ACCESS)
	{
		//CFileFind filefind;
		//BOOL bFind = filefind.FindFile(strDateBaseName);

		//if (!bFind)
		//{
		//	m_strErrorMsg = _T("找不到数据库");
		//	return FALSE;
		//}
		//else
		//{
		//	m_strConnect.Format(_T("provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s"),strDateBaseName);
		//}
	}
	else if (nDataType == DATEBASE_TYPE_SQL2000)
	{
		_stprintf(m_strConnect, _T("Driver={SQL Server};Server=%s;Uid=%s;Pwd=%s;Database=%s"), strIP,strUser,strPassWord,strDateBaseName);
		//m_strConnect.Format(_T("Driver={SQL Server};Server=%s;Uid=%s;Pwd=%s;Database=%s"),strIP,strUser,strPassWord,strDateBaseName);
	}
	else if (nDataType == DATEBASE_TYPE_SQL2005)
	{
		_stprintf(m_strConnect, _T("Driver={SQL Native Client};Server=%s;Uid=%s;Pwd=%s;Database=%s"), strIP,strUser,strPassWord,strDateBaseName);
		//m_strConnect.Format(_T("Driver={SQL Native Client};Server=%s;Uid=%s;Pwd=%s;Database=%s"),strIP,strUser,strPassWord,strDateBaseName);
	}
	else
	{
		_tcscpy(m_strErrorMsg, _T("没有合适的连库串"));
		return FALSE;
	}

	//实例化指针对象
	HRESULT hRes;
	//把CString转换成com组件里的字符串变量
	_bstr_t bstrConnection(m_strConnect);
	_bstr_t bstrUser(strUser);
	_bstr_t bstrPassword(strPassWord);

	try
	{
		//创建连接对象 序列码
		hRes = m_pConnection.CreateInstance(__uuidof(Connection));
		if (!SUCCEEDED(hRes))
		{
			_tcscpy(m_strErrorMsg, _T("实例化连接指针对象失败"));
			//m_strErrorMsg = _T("实例化连接指针对象失败");
			return FALSE;
		}

		//连接数据库(连库串，用户名，密码,打开方式)
		//字符串要用的是com组件的字符串 adModeShareDenyNone多用户共同可以访问一个数据库
		hRes = m_pConnection->Open(bstrConnection,bstrUser,bstrPassword,adModeShareDenyNone);
		if (!SUCCEEDED(hRes))
		{
			_tcscpy(m_strErrorMsg, _T("打开数据库失败"));
			//m_strErrorMsg = _T("打开数据库失败");
			return FALSE;
		}

		//实例化数据集指针对象
		hRes = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if (!SUCCEEDED(hRes))
		{
			_tcscpy(m_strErrorMsg, _T("实例化数据集指针对象失败"));
			//m_strErrorMsg = _T("实例化数据集指针对象失败");
			return FALSE;
		}
		//实例化命令集指针对象
		hRes = m_pCommandmsg.CreateInstance(__uuidof(Command));
		if (!SUCCEEDED(hRes))
		{
			_tcscpy(m_strErrorMsg, _T("实例化命令集指针对象失败"));
			//m_strErrorMsg = _T("实例化命令集指针对象失败");
			return FALSE;
		}

	}catch(_com_error *e)
	{
		//AfxMessageBox(e->Description());
		//m_strErrorMsg.Format(_T("%s"),e->Description());
		_stprintf(m_strErrorMsg, _T("%s"), e->Description());
		return FALSE;
	}

	return TRUE;
}

//关闭数据库
void CMyDao::CloseDataBase()
{
	//TODO:
	m_pConnection->Close();
	//m_pConnection.DestroyInstance();
}

//添加数据
BOOL CMyDao::AddData(TCHAR* strTableName,list<TCHAR*>& strCloumValue,int nCloumNum)
{
	TCHAR strQuery[100];
	_stprintf(strQuery, _T("select * from [%s]"),strTableName);
	//strQuery.Format(_T("select * from [%s]"),strTableName);

	//自带的字符串类型
	_bstr_t bstrQuery(strQuery);
	_bstr_t bstrCource(m_strConnect);

	HRESULT hres;
	Fields *pFiles = NULL;
	//ado把所有的数据类型都写在VARIANT这里面
	VARIANT varIndex;
	varIndex.vt = VT_I4; //R代表float CY==钱数  VT_带表的都是comADO代表的数据类型

	Field  *pFile = NULL;
	DataTypeEnum dataType;

	VARIANT varValue;
	try
	{
		//打开表
		hres = m_pRecordset->Open(bstrQuery,bstrCource,adOpenDynamic,adLockBatchOptimistic,adCmdText);
		if (!SUCCEEDED(hres))
		{
			//m_strErrorMsg = _T("打开表失败");
			_stprintf(m_strErrorMsg, _T("打开表失败"));
			return FALSE;
		}
		else
		{
			//计算添加记录的行数
			int nRosNum = (int)(strCloumValue.size()/nCloumNum);
			if(nRosNum <= 0 )
			{
				//m_strErrorMsg = _T("参数个数错误");
				//AfxMessageBox(_T("输入参数错误，请重新输入"));
				_stprintf(m_strErrorMsg, _T("参数个数错误"));
				return FALSE;
			}
			//添加记录 循环行
			for (int i=0; i<nRosNum; i++)
			{
				//记录集告诉要添加记录
				m_pRecordset->AddNew();
				//要把记录一列一列的写进去
				for (int ncloum=0;ncloum<nCloumNum;ncloum++)
				{
					//传入的都是字符串 我要先得到列的数据类型 然后在做相应的转换 再把数据添加到数据库里
					//在数据库里列都是以集合形式存在的，所以先要得到所有的列，在从中得到每一个列
					//Fields *pFiles 是个双指针
					//1.得到所有的列
					m_pRecordset->get_Fields(&pFiles);

					//2.得到具体的列
					//ADO的COM对数据库编程 除了定义了字符串其他的没有具体定义 ado把所有的数据类型都写在VARIANT这里面
					varIndex.intVal = ncloum;
					pFiles->get_Item(varIndex,&pFile);

					//3.得到列(字段)对应的数据类型
					pFile->get_Type(&dataType);

					//4.转换 CString--》对应的类型 0 1 2 3 4 5   3
					//TODO:
					//if (StringToDataType(strCloumValue.GetAt(ncloum+i*nCloumNum),dataType,&varValue))
					if (StringToDataType(GetPos(strCloumValue, ncloum+i*nCloumNum),dataType,&varValue))
					{
						//5.添加记录
						pFile->put_Value(varValue);
						if (varValue.vt == (VT_UI1|VT_ARRAY))
						{
							SafeArrayDestroy(varValue.parray);
						}
					}
					
				}
				//更新数据
				m_pRecordset->UpdateBatch(adAffectCurrent);

			}
			//关闭记录集 如果不关闭就会被一直占用，就打不开了
			//if (pStrReturnID)
			//{
			//	m_pRecordset->MoveLast();
			//	m_pRecordset->get_Fields(&pFiles);

			//	varIndex.intVal = 0;
			//	pFiles->get_Item(varIndex,&pFile);
			//	VARIANT varKey;
			//	pFile->get_Value(&varKey);

			//	pStrReturnID->Format(_T("%d"),varKey.intVal);
			//}

			m_pRecordset->Close();
		}
	}catch(_com_error *e)
	{
		//AfxMessageBox(e->Description());
		//m_strErrorMsg.Format(_T("%s"),e->Description());
		_stprintf(m_strErrorMsg, _T("%s"),e->Description());
		return FALSE;
	}

	return TRUE;
}

//字符串转换为指定类型
BOOL CMyDao::StringToDataType(TCHAR* strValue,int nDataType, VARIANT *pVar)
{
	switch(nDataType)
	{
	case adInteger: //整形
		pVar->vt = VT_I2;
		pVar->intVal = _ttoi(strValue);
		break;
	case adBoolean: //BOOL类型
		pVar->vt = VT_BOOL;
		pVar->bVal = _ttoi(strValue);
		break;
	case adSingle:   //单精度
		pVar->vt = VT_R4;
		pVar->fltVal = (float)_tstof(strValue);
		break;
	case adDouble:  //双精度
		pVar->vt = VT_R8;
		pVar->dblVal = _tstof(strValue);
		break;
	case adBSTR:   //字符串
	case adChar:
	case adVarChar:
	case adVarWChar:	
	case adWChar:
	case adLongVarWChar:
	case adLongVarChar:
		pVar->vt = VT_BSTR;
		pVar->bstrVal = (bstr_t)strValue;
		break;
	default:
		pVar->vt = VT_EMPTY;
		break;
	}
	return TRUE;
}

//获取数据
BOOL CMyDao::GetData(TCHAR* strSql,list<TCHAR*>& strQueryCloum,int nCloumNum,list<TCHAR*>& strDataValue)
{
	HRESULT het;
	//_bstr_t    字符串    		_variant_t 多种数据类型集合 vt类型   ..value值								//com
	_bstr_t bstrSql = strSql;				//执行sql
	_bstr_t bstrConnect = m_strConnect;		//连库串
	_bstr_t bstrQueryCloum;					//列名

	Fields *fields;
	FieldPtr fieldPtr;

	TCHAR strValue[100];
	_variant_t varBLOB;

	try
	{
		het = m_pRecordset->Open(bstrSql,bstrConnect,adOpenDynamic,adLockOptimistic,adCmdText);
		if (!SUCCEEDED(het))
		{
			//AfxMessageBox(_T("打开表失败"));
			//m_strErrorMsg = _T("查询表失败");
			_stprintf(m_strErrorMsg, _T("查询表失败"));
			return FALSE;
		}
		//循环结果
		  list<TCHAR*>::iterator it;
		   //行
		while(!m_pRecordset->ADOEOF)
		{
			//	列
			it = strQueryCloum.begin();
			for(int i=0;i<nCloumNum,it != strQueryCloum.end();i++,it++)
			{
				bstrQueryCloum = *it;
				//		获得所有列
				fields = m_pRecordset->GetFields();
					  //获得指定列
				fieldPtr = fields->GetItem(bstrQueryCloum);
					 //获得列对应的值
				varBLOB = fieldPtr->GetValue();
				
				//long nSize =fieldPtr->ActualSize;
				
				
				//注意图片名称必须为图片流的前一个字段
				//strValue = DataToCStringType(varBLOB,nSize,strValue);
				//strValue = DataToCStringType(varBLOB);
				_stprintf(strValue, _T("%s"), DataToStringType(varBLOB));
				TCHAR* pCopy = new TCHAR[_tcslen(strValue)+1];
				_tcscpy(pCopy, strValue);
				strDataValue.insert(strDataValue.begin(), pCopy);
				 
				//varBLOB.Detach();
			}
			m_pRecordset->MoveNext();
		}

		m_pRecordset->Close();
	}
	catch(_com_error *e)
	{
		//m_strErrorMsg = e->ErrorMessage();
		//AfxMessageBox(e->ErrorMessage());
		_stprintf(m_strErrorMsg, _T("%s"), e->ErrorMessage());
	}

	return TRUE;
}

//数据转字符串
TCHAR* CMyDao::DataToStringType(VARIANT Var)
{
	TCHAR strValue[100];
	switch(Var.vt)
	{
	case VT_BOOL:
		//strValue.Format(_T("%d"), Var.boolVal);
		_stprintf(strValue, _T("%d"), Var.boolVal);
		break;
	case VT_I4:
		//strValue.Format(_T("%d"), Var.intVal);
		_stprintf(strValue, _T("%d"), Var.intVal);
		break;
	case VT_R8:
	case VT_DECIMAL:
		//strValue.Format(_T("%0.2f"), Var.dblVal);
		_stprintf(strValue, _T("%0.2f"), Var.dblVal);
		break;
	case VT_BSTR:
		_stprintf(strValue, _T("%s"), Var.bstrVal);
		//strValue= Var.bstrVal;
		break;
	default:
		_stprintf(strValue, _T(""));
		break;
	}

	return strValue;
}
//修改数据
BOOL CMyDao::EditData(TCHAR* strSQL, int nColumnNum, list<TCHAR*>& pStrFieldName, list<TCHAR*> &saValue)
{
	int nIndex = 0;		//列的索引
	int nCount = 0;		//修改的总数
	int nTotal = (int)saValue.size();  //一共修改的个数

	HRESULT hRet;
	_variant_t varFieldName;
	_variant_t varValue;

	_bstr_t bstrQuery = strSQL;
	_bstr_t bstrConnent = m_strConnect;
	try
	{
		hRet = m_pRecordset->Open(bstrQuery, bstrConnent, adOpenDynamic, adLockOptimistic, adCmdText);
		if(!SUCCEEDED(hRet))
		{
			_tcscpy(m_strErrorMsg, _T("打开表失败"));
			//m_strErrorMsg = _T("打开表失败！");
			return FALSE;
		}
		if (m_pRecordset->ADOEOF)
		{
			_tcscpy(m_strErrorMsg, _T("没有选择修改的行"));
			//m_strErrorMsg = _T("没有选择修改的行");
			m_pRecordset->Close();
			return FALSE;
		}
		Fields *pFiles = NULL;
		//ado把所有的数据类型都写在VARIANT这里面
		VARIANT varIndex;
		varIndex.vt = VT_I4; //R代表float CY==钱数  VT_带表的都是comADO代表的数据类型

		Field  *pFile = NULL;
		list<TCHAR*>::iterator it = pStrFieldName.begin();
		list<TCHAR*>::iterator itValue = saValue.begin();
		while(!m_pRecordset->ADOEOF)
		{
			////修改哪一列  s#  sname    1 'ss'  2 'ss2'   stringarr  4    2
			//varFieldName.SetString((bstr_t)pStrFieldName[nIndex]);
			varFieldName.SetString((bstr_t)(*it));

			//varValue.SetString((bstr_t)saValue.GetAt(nCount));
			varValue.SetString((bstr_t)(*itValue));

			m_pRecordset->Update(varFieldName,varValue);
			nIndex++;
			it++;
			nCount++;
			itValue++;
			if(nIndex>=nColumnNum)
			{
				nIndex = 0;
				it = pStrFieldName.begin();
				m_pRecordset->MoveNext();
			}

			if (nColumnNum == 1)
			{
				if(nCount == nTotal)
				{
					break;
				}
			}
			else
			{
				if(nCount > nTotal)
				{
					break;
				}
			}
		}
		m_pRecordset->Close();
	}
	catch(_com_error *e)
	{
		//m_strErrorMsg = e->ErrorMessage();
		//AfxMessageBox(e->Description());
		_stprintf(m_strErrorMsg, _T("%s"), e->Description());
		return FALSE;
	}

	return TRUE;
}

//删除数据
BOOL CMyDao::DeleteData(TCHAR* strSQL)
{
	HRESULT hRet;

	_bstr_t bstrQuery = strSQL;
	_bstr_t bstrConnent = m_strConnect;

	try
	{
		hRet = m_pRecordset->Open(bstrQuery, bstrConnent, adOpenDynamic, adLockOptimistic, adCmdText);
		if(!SUCCEEDED(hRet))
		{
			_tcscpy(m_strErrorMsg, _T("打开表失败"));
			//m_strErrorMsg = _T("打开表失败！");
			return FALSE;
		}
		while(!m_pRecordset->ADOEOF)
		{
			m_pRecordset->Delete(adAffectCurrent);
			m_pRecordset->MoveNext();
		}

		m_pRecordset->Close();
	}
	catch(_com_error *e)
	{
		//AfxMessageBox(e->Description());
		_stprintf(m_strErrorMsg, _T("%s"), e->Description());
		return FALSE;
	}
	
	return TRUE;
}