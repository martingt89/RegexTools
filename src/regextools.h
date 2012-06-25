/* ---------------------------------------------------------------------------
** Author: Martin Geier
** regextools.h is part of RegexTools.
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

#ifndef REGEXTOOLS_H_
#define REGEXTOOLS_H_

#include <regex.h>
#include <string>
#include <exception>

namespace RegexTools{

class RegexException : std::exception{
public:
	RegexException(std::string message);
	virtual ~RegexException() throw ();
	virtual std::string what();
private:
	std::string message;
};

class Matcher{
public:
	Matcher (std::string text, regex_t regex);
	~Matcher();
	bool find();
	std::string getGroup(unsigned int  number);
private:
	regex_t regex;
	regmatch_t *groups;
	std::string text;
	unsigned int numberOfGroups;
	unsigned long int lastPosition;
	unsigned long int backPosition;
};

class Regex {
public:
	Regex(const std::string& expression, const bool caseIgnor = false)
			throw (RegexException);
	virtual ~Regex();
	bool search(const std::string& text, std::string::size_type &start,
			std::string::size_type &end) throw (RegexException);
	bool search(const std::string& text) throw (RegexException);
	Matcher getMatcher(std::string text);
private:
	regex_t regex;
	std::string text;
};

}

#endif /* REGEXTOOLS_H_ */

