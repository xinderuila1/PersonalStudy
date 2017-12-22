# -*- coding: utf-8 -*-
# -*- coding: cp936 -*-

'''
   智能分析类，与Jira/GEH做交互
'''

import urllib2
import urllib
import cookielib
import json
import re
import os
import sys 
import codecs

from jira import JIRA

class AnalysisCrashOper:
    '''
    与Jira/GEH做交互
    '''
    
    __jira = None
    __issue = None
    __productCode = ''
    __productVersion = ''    
    __dumpAddress = 'http://crash.glodon.com/file/download/%s/%s/dump/%s.gz'
    __pdbAddress = 'http://crash.glodon.com/file/download/%s/%s/pdb/pdb_x86.7z'
    __crashListAddress = 'http://crash.glodon.com/api/signature/%s/%s/crashes?from=all&to=all&ver_id=%s&m_ver=%s'
    __stackAddress = 'http://crash.glodon.com/api/crash/id?id=%s'
    
    #构造函数 登录Jira/GEH
    def initialise(self, jiraServer, username, password):
        #登录Jira
        self.__jira = JIRA(server=jiraServer, basic_auth=(username, password))
        
        #登录GEH
        cookieJar = cookielib.LWPCookieJar()
        cookie_support = urllib2.HTTPCookieProcessor(cookieJar)
        opener = urllib2.build_opener(cookie_support, urllib2.HTTPHandler)
        urllib2.install_opener(opener)        

        headers = {
            'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36'}
    
        sPostBody = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:sso=\"http://www.glodon.com/xsd/SsoAuthUser\">\n\
               <soapenv:Header/>\n\
               <soapenv:Body>\n\
                  <sso:SsoAuthUserRequest>\n\
                     <sso:userCode>" + username +  "</sso:userCode>\n\
                     <sso:password>" + password + "</sso:password>\n\
                  </sso:SsoAuthUserRequest>\n\
               </soapenv:Body>\n\
            </soapenv:Envelope>"
    
        #安全认证
        sLoginUrl = "http://home.glodon.com:7783/Utilities/Account/ProxyService/SsoAuthUserProcess"
        request = urllib2.Request(sLoginUrl, sPostBody, headers)
        result = urllib2.urlopen(request)
    
        #用户登录
        loginParam = {'domainAccount':username, 'password':password}
        sLoginUrl = "http://crash.glodon.com/api/userLogin"
    
        postData = urllib.urlencode(loginParam)
        request = urllib2.Request(sLoginUrl, postData, headers)
        result = urllib2.urlopen(request)     
        
    
    #查询Crash集
    def searchCrashInfo(self, sSqlStr): 
        issueDict = {}
        fields = 'key, customfield_10901'
        recordList = self.__jira.search_issues(sSqlStr, 0, 10000, fields)
        for record in recordList:
            issueDict[record.key] = record.fields.customfield_10901
        return issueDict  
    
    #查找Crash
    def searchIssue(self, sCrashKey):
        self.__issue = self.__jira.issue(sCrashKey)
        
    #预处理
    def beforeAnalysis(self, exePath, productCode):
        self.__productCode = productCode
        self.__productVersion = self.__issue.fields.customfield_10212
    
    #后处理
    def afterAnalysis(self):
        self.__productCode = ''
        self.__productVersion = ''
        self.__issue = None
    
    #版本号
    def productVersion(self):
        return self.__productVersion

    #栈信息
    def crashStack(self, sStackId):
        if sStackId:
            jsonResult = json.loads(urllib2.urlopen(self.__stackAddress % sStackId).read(), 'UTF-8')
            stackJson = json.dumps(jsonResult, ensure_ascii = False,indent = 4)    
            return stackJson
        return ""

    #分析Crash
    def analysisIssue(self):
        crashListAddr = self.__issue.fields.customfield_10901
        urllib2.urlopen(crashListAddr)
        searchResult = re.findall(r"(\d+)", crashListAddr)
        if searchResult:
            crashList = self.__crashListAddress % (searchResult[0], self.__productCode, searchResult[2], searchResult[2])
            result = urllib2.urlopen(self.__crashListAddress % (searchResult[0], self.__productCode, searchResult[2], searchResult[2]))
            jsonResult = json.loads(result.read(), 'UTF-8')
            jsonResult = json.dumps(jsonResult, ensure_ascii = False,indent = 4)
            return jsonResult        
        return ""
    
    #更新Jira
    def updateIssue(self, sInfoDict):
        if sInfoDict:
            remark = ''
            stackInfo = ''
            if sInfoDict['customfield_10801']:
                #归属团队
                self.__issue.update({'customfield_10801': {'id': sInfoDict['customfield_10801']}})
            if sInfoDict['assignee']:
                #归属人员
                self.__issue.update({'assignee': {'name' : sInfoDict['assignee']}})
            if sInfoDict['description']:
                #描述信息
                self.__issue.update({'description': sInfoDict['description']})
            if sInfoDict['remark']:
                #备注信息
                remark = sInfoDict['remark']
            if sInfoDict['autoTest']:
                #是自动化
                self.__issue.fields.labels.append(u'自动化')
                self.__issue.update(fields={"labels": self.__issue.fields.labels})
            if sInfoDict['highFrequency']:
                #高频次Crash
                self.__issue.fields.labels.append(u'高频次')
                self.__issue.update(fields={"labels": self.__issue.fields.labels})   
            if sInfoDict['crashCount']:
                #崩溃次数
                self.__issue.fields.labels.append(sInfoDict['crashCount'])
                self.__issue.update(fields={"labels": self.__issue.fields.labels})
            if sInfoDict['stackInfo']:
                #堆栈信息
                stackInfo = sInfoDict['stackInfo']
            if sInfoDict['dumpUrl']:
                #dump及pdb地址    
                crash_DumpAddress = self.__dumpAddress % (self.__productCode, self.__productVersion, sInfoDict['dumpUrl'])
                crash_PdbAddress = self.__pdbAddress % (self.__productCode, self.__productVersion)                
    
                #添加备注
                comment_detail = u'dump下载地址：' + crash_DumpAddress + '\n' + u'pdb 下载地址：' + crash_PdbAddress + '\n\n' + u'Crash详细报告：\n' + remark
                self.__jira.add_comment(self.__issue, comment_detail)  
                
                #堆栈信息
                comment_stack = u'stack堆栈信息：'+ '\n\n' + stackInfo
                self.__jira.add_comment(self.__issue, comment_stack)

        

#for test
#//////////////////////////////////////////
#test = AnalysisCrashOper()
#test.initialise('http://pm.glodon.com/newjira/', 'sunjj', '52zhaodan!')
#test.searchIssue('GTJTJ-2512')
#sInfoDict = {'remark':"...描述", "customfield_10801":"11122", "assignee":"mazq-a", "description":"", "remark":"", "autoTest":"", "highFrequency":"", "crashCount":"", "stackInfo":"", "dumpUrl":"af73529a-c1b2-4592-8643-911c7e65efab"}
#test.updateIssue(sInfoDict)
#print 'hello'
#//////////////////////////////////////////