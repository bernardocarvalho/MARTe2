#!/usr/bin/env python3
__copyright__ = '''
    Copyright 2020 F4E | European Joint Undertaking for ITER and
    the Development of Fusion Energy ('Fusion for Energy').
    Licensed under the EUPL, Version 1.1 or - as soon they will be approved
    by the European Commission - subsequent versions of the EUPL (the "Licence")
    You may not use this work except in compliance with the Licence.
    You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 
    Unless required by applicable law or agreed to in writing, 
    software distributed under the Licence is distributed on an "AS IS"
    basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
    or implied. See the Licence permissions and limitations under the Licence.
'''
__license__ = 'EUPL'
__author__ = 'Andre Neto'
__date__ = '21/09/2020'

##
# Imports
##
from abc import ABCMeta, abstractmethod
from datetime import datetime
from termcolor import colored
import glob

##
# Class definitions
##
class QAReporter(object):
    """ Abstract class for all QA reporters implementation.
    """

    __metaclass__ = ABCMeta

    def __init__(self, logger):
        """ Sets the logger.
        """
        self.logger = logger
        self.msgs = {}
        self.currentHelperName = None
        self.INFO = 'info'
        self.WARNING = 'warning'
        self.ERROR = 'ERROR'
        self.OK = 'ok'

    def Configure(self, args):
        """ Configures the reporter instance.
        
        Args:
            args (dict): module specific parameters. 
        Returns:
            True if all the parameters are successfully loaded.
        """
        pass

    def SetHelper(self, helperName):
        self.currentHelperName = helperName
        if (helperName not in self.msgs):
            self.msgs[helperName] = {
                        self.INFO : [],
                        self.WARNING : [],
                        self.ERROR : [],
                        self.OK : []
                    }

    def WriteInfo(self, info):
        """ Writes standard information.
        
        Args:
            info (str): the information to write.
        """
        self.msgs[self.currentHelperName][self.INFO].append(info)

    def WriteOK(self, info):
        """ Writes positive information.
        
        Args:
            info (str): the information to write.
        """
        self.msgs[self.currentHelperName][self.OK].append(info)

    def WriteWarning(self, info):
        """ Writes warning information.
        
        Args:
            info (str): the information to write.
        """
        self.msgs[self.currentHelperName][self.WARNING].append(info)

    def WriteError(self, info):
        """ Writes error information.
        
        Args:
            info (str): the information to write.
        """
        self.msgs[self.currentHelperName][self.ERROR].append(info)

    def Terminate(self):
        """ Outputs the report information.
        """
        pass

class ConsoleReporter(QAReporter):
    """ QA reporter that outputs to the console.
    """

    def PrintLine(self, prefix, prefixColor, info):
        textPrefix = colored(prefix, prefixColor, attrs=['bold'])
        textInfo = colored(info)
        print('{0} [{1}]'.format(textPrefix, textInfo))

    def Terminate(self):
        COL_WIDTH = 200
        INFO_COLOR = 'grey'
        OK_COLOR = 'green'
        WARN_COLOR = 'yellow'
        ERROR_COLOR = 'red'
        for helper in self.msgs:
            print('{0:-<{1}}'.format('', COL_WIDTH))
            #Colored changes the line length
            print('+ {0: <{1}} +'.format(colored(helper, attrs=['bold']), COL_WIDTH + 4))
            print('{0:-<{1}}'.format('', COL_WIDTH))
            for info in self.msgs[helper][self.INFO]:
                msg = '{0}: {1}'.format(colored('INFO', INFO_COLOR, attrs=['bold']), info)
                print('| {0: <{1}} |'.format(msg, COL_WIDTH + 9))
            for ok in self.msgs[helper][self.OK]:
                msg = '{0}: {1}'.format(colored('OK', OK_COLOR, attrs=['bold']), ok)
                print('| {0: <{1}} |'.format(msg, COL_WIDTH + 9))
            for warn in self.msgs[helper][self.WARNING]:
                msg = '{0}: {1}'.format(colored('WARN', WARN_COLOR, attrs=['bold']), warn)
                print('| {0: <{1}} |'.format(msg, COL_WIDTH + 9))
            for err in self.msgs[helper][self.ERROR]:
                msg = '{0}: {1}'.format(colored('ERROR', ERROR_COLOR, attrs=['bold']), err)
                print('| {0: <{1}} |'.format(msg, COL_WIDTH + 9))
        print('{0:-<{1}}'.format('', COL_WIDTH))

