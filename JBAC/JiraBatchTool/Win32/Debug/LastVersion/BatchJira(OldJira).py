# -*- coding: utf-8 -*-
# -*- coding: cp936 -*-

import urllib2
import urllib
import cookielib
import json
import re
import os
import sys 
import codecs

reload(sys)
sys.setdefaultencoding('utf8')

from jira import JIRA

#登录Jira服务器 jira.search_issues("project = GTJ AND issuetype = Crash");
jira = JIRA(server='http://pm.glodon.com/jira/', basic_auth=('sunjj', '52zhaodan!'))


#//全局变量
#//////////////////////////////////////////
g_Issue = None
g_DumpAddress = 'http://crash.glodon.com/file/download/%s/%s/dump/%s.gz'
g_PdbAddress = 'http://crash.glodon.com/file/download/%s/%s/pdb/pdb_x86.7z'
g_GehAddress = 'http://crash.glodon.com/'
g_StackAddress = 'http://crash.glodon.com/api/crash/id?id=%s'
g_CrashFilePath = ''
g_CrashFileName = ''
g_CrashReport = ''
g_CrashReportFile = ''
g_ProductCode = ''
g_ProductVersion = ''
g_Remark = ''
#//////////////////////////////////////////

#//0.保持会话
#//////////////////////////////////////////     
cookieJar = cookielib.LWPCookieJar()
cookie_support = urllib2.HTTPCookieProcessor(cookieJar)
opener = urllib2.build_opener(cookie_support, urllib2.HTTPHandler)
urllib2.install_opener(opener)
#/////////////////////////////////////////


#//1.登录异常捕获平台
#////////////////////////////////////////// 
def loginGEH():
        headers = {
                'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36'}
        
        sPostBody = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:sso=\"http://www.glodon.com/xsd/SsoAuthUser\">\n\
           <soapenv:Header/>\n\
           <soapenv:Body>\n\
              <sso:SsoAuthUserRequest>\n\
                 <sso:userCode>sunjj</sso:userCode>\n\
                 <sso:password>52zhaodan!</sso:password>\n\
              </sso:SsoAuthUserRequest>\n\
           </soapenv:Body>\n\
        </soapenv:Envelope>"
        
        sLoginUrl = "http://home.glodon.com:7783/Utilities/Account/ProxyService/SsoAuthUserProcess"
        request = urllib2.Request(sLoginUrl, sPostBody, headers)
        result = urllib2.urlopen(request)
        
        loginParam = {
                'domainAccount':'sunjj',
                'password':'52zhaodan!'}
        sLoginUrl = "http://crash.glodon.com/api/userLogin"
        
        postData = urllib.urlencode(loginParam)
        request = urllib2.Request(sLoginUrl, postData, headers)
        result = urllib2.urlopen(request)
#//////////////////////////////////////////


#//2.crash链接地址
#//////////////////////////////////////////    
#def crashListAddress(sCrashKey):
        #issuee = jira.issue(sCrashKey)
        #return issuee.fields.customfield_15361
#//////////////////////////////////////////


#//3.crash报告列表Json
#//////////////////////////////////////////     
def crashInfoJson(sCrashListAddr):
        urllib2.urlopen(sCrashListAddr)
        searchResult = re.search(r"(\d+)", sCrashListAddr);
        if searchResult:
                result = urllib2.urlopen(g_GehAddress + "api/signature/" + searchResult.group(0) + "/crashes?p=1")      
                jsonResult = json.loads(result.read(), 'UTF-8')
                return jsonResult
#//////////////////////////////////////////

#//5.更新Jira的Issue相关
#////////////////////////////////////////// 
def updateIssueInfo(sInfoDict):
        #全局变量
        global g_Issue
        if sInfoDict:
                if sInfoDict['customfield_14262']:
                        #归属团队
                        g_Issue.update({'customfield_14262': {'id': sInfoDict['customfield_14262']}})
                if sInfoDict['assignee']:
                        #归属人员
                        g_Issue.update({'assignee': {'name' : sInfoDict['assignee']}}) 
                if sInfoDict['autoTest']:
                        #是自动化
                        g_Issue.update({'customfield_11364': {'id': sInfoDict['autoTest']}})
                if sInfoDict['description']:
                        #描述信息
                        g_Issue.update({'description': sInfoDict['description']})
                if sInfoDict['remark']:
                        #备注信息
                        global g_Remark
                        g_Remark = sInfoDict['remark']
                if sInfoDict['highFrequency']:
                        #高频次Crash
                        g_Issue.fields.labels.append(u'高频次')
                        g_Issue.update(fields={"labels": g_Issue.fields.labels})                        
                        
        #添加附件crashInfo
        if os.path.exists(g_CrashFilePath):
                useFile = open(g_CrashFilePath,'rb')
                jira.add_attachment(g_Issue, useFile, g_CrashFileName)
                useFile.close    
                
        #dump及pdb地址    
        crash_DumpAddress = g_DumpAddress % (g_ProductCode, g_ProductVersion, sInfoDict['dumpUrl'])
        crash_PdbAddress = g_PdbAddress % (g_ProductCode, g_ProductVersion)                

        #添加备注
        comment = 'dump Address: ' + crash_DumpAddress + '\n' + 'pdb Address: ' + crash_PdbAddress + '\n\n' + 'Crash Report: ' + g_Remark
        jira.add_comment(g_Issue, comment)
