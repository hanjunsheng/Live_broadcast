#pragma once

#ifndef __INCLUDE_MY_DAO_H__
#define __INCLUDE_MY_DAO_H__
#include <windows.h>
#include <list>
#include <stdio.h>
#include <tchar.h>
using namespace std;

#pragma warning(disable:4018 4996 4172)

#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" no_namespace rename("EOF","ADOEOF") rename("BOF","ADOBOF")
#define DATEBASE_TYPE_ACCESS	0
#define DATEBASE_TYPE_SQL2000   1
#define DATEBASE_TYPE_SQL2005   2
#define DATEBASE_TYPE_OTHER		3

#define DEF_MAX_STR_LEN			(100)

class CMyDao
{
public:
	CMyDao();
	~CMyDao();
	BOOL InitCom();        //初始化com组件
	void releaseCom();     //释放com组件

public:
	//初始化数据库
	BOOL OpenDateBase(TCHAR* strDateBaseName,int nDataType,TCHAR* strUser,TCHAR* strPassWord,TCHAR* strIP);

	//关闭数据库
	void CloseDataBase();

	//添加数据
	BOOL AddData(TCHAR* strTableName,list<TCHAR*>& strCloumValue,int nCloumNum);
	//字符串转换为指定类型
	BOOL StringToDataType(TCHAR* strValue,int nDataType, VARIANT *pVar);

	//获取数据
	BOOL GetData(TCHAR* strSql,list<TCHAR*>& strQueryCloum,int nCloumNum,list<TCHAR*>& strDataValue);

	//数据转字符串
	TCHAR* DataToStringType(VARIANT Var);

	//修改数据
	BOOL EditData(TCHAR* strSQL, int nColumnNum, list<TCHAR*>& pStrFieldName, list<TCHAR*> &saValue);

	//删除数据
	BOOL DeleteData(TCHAR* strSQL);

private:
	//获取指定位置的值
	TCHAR* GetPos(list<TCHAR*>& str, int iPos)
	{
		if (str.size() <= iPos)
		{
			return NULL;
		}
		list<TCHAR*>::iterator it = str.begin();
		int iCount = 0;
		while (it != str.end())
		{
			if (iCount++ == iPos)
			{
				break;
			}
			it++;
		}

		return *it;
	}

private:
	_ConnectionPtr m_pConnection;  //连接数据库的指针对象
	_RecordsetPtr  m_pRecordset;   //操作记录集的指针对象
	_CommandPtr    m_pCommandmsg;  //SQL命令的指针对象

	BOOL m_binitComFlag;
	TCHAR m_strErrorMsg[DEF_MAX_STR_LEN];         //错误信息
	TCHAR m_strConnect[DEF_MAX_STR_LEN];          //连接串
};

#endif //__INCLUDE_MY_DAO_H__

