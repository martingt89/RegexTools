RegexTools
==========

Little c++ wrapper for POSIX regex

ONLY FOR LINUX

Namespace
=========

RegexTools

Using
=====

class Regex

		Regex::Regex(const std::string& expression, const bool caseIgnore) throw (RegexException)
			expression - regular expression
			caseIgnore - if case sensitive or not
		
		bool Regex::search(const std::string& text) const throw (RegexException);
			return true if search pattern is in text

		bool Regex::search(const std::string& text, std::string::size_type &beginMatchPosition,	std::string::size_type &endMatchPosition) const throw (RegexException)
			return true if search pattern in text and set start to first match letter and end to first letter after match
			
		Matcher Regex::getMatcher(std::string text) const
			return object Matcher that will works with text

class Matcher

		bool Matcher::find()
			true if match regular expresion in text after last text match
		
		std::string Matcher::getGroup(unsigned int index) const throw (RegexException);
			if find return true - getGroup return groupe with index "index"
			
class RegexException
		
		- inherits from std::exception

			

