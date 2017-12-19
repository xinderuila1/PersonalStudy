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
g_DumpAddress = ''
g_PdbAddress = ''
g_GehAddress = 'http://a.glodon.com/'
g_CrashFilePath = ''
g_CrashFileName = ''
g_CrashReport = ''
g_CrashReportFile = ''
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
        loginParam = {
                'username':'sunjj',
                'password':'52zhaodan!'}
        
        headers = {
                'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36',
                'Referer':'http://cas.grandsoft.com.cn/login'}
        
        sLoginUrl = "http://cas.grandsoft.com.cn/login?service=http://a.glodon.com/login/cas_callback/?back=/contact_us/"
        
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
        result = urllib2.urlopen(g_GehAddress + 'bug/page?limit=20')      
        jsonResult = json.loads(result.read(), 'UTF-8')
        return jsonResult
#//////////////////////////////////////////

#//4.获取Crash报告信息
#//////////////////////////////////////////  
def crashRoportInfo(sCrashReportAddr):
        result = urllib2.urlopen(sCrashReportAddr).read();
        searchResult = re.search(r'<code>[\w\W]*</code>', result)
        if searchResult :
                useFile = open(g_CrashFilePath,'w')
                useFile.write(searchResult.group(0))
                useFile.close
        
        searchResult = re.search(r"(?<=href=\")/dump_download/.+?(?=\")|(?<=href=\').+?(?=\')", result)
        if searchResult:
                global g_DumpAddress
                g_DumpAddress = g_GehAddress + searchResult.group(0)
        else:
                global g_DumpAddress
                g_DumpAddress = ""
                
        searchResult = re.search(r"(?<=href=\")/pdb_download/.+?(?=\")|(?<=href=\').+?(?=\')", result)
        if searchResult:
                global g_PdbAddress
                g_PdbAddress = g_GehAddress + searchResult.group(0)
        else:
                global g_PdbAddress
                g_PdbAddress = ""
#//////////////////////////////////////////  

#//5.更新Jira的Issue相关
#////////////////////////////////////////// 
def updateIssueInfo(sInfoDict):
        #全局变量
        global g_Issue
        if sInfoDict:
                g_Issue.update({'description': 'test by sunjj today'})
                
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
                        
        #添加附件crashInfo
        if os.path.exists(g_CrashFilePath):
                useFile = open(g_CrashFilePath,'rb')
                jira.add_attachment(g_Issue, useFile, g_CrashFileName)
                useFile.close    
        #添加备注
        global g_DumpAddress
        global g_PdbAddress
        global g_CrashReport
        comment = 'dump Address: ' + g_DumpAddress + '\n\n' + 'pdb Address: ' + g_PdbAddress + '\n\n' + 'Crash Report: ' + g_CrashReport
        jira.add_comment(g_Issue, comment)
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
        delKeys = ['ptime', 'ip', 'send_time', 'report_id']
        for index in range(len(crashReport)):     
                for delKey in delKeys:
                        del crashReport[index][delKey]  
        
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
        
#//分析Issue
#////////////////////////////////////////// 
def analysisIssue():
        global g_Issue
        crashListAddress = g_Issue.fields.customfield_15361
        jsonResult = crashInfoJson(crashListAddress)
        if jsonResult['rows']:
                regix = r"(?<=href=\").+?(?=\")|(?<=href=\').+?(?=\')"
                href = jsonResult['rows'][0]['report_id']
                sCrashReportHref = re.search(regix, href, re.I|re.S|re.M) 
                if sCrashReportHref:          
                        sCrashReportAddr = g_GehAddress + sCrashReportHref.group(0)
                        crashRoportInfo(sCrashReportAddr)
                        filterCrashReport(jsonResult['rows'])

#//更新Issue
#////////////////////////////////////////// 
def updateIssue(sInfoDict):
        updateIssueInfo(sInfoDict)
  
#//分析前操作sExePath
#////////////////////////////////////////// 
def beforeAnalysis(sExePath):
        global g_CrashFileName
        g_CrashFileName = g_Issue.key + '-CrashInfo.txt'
        
        global g_CrashFilePath
        g_CrashFilePath = sExePath + '\\crashInfo\\' + g_CrashFileName  
        
        global g_CrashReportFile
        g_CrashReportFile = sExePath + '\\crashInfo\\' + g_Issue.key + '.json'
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
        
        global g_DumpAddress
        g_DumpAddress = ''
        
        global g_PdbAddress
        g_PdbAddress = ''       
        
        global g_CrashReport
        g_CrashReport = ''
        
        global g_CrashReportFile
        g_CrashReportFile = ''
#//////////////////////////////////////////

        
#//自动登录GEH平台
#//////////////////////////////////////////
loginGEH()

#searchCrashInfo('project = GTJ AND issuetype = Crash')
#for test
#searchIssue("GTJ-32343")
#beforeAnalysis(curPythonFilePath())
#analysisIssue()
#sMap ={'customfield_14262':'18936', 'assignee':'', 'autoTest':'11156'}
#updateIssue(sMap)
#afterAnalysis()
#print "hello"