class RedmineReporter(QAReporter):
    """ QA reporter that outputs in a format that is ready to be copy-pasted for redmine.
    """

    def Configure(self, args):
        """ Configures the reporter instance.
        
        Args:
            args (dict): args['reviewauthor'] (str) the name of the person doing the review.
                         args['outputfile'] (str) the name of the file to write.
                         args['repo'] (git.Repo) the git repository.
        Returns:
            True if all the parameters are successfully loaded.
        """
        self.reviewAuthor = args['reviewauthor']
        self.outputFile = args['outputfile']
        self.repo = args['repo']
        return True

    def WriteHelperOutput(self, helper):
        OK_COLOR = 'green'
        WARN_COLOR = 'yellow'
        ERROR_COLOR = 'red'
        ret = '\n*{0}*\n\n'.format(helper)
        errFound = False
        for msg in self.msgs[helper][self.ERROR]:
            msg = msg.rstrip().replace('%', '%%')
            errFound = True
            ret += '%{{color:{0}}}ERR:% {1}\n'.format(ERROR_COLOR, msg)
        for msg in self.msgs[helper][self.WARNING]:
            msg = msg.rstrip().replace('%', '%%')
            errFound = True
            ret += '%{{color:{0}}}WARN:% {1}\n'.format(WARN_COLOR, msg)

        for msg in self.msgs[helper][self.OK]:
            msg = msg.rstrip().replace('%', '%%')
            ret += '%{{color:{0}}}OK:% {1}\n'.format(OK_COLOR, msg)

        if (len(self.msgs[helper][self.INFO]) > 0):
            ret += 'Information\n'
            ret += '<pre>\n'
            for msg in self.msgs[helper][self.INFO]:
                msg = msg.rstrip()
                ret += msg
                ret += '\n'
            ret += '</pre>\n'


        if (not errFound):
            ret += '%{{color:{0}}}OK:% no errors found.\n\n'.format(OK_COLOR)

        return ret

    def Terminate(self):
        DATE_OF_REVIEW_STR = '*Date of the review:* '
        PERSON_REVIEW_STR = '*Person who did the review:* '
        VERSION_OF_REQS_STR = '*Version of requirements:* '
        VERSION_OF_DDD_STR = '*Version of architecture & design document:* '
        VERSION_OF_SRC_STR = '*Version of source code:* '
        RESULT_REVIEW_STR = '*Result of review:* '
        NON_CONFORMITIES_STR = '*List of non-conformities:* '
        VERSION_OF_UNIT_TESTS_STR = '*Version of unit tests:* '
        RESULT_COVERAGE_TEST_STR = '*Result of coverage tests review:* '
        RESULT_FUNCTIONAL_TEST_STR = '*Result of functional tests review:* '

        COMMENTS_STR = '*Comments:* '
        TODO_STR = '%{background:yellow}TODO%'
        NA_STR = 'N/A'
        NEW_LINE = '\n\n'

        now = datetime.now()
        dateStr = now.strftime("%d/%m/%Y")
       
        out = 'DESCRIBE CHANGES {0}{1}'.format(TODO_STR, NEW_LINE)
        out += 'h1. Requirements review{0}'.format(NEW_LINE)
        out += '{0}{1}{2}'.format(DATE_OF_REVIEW_STR, dateStr, NEW_LINE)
        out += '{0}{1}{2}'.format(PERSON_REVIEW_STR, self.reviewAuthor, NEW_LINE)
        out += '{0}{1}{2}'.format(VERSION_OF_REQS_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(RESULT_REVIEW_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(NON_CONFORMITIES_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(COMMENTS_STR, TODO_STR, NEW_LINE)

        out += 'h1. Architecture & design review{0}'.format(NEW_LINE)
        out += '{0}{1}{2}'.format(DATE_OF_REVIEW_STR, dateStr, NEW_LINE)
        out += '{0}{1}{2}'.format(PERSON_REVIEW_STR, self.reviewAuthor, NEW_LINE)
        out += '{0}{1}{2}'.format(VERSION_OF_DDD_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(RESULT_REVIEW_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(NON_CONFORMITIES_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(COMMENTS_STR, TODO_STR, NEW_LINE)

        codeVersion = self.repo.head.object.hexsha
        out += 'h1. Code and documentation review{0}'.format(NEW_LINE)
        out += '{0}{1}{2}'.format(DATE_OF_REVIEW_STR, dateStr, NEW_LINE)
        out += '{0}{1}{2}'.format(PERSON_REVIEW_STR, self.reviewAuthor, NEW_LINE)
        out += '{0}{1}{2}'.format(VERSION_OF_SRC_STR, codeVersion, NEW_LINE)
        out += '{0}{1}{2}'.format(RESULT_REVIEW_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}'.format(NON_CONFORMITIES_STR, NEW_LINE)

        out += '%{{background:red}}Run flexelint and report% {0}{1}'.format(TODO_STR, NEW_LINE)
        out += self.WriteHelperOutput('Headers')
        out += self.WriteHelperOutput('Linter')
        out += self.WriteHelperOutput('Doxygen')

        out += NEW_LINE
        out += 'h1. Unit test review{0}'.format(NEW_LINE)
        out += '{0}{1}{2}'.format(DATE_OF_REVIEW_STR, dateStr, NEW_LINE)
        out += '{0}{1}{2}'.format(PERSON_REVIEW_STR, self.reviewAuthor, NEW_LINE)
        out += '{0}{1}{2}'.format(VERSION_OF_SRC_STR, codeVersion, NEW_LINE)
        out += '{0}{1}{2}'.format(RESULT_COVERAGE_TEST_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(RESULT_FUNCTIONAL_TEST_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(RESULT_REVIEW_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(COMMENTS_STR, TODO_STR, NEW_LINE)
        out += '{0}{1}'.format(NON_CONFORMITIES_STR, NEW_LINE)

        out += self.WriteHelperOutput('Functional tests')
        out += self.WriteHelperOutput('GTest')
        out += self.WriteHelperOutput('Coverage')

        out += NEW_LINE
        out += 'h1. Integration test review{0}'.format(NEW_LINE)
        out += '{0}{1}{2}'.format(DATE_OF_REVIEW_STR, dateStr, NEW_LINE)
        out += '{0}{1}{2}'.format(PERSON_REVIEW_STR, self.reviewAuthor, NEW_LINE)
        out += '{0}{1}{2}'.format(VERSION_OF_SRC_STR, codeVersion, NEW_LINE)
        out += '{0}{1}{2}'.format(RESULT_REVIEW_STR, NA_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(NON_CONFORMITIES_STR, NA_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(COMMENTS_STR, NA_STR, NEW_LINE)

        out += NEW_LINE
        out += 'h1. Acceptance test review{0}'.format(NEW_LINE)
        out += '{0}{1}{2}'.format(DATE_OF_REVIEW_STR, dateStr, NEW_LINE)
        out += '{0}{1}{2}'.format(PERSON_REVIEW_STR, self.reviewAuthor, NEW_LINE)
        out += '{0}{1}{2}'.format(VERSION_OF_SRC_STR, codeVersion, NEW_LINE)
        out += '{0}{1}{2}'.format(RESULT_REVIEW_STR, NA_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(NON_CONFORMITIES_STR, NA_STR, NEW_LINE)
        out += '{0}{1}{2}'.format(COMMENTS_STR, NA_STR, NEW_LINE)

        self.logger.debug(out)
        with open(self.outputFile, 'w') as f:
            f.write(out)
            self.logger.info('Wrote redmine output to {0}'.format(self.outputFile))

class HTMLReporter(QAReporter):
    """ QA reporter that outputs in HTML.
    """

    def Configure(self, args):
        """ Configures the reporter instance.
        
        Args:
            args (dict): args['reviewauthor'] (str) the name of the person doing the review.
                         args['outputfile'] (str) the name of the file to write.
                         args['repo'] (git.Repo) the git repository.
        Returns:
            True if all the parameters are successfully loaded.
        """
        self.reviewAuthor = args['reviewauthor']
        self.outputFile = args['outputfile']
        self.repo = args['repo']
        return True

    def WriteHelperOutput(self, helper):
        OK_COLOR = 'green'
        WARN_COLOR = 'yellow'
        ERROR_COLOR = 'red'
        NEW_LINE = '\n'
        ret = '<h1>{0}</h1>'.format(helper)
        ret += NEW_LINE
        ret += '<table>'
        ret += NEW_LINE
        ret += '<tr><th>Severity</th><th>Message</th></tr>'
        ret += NEW_LINE
        errFound = False
        for msg in self.msgs[helper][self.ERROR]:
            msg = msg.rstrip()
            errFound = True
            ret += '<tr><td>ERROR</td><td>{0}</td></tr>'.format(msg)
            ret += NEW_LINE
        for msg in self.msgs[helper][self.WARNING]:
            msg = msg.rstrip()
            errFound = True
            ret += '<tr><td>WARN</td><td>{0}</td></tr>'.format(msg)
            ret += NEW_LINE

        for msg in self.msgs[helper][self.OK]:
            msg = msg.rstrip()
            ret += '<tr><td>OK</td><td>{0}</td></tr>'.format(msg)
            ret += NEW_LINE

        ret += '</table>'

        if (len(self.msgs[helper][self.INFO]) > 0):
            ret += 'Information'
            ret += NEW_LINE
            ret += '<pre>'
            ret += NEW_LINE
            for msg in self.msgs[helper][self.INFO]:
                msg = msg.rstrip()
                ret += msg
                ret += '\n'
            ret += '</pre>'
            ret += NEW_LINE

        ret += NEW_LINE
        if (not errFound):
            ret += 'No errors found!'
        ret += NEW_LINE

        return ret

    def Terminate(self):
        NEW_LINE = '\n'

        now = datetime.now()
        dateStr = now.strftime("%d/%m/%Y")
        codeVersion = self.repo.head.object.hexsha
       
        out = '<html>'
        out += NEW_LINE
        out += '<p>Report date: {0}</p>'.format(dateStr)
        out += NEW_LINE
        out += '<p>Software version: {0}</p>'.format(codeVersion)
        out += NEW_LINE
        out += '<p>Reviewer: {0}</p>'.format(self.reviewAuthor)
        out += NEW_LINE

        out += self.WriteHelperOutput('Headers')
        out += self.WriteHelperOutput('Linter')
        out += self.WriteHelperOutput('Doxygen')
        out += self.WriteHelperOutput('Functional tests')
        out += self.WriteHelperOutput('GTest')
        out += self.WriteHelperOutput('Coverage')
        out += '</html>'
        out += NEW_LINE
        self.logger.debug(out)
        with open(self.outputFile, 'w') as f:
            f.write(out)
            self.logger.info('Wrote html output to {0}'.format(self.outputFile))

class CompositeReporter(QAReporter):
    """ QA reporter that calls Terminate on all the registered reporters.
    """

    def Configure(self, args):
        """ Configures the reporter instance.
        
        Args:
            args (dict): args['reporters'] ([QAReporter]) all the QA reporters to call.
        Returns:
            True if all the parameters are successfully loaded.
        """
        self.reporters = args['reporters']
        return True

    def Terminate(self):
        for r in self.reporters:
            r.msgs = self.msgs
            r.Terminate()


class CompositeReporter(QAReporter):
    """ QA reporter that calls Terminate on all the registered reporters.
    """

    def Configure(self, args):
        """ Configures the reporter instance.
        
        Args:
            args (dict): args['reporters'] ([QAReporter]) all the QA reporters to call.
        Returns:
            True if all the parameters are successfully loaded.
        """
        self.reporters = args['reporters']
        return True

    def Terminate(self):
        for r in self.reporters:
            r.msgs = self.msgs
            r.Terminate()

