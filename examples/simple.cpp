// compile g++ simple.cpp ../src/regextools.cpp

#include <iostream>
#include "../src/regextools.h"
#include <string>

int main(){

std::string s = "you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation";

RegexTools::Regex reg("GNU ([^ ]+) Public License");

if(reg.search(s)){
	std::cout<<"Found"<<std::endl;
	RegexTools::Matcher matcher = reg.getMatcher(s);
	if(matcher.find()){
		std::cout<<"Type: "<<matcher.getGroup(1)<<std::endl;
	}
}else{
	std::cout<<"Sorry, not found"<<std::endl;
}

return 0;
}
