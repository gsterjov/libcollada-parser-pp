/*
Copyright (c) 2010 Goran Sterjov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef COLLADA_PARSER_EFFECT_H_
#define COLLADA_PARSER_EFFECT_H_


#include <vector>
#include <string>

#include <ColladaParser/Config.h>
#include <ColladaParser/Profile.h>


/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	typedef std::vector<ProfileCommon*> ProfileCommonList;
	
	
	
	class COLLADA_PARSER_API Effect
	{
	public:
		Effect (ticpp::Element* element);
		~Effect ();
		
		
		std::string getID() { return mID; }
		std::string getName() { return mName; }
		
		const ProfileCommonList& getCommonProfiles() { return mCommonProfiles; }
		
		
	private:
		/* effect properties */
		std::string mID;
		std::string mName;
		
		ProfileCommonList mCommonProfiles;
		
		
		/* parsing methods */
		void parse (ticpp::Element* element);
	};

}


#endif /* COLLADA_PARSER_EFFECT_H_ */
