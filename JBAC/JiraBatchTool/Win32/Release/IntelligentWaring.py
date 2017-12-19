# -*- coding: utf-8 -*-
# -*- coding: cp936 -*-

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


g_UnAnalysisCrashUrl = "http://crash.glodon.com/api/signature/0/gtj2017/crashes?from=%s&to=%s&source=1&s=2000&ver_id=254276&m_ver=254276&search_key=&description=0&bits=0&autotest=0"
#  //0.保持会话
# //////////////////////////////////////////
cookieJar = cookielib.LWPCookieJar()
cookie_support = urllib2.HTTPCookieProcessor(cookieJar)
opener = urllib2.build_opener(cookie_support, urllib2.HTTPHandler)
urllib2.install_opener(opener)
# /////////////////////////////////////////


# //1.登录异常捕获平台
# //////////////////////////////////////////
def loginGEH():
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36'}

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
        'domainAccount': 'sunjj',
        'password': '52zhaodan!'}
    sLoginUrl = "http://crash.glodon.com/api/userLogin"

    postData = urllib.urlencode(loginParam)
    request = urllib2.Request(sLoginUrl, postData, headers)
    result = urllib2.urlopen(request)
# //////////////////////////////////////////

# //2.下载未分析的异常信息
# //////////////////////////////////////////
def downloadUnAnalysisCrash():
    today = datetime.date.today()
    searchDay = g_UnAnalysisCrashUrl % (today, today)
    jsonResult = json.loads(urllib2.urlopen(searchDay).read(), 'UTF-8')
    crashJson = json.dumps(jsonResult, ensure_ascii=False, indent=4)
    return crashJson
# //////////////////////////////////////////

# //3. 发送邮件
# //////////////////////////////////////////
def sendEmailToTesters(sEmailContent):
    mail_host = "exchange.grandsoft.com.cn"  # 设置服务器
    mail_user = "SUNJJ@grandsoft.com.cn"  # 用户名
    mail_pass = "52zhaodan!"  # 口令
    
    fromAdd = "SUNJJ@grandsoft.com.cn"
    toAdd =  ['wangwj-a@glodon.com', 'HUANGJ-D@glodon.com', 'XIAT@glodon.com', 'SUNJJ@glodon.com', 'ZHANGYC-C@glodon.com', 'ZHANGH-AA@glodon.com', 'YANGWL-A@glodon.com', 'SHENC@glodon.com']
    
    sender = 'SUNJJ@grandsoft.com.cn'
    receivers = ['wangwj-a@glodon.com', 'HUANGJ-D@glodon.com', 'XIAT@glodon.com', 'SUNJJ@glodon.com', 'ZHANGYC-C@glodon.com', 'ZHANGH-AA@glodon.com', 'YANGWL-A@glodon.com', 'SHENC@glodon.com']  
    
    message = MIMEText(sEmailContent, 'plain', 'utf-8')
    message['Subject'] = Header('GTJ2018土建云计量9.0版本当天高频次崩溃用户统计', 'utf-8')
    message['From'] = fromAdd
    message['To'] = '; '.join(toAdd)
    message['Cc'] = "xiell@glodon.com"    
    
    
    smtpObj = smtplib.SMTP(mail_host)
    smtpObj.sendmail(sender, receivers, message.as_string())
   
    return
# //////////////////////////////////////////

#//自动登录GEH平台
#//////////////////////////////////////////
loginGEH()
#//////////////////////////////////////////