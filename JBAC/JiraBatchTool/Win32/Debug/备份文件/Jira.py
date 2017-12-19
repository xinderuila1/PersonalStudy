# -*- coding: utf-8 -*-

from jira import JIRA

jira = JIRA(server='http://pm.glodon.com/jira/', basic_auth=('sunjj', '52zhaodan!'))

oooo = jira.search_issues("project = GTJ AND issuetype = Crash");

#根据过滤条件查询Issue
def searchCrashInfo(sSqlStr):
        issueDict = {}
        recordList = jira.search_issues(sSqlStr)
        for record in recordList:
                issueDict[record.key] = record.fields.summary
                print record.key + ':' + record.fields.summary
        return issueDict


sSqlStr = "project = GTJ AND issuetype = Crash"
searchCrashInfo(sSqlStr)