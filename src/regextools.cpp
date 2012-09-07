/* ---------------------------------------------------------------------------
** Author: Martin Geier
** regextools.cpp is part of RegexTools.
**
** RegexTools is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
** -------------------------------------------------------------------------*/

#include "regextools.h"
#include <cstdlib>

namespace RegexTools{

static const int REGERROR_BUFFER_SIZE = 100;
static const int REGEXEC_MATCH = 0;

Regex::Regex(const std::string& expression, const bool caseIgnore) throw (RegexException){
	int flags = 0;
	if(caseIgnore){
		addIgnorCaseFlag(flags);
	}
	addExtendedRegexFlag(flags);
	if (int regcompReturnStatus = regcomp(&regex, expression.c_str(), flags) != 0){
		std::string regcompErrorMessage = getRegexErrorMessage(regcompReturnStatus, regex);
		throw RegexException(std::string("Regcomp error: ")+regcompErrorMessage+", regex expression: "+expression);
	}
}

Regex::~Regex() {
	regfree(&regex);
}
bool Regex::search(const std::string& text) const throw (RegexException){
	std::string::size_type x, y;
	return this->search(text, x, y);
}
bool Regex::search(const std::string& text, 
					std::string::size_type &beginMatchPosition,
					std::string::size_type &endMatchPosition) const throw (RegexException){
	regmatch_t pmatch[1];
	int regexecReturnStatus = regexec(&regex, text.c_str(), 1, pmatch, 0);
	
	if(regexecReturnStatus != REGEXEC_MATCH){
		if(regexecReturnStatus == REG_NOMATCH) return false;
		std::string regexecErrorMessage = getRegexErrorMessage(regexecReturnStatus, regex);
		throw RegexException("Regexec error: "+regexecErrorMessage);
	}
	beginMatchPosition = pmatch[0].rm_so;
	endMatchPosition = pmatch[0].rm_eo;
	return true;
}

Matcher Regex::getMatcher(const std::string& text) const{
	return Matcher(text, this->regex);
}

std::string Regex::getRegexErrorMessage(const int& errorStatus, const regex_t& regex) const{
	char buffer[REGERROR_BUFFER_SIZE+1] = {0};
	size_t messageLength = regerror(errorStatus, &regex, buffer, REGERROR_BUFFER_SIZE);
	return std::string(buffer, messageLength);
}

void Regex::addIgnorCaseFlag(int& flags) {
	flags |= REG_ICASE;
}
void Regex::addExtendedRegexFlag(int& flags) {
	flags |= REG_EXTENDED;
}

//----------------------------Matcher------------------------------//
Matcher::Matcher (const std::string& text, const regex_t& regex){
	this->text = text;
	this->regex = regex;
	numberOfGroups = regex.re_nsub + 1;
	groups = 0;
	groups = (regmatch_t*)malloc(sizeof(regmatch_t)*numberOfGroups);
	lastPosition = 0;
	backPosition = 0;
}
Matcher::~Matcher (){
	if(groups)
		free(groups);
}
bool Matcher::find(){
	bool isMatch = false;
	int status = regexec(&regex, text.c_str()+lastPosition, numberOfGroups, groups, 0);
	if(status == REGEXEC_MATCH){
		isMatch = true;
		backPosition = lastPosition;
		lastPosition = lastPosition + groups[0].rm_so+1;
	}
	return isMatch;
}
std::string Matcher::getGroup(const unsigned int& index) const throw (RegexException){
	if(index >= numberOfGroups){
		throw RegexException("Regexec error: group index is not valid");
	}
	if(groups[index].rm_eo == -1 || groups[index].rm_so == -1){
		throw RegexException("Regexec error: group not match");
	}
	int groupTextLength = groups[index].rm_eo - groups[index].rm_so;

	std::string groupText = "";
	try{
		groupText = text.substr(backPosition + groups[index].rm_so, groupTextLength);
	}catch(const std::exception& ex){
		throw RegexException(std::string("Regexec error: ")+ex.what());
	}
	return groupText;
}
bool Matcher::getGroup(const unsigned int& index, std::string& groupText) const{
	if(index >= numberOfGroups || groups[index].rm_eo == -1 || groups[index].rm_so == -1){
		return false;
	}

	int groupTextLength = groups[index].rm_eo - groups[index].rm_so;
	try{
		groupText = text.substr(backPosition + groups[index].rm_so, groupTextLength);
	}catch(const std::exception& ex){
		return false;
	}
	return true;
}
RegexException::RegexException(const std::string& message){
	this->message = message;
}
RegexException::~RegexException() throw (){
}

const char* RegexException::what() const throw (){
	return message.c_str();
}

}

