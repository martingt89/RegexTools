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

namespace RegexTools{

Regex::Regex(const std::string& expression, const bool caseIgnore) throw (RegexException){
	int flags = 0;
	if(caseIgnore){
		flags |= REG_ICASE;
	}
	if (int st = regcomp(&regex, expression.c_str(), flags|REG_EXTENDED) != 0){
		char buffer[100];
		size_t len = regerror(st, &regex, buffer, 100);
		throw RegexException(std::string("Cannot compile regex: ")+expression+" "+std::string(buffer, len));
	}
}

Regex::~Regex() {
	regfree(&regex);
}
bool Regex::search(const std::string& text){
	std::string::size_type x, y;
	return this->search(text, x, y);
}
bool Regex::search(const std::string& text, std::string::size_type &start, std::string::size_type &end){
	regmatch_t pmatch[1];
	int error= regexec(&regex, text.c_str(), 1, pmatch, 0);
	if(!error){
		start = pmatch[0].rm_so;
		end = pmatch[0].rm_eo;
		return true;
	}
	return false;
}
Matcher Regex::getMatcher(std::string text){
	return Matcher(text, this->regex);
}

Matcher::Matcher (std::string text, regex_t regex){
	this->text = text;
	this->regex = regex;
	numberOfGroups = regex.re_nsub + 1;
	groups = (regmatch_t*)malloc(sizeof(regmatch_t)*numberOfGroups);
	lastPosition = 0;
}
Matcher::~Matcher (){
	if(groups)
		free(groups);
}
bool Matcher::find(){

	int status= regexec(&regex, text.c_str()+lastPosition, numberOfGroups, groups, 0);
	if(status == 0){
		backPosition = lastPosition;
		lastPosition = lastPosition + groups[0].rm_so+1;
		return true;
	}
	return false;
}
std::string Matcher::getGroup(unsigned int number){
	if(number >= numberOfGroups){
		return "";
	}
	int n = groups[number].rm_eo - groups[number].rm_so;
	return text.substr(backPosition + groups[number].rm_so, n);
}

RegexException::RegexException(std::string message){
	this->message = message;
}
RegexException::~RegexException() throw (){

}
std::string RegexException::what(){
	return message;
}

}

