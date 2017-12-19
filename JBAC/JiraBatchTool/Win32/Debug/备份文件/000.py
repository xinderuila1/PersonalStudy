# -*- coding: utf-8 -*-
import os
import exceptions
import sys

from jira import JIRA

jira = JIRA(server='http://pm.glodon.com/jira/', basic_auth=('sunjj', '52zhaodan!'))

#oooo = jira.search_issues("project = GTJ AND issuetype = Crash");

#根据过滤条件查询Issue
def searchCrashInfo(sSqlStr): 
        issueDict = {"1":"a"}
        #recordList = jira.search_issues(sSqlStr)
        #for record in recordList:
        #        issueDict[record.key] = record.fields.summary
        #issue.fields.customfield_15361
        issue = jira.issue('GTJ-27692')
        name = issue.fields.summary
        issueDict[issue.key] = name
        return issueDict


#sSqlStr = "project = GTJ AND issuetype = Crash"
#searchCrashInfo(sSqlStr)