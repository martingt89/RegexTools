RegexTools
==========

Little c++ wrapper for POSIX regex

ONLY FOR LINUX

Namespace
=========

Regex

Using
=====

class Regex

		Regex::Regex(const std::string& expression, const bool caseIgnore) throw (RegexException)
			expression - regular expression
			caseIgnore - if case sensitive or not
		
		bool Regex::search(const std::string& text)
			return true if search pattern in  text
		
		bool Regex::search(const std::string& text, std::string::size_type &start, std::string::size_type &end)
			return true if search pattern in text and set start to first match letter and end to first letter after match
			
		Matcher Regex::getMatcher(std::string text)
			return object Matcher that will works with text

class Matcher

		bool Matcher::find()
			true if match regular expresion in text after last text match
		
		std::string Matcher::getGroup(unsigned int number)
			if find return true - getGroup return groupe with number "number"
			
class RegexException
		
		- inherits from std::exception

			