#////////////////////////////////////////// 

#//5.更新Jira的Issue相关
#////////////////////////////////////////// 
def downloadStack(sStackId):
        if sStackId:
                testaa = g_StackAddress % sStackId
                jsonResult = json.loads(urllib2.urlopen(g_StackAddress % sStackId).read(), 'UTF-8')
                stackJson = json.dumps(jsonResult, ensure_ascii = False,indent = 4)    
                useFile = open(g_CrashFilePath,'w')
                useFile.write(stackJson)
                useFile.close
        
#////////////////////////////////////////// 



#获取脚本文件的当前路径
#////////////////////////////////////////// 
def curPythonFilePath():
        path = sys.path[0]
        #判断为脚本文件还是py2exe编译后的文件，如果是脚本文件，则返回的是脚本的目录，
        #如果是py2exe编译后的文件，则返回的是编译后的文件路径
        if os.path.isdir(path):
                return path
        elif os.path.isfile(path):
                return os.path.dirname(path)

#////////////////////////////////////////// 

#更新CrashReport信息
#////////////////////////////////////////// 
def filterCrashReport(crashReport):
        global g_CrashReport
        g_CrashReport = json.dumps(crashReport, ensure_ascii = False,indent = 4)    
        
        
        if g_CrashReport:
                global g_CrashReportFile
                useFile = open(g_CrashReportFile,'w')
                useFile.write(g_CrashReport)
                useFile.close
#////////////////////////////////////////// 


#以下为实际起作用的函数
#////////////////////////////////////////// 

#根据过滤条件查询Issue
#////////////////////////////////////////// 
def searchCrashInfo(sSqlStr): 
        issueDict = {}
        fields = 'key, customfield_15361'
        recordList = jira.search_issues(sSqlStr, 0, 10000, fields)
        for record in recordList:
                issueDict[record.key] = record.fields.customfield_15361
        return issueDict
#////////////////////////////////////////// 

#//查找Issue
#////////////////////////////////////////// 
def searchIssue(sCrashKey):
        global g_Issue
        g_Issue = jira.issue(sCrashKey)
#////////////////////////////////////////// 

#//分析Issue
#////////////////////////////////////////// 
def analysisIssue():
        global g_Issue
        crashListAddress = g_Issue.fields.customfield_15361
        jsonResult = crashInfoJson(crashListAddress)
        filterCrashReport(jsonResult)
#////////////////////////////////////////// 

#//更新Issue
#////////////////////////////////////////// 
def updateIssue(sInfoDict):
        updateIssueInfo(sInfoDict)
#////////////////////////////////////////// 

#//issue的版本号
#////////////////////////////////////////// 
def productVersion():
        global g_ProductVersion
        return g_ProductVersion
#//////////////////////////////////////////
  
#//分析前操作sExePath
#////////////////////////////////////////// 
def beforeAnalysis(sExePath):
        global g_CrashFileName
        g_CrashFileName = g_Issue.key + '-CrashInfo.txt'
        
        global g_CrashFilePath
        g_CrashFilePath = sExePath + '\\crashInfo\\' + g_CrashFileName  
        
        global g_CrashReportFile
        g_CrashReportFile = sExePath + '\\crashInfo\\' + g_Issue.key + '.json'
        
        global g_ProductCode 
        g_ProductCode = 'gtj2017'
        
        global g_ProductVersion
        g_ProductVersion = g_Issue.fields.customfield_11261
#//////////////////////////////////////////
  
#//分析后操作
#////////////////////////////////////////// 
def afterAnalysis():
        global g_CrashFileName
        g_CrashFileName = ''
        
        global g_CrashFilePath
        g_CrashFilePath = ''        
        
        global g_Issue        
        g_Issue = None      
        
        global g_CrashReport
        g_CrashReport = ''
        
        global g_CrashReportFile
        g_CrashReportFile = ''
        
        global g_ProductCode
        g_ProductCode = ''
        
        global g_ProductVersion
        g_ProductVersion = ''
        
        global g_Remark
        g_Remark = ''
#//////////////////////////////////////////

    
#//自动登录GEH平台
#//////////////////////////////////////////
loginGEH()
#//////////////////////////////////////////

#downloadStack('GTJ-34609')
#searchCrashInfo('project = GTJ AND issuetype = Crash')
#for test
#searchIssue("GTJ-36819")
#beforeAnalysis(curPythonFilePath())
#analysisIssue()
#sMap ={'customfield_14262':'18933', 'assignee':'', 'autoTest':'', 'description':'', 'remark':'', 'dumpUrl':''}
#updateIssue(sMap)
#afterAnalysis()
#print "hello"