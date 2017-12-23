# -*- coding: utf-8 -*-
# -*- coding: cp936 -*-

'''
  智能分析类，与GEH交互/自动发邮件
'''

import urllib2
import urllib
import cookielib
import datetime
import json
import re
import os

import smtplib
from email.mime.text import MIMEText
from email.header import Header

import sys
import codecs
reload(sys)
sys.setdefaultencoding('utf8')

fromAdd = "SUNJJ@grandsoft.com.cn"
toAdd = ['HUANGJ-D@glodon.com', 'XIAT@glodon.com', 'ZHAOFH-A@glodon.com', 'wul-b@glodon.com', 'shiqn@glodon.com', 'FANXN@glodon.com', 'wangwj-a@glodon.com','SUNJJ@glodon.com']
toCsr = ['houj@glodon.com', 'zhuy@glodon.com', 'xiell@glodon.com', 'ZHANGYC-C@glodon.com', 'ZHANGH-AA@glodon.com', 'YANGWL-A@glodon.com', 'SHENC@glodon.com']

#fromAdd = "SUNJJ@grandsoft.com.cn"
#toAdd = ["SUNJJ@grandsoft.com.cn"]  
#toCsr = ["SUNJJ@grandsoft.com.cn"]  

class WarningCrashOper():
    '''
    与GEH交互/自动发邮件
    '''
    
    __strartCrashday = "2017-01-01"
    __unAnalysisCrashUrl = "http://crash.glodon.com/api/signature/0/%s/crashes?from=%s&to=%s&source=1&s=2000&ver_id=%s&m_ver=%s&search_key=&description=0&bits=0&autotest=0"
    __allVersionInfoUrl = "http://crash.glodon.com/api/dau?source=1&product=%s&version=%s&startDate=%s&endDate=%s&sub_product=all"
    __crashVersionInfoUrl = "http://crash.glodon.com/api/crash/daily?outer=1&product=%s&version=%s&start=%s&end=%s&sub_product=all"
    
    #登录GEH
    def initialise(self, username, password):
        #保持会话
        cookieJar = cookielib.LWPCookieJar()
        cookie_support = urllib2.HTTPCookieProcessor(cookieJar)
        opener = urllib2.build_opener(cookie_support, urllib2.HTTPHandler)
        urllib2.install_opener(opener)        
        
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36'}
        
        sPostBody = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:sso=\"http://www.glodon.com/xsd/SsoAuthUser\">\n\
               <soapenv:Header/>\n\
               <soapenv:Body>\n\
                  <sso:SsoAuthUserRequest>\n\
                     <sso:userCode>" + username + "</sso:userCode>\n\
                     <sso:password>" + password + "</sso:password>\n\
                  </sso:SsoAuthUserRequest>\n\
               </soapenv:Body>\n\
            </soapenv:Envelope>"
    
        #安全认证
        sLoginUrl = "http://home.glodon.com:7783/Utilities/Account/ProxyService/SsoAuthUserProcess"
        request = urllib2.Request(sLoginUrl, sPostBody, headers)
        result = urllib2.urlopen(request)
    
        #身份认证
        loginParam = {'domainAccount':username, 'password': password}
        sLoginUrl = "http://crash.glodon.com/api/userLogin"
    
        #登录GEH
        postData = urllib.urlencode(loginParam)
        request = urllib2.Request(sLoginUrl, postData, headers)
        result = urllib2.urlopen(request)       
        
    #未分析异常
    def downloadUnAnalysisCrash(self, productCode, versonId):
        today = datetime.date.today()
        searchDay = self.__unAnalysisCrashUrl % (productCode, today, today, versonId, versonId)
        jsonResult = json.loads(urllib2.urlopen(searchDay).read(), 'UTF-8')
        crashJson = json.dumps(jsonResult, ensure_ascii=False, indent=4)
        return crashJson 
    
    #版本基本情况
    def versionBasicInfo(self, productCode, version, beginDay):
        startToday = datetime.date.today()
        endToday = datetime.date.today()
        if beginDay == "":
            startToday = self.__strartCrashday
        searchDay = self.__allVersionInfoUrl % (productCode, version, startToday, endToday)
        jsonResult = json.loads(urllib2.urlopen(searchDay).read(), 'UTF-8')
        crashJson = json.dumps(jsonResult, ensure_ascii=False, indent=4)
        return crashJson         
        
    #版本崩溃情况
    def versionCrashInfo(self, productCode, version, beginDay): 
        startToday = datetime.date.today()
        endToday = datetime.date.today()
        if beginDay == "":
            startToday = self.__strartCrashday
        searchDay = self.__crashVersionInfoUrl % (productCode, version, startToday, endToday)
        jsonResult = json.loads(urllib2.urlopen(searchDay).read(), 'UTF-8')
        crashJson = json.dumps(jsonResult, ensure_ascii=False, indent=4)
        return crashJson           
        
    #发送邮件
    def sendEmailToTesters(self, htmlFile, headers):
        fileObject = open(htmlFile, 'r')
        fileString = fileObject.read()
        fileObject.close()
        htmlContent = fileString.decode('gbk')   

        strFrom = fromAdd
        strTo = '; '.join(toAdd)        
        strCs = '; '.join(toCsr)
        
        mail_head = headers + u'版本当天高频次崩溃用户统计'
        mail_host = "exchange.grandsoft.com.cn"  # 设置服务器
        message = MIMEText(htmlContent, 'html', 'utf-8')
        message['Subject'] = Header(mail_head, 'utf-8')
        message['From'] = strFrom
        message['To'] = strTo
        message['Cc'] = strCs
        
        smtpObj = smtplib.SMTP(mail_host)
        smtpObj.sendmail(fromAdd, toAdd, message.as_string())
    
#///////////////////////////////////////////////
#ooo = WarningCrashOper()
#ooo.initialise("sunjj", "52zhaodan!")
#ooo.versionBasicInfo("gtj2017", "1.0.9.0", "")
#print "hello"
#///////////////////////////////////////////////

        

    