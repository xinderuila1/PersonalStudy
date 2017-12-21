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

class WarningCrashOper(object):
    '''
    与GEH交互/自动发邮件
    '''
    
    __unAnalysisCrashUrl = "http://crash.glodon.com/api/signature/0/%s/crashes?from=%s&to=%s&source=1&s=2000&ver_id=254276&m_ver=254276&search_key=&description=0&bits=0&autotest=0"
    
    #登录GEH
    def __init__(self, username, password):
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
    def downloadUnAnalysisCrash(self, productCode):
        today = datetime.date.today()
        searchDay = self.__unAnalysisCrashUrl % (productCode, today, today)
        jsonResult = json.loads(urllib2.urlopen(searchDay).read(), 'UTF-8')
        crashJson = json.dumps(jsonResult, ensure_ascii=False, indent=4)
        return crashJson 
    
    #发送邮件
    def sendEmailToTesters(sEmailContent, fromAdd, toAdd, contents):
        mail_host = "exchange.grandsoft.com.cn"  # 设置服务器
        fromAdd = "SUNJJ@grandsoft.com.cn"
        toAdd =  ["SUNJJ@grandsoft.com.cn"]  
    
        sender = 'SUNJJ@grandsoft.com.cn'
        receivers = ['SUNJJ@grandsoft.com.cn']   
    
        message = MIMEText(sEmailContent, 'plain', 'utf-8')
        message['Subject'] = Header('GTJ2018土建云计量9.0版本当天高频次崩溃用户统计', 'utf-8')
        message['From'] = fromAdd
        message['To'] = '; '.join(toAdd)  
        
        smtpObj = smtplib.SMTP(mail_host)
        smtpObj.sendmail(sender, receivers, message.as_string())