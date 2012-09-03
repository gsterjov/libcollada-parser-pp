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

#include "Effect.h"

#include <stdexcept>
#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	Effect::Effect (ticpp::Element* element)
	{
		parse (element);
	}
	
	
	/* destructor */
	Effect::~Effect ()
	{
		
	}
	
	
	
	
	/* parse effect element */
	void Effect::parse (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* effect properties */
		mID   = element->GetAttribute ("id");
		mName = element->GetAttributeOrDefault ("name", "");
		
		
		/* sift through effect elements
		 * asset[0-1], annotate[*], newparam[*], profile[1-*], extra[*] */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			std::string val = iter->Value();
			ticpp::Element* child = iter.Get();
			
			
			/* found a profile */
			if (val == "profile_COMMON")
				mCommonProfiles.push_back (new ProfileCommon (child));
		}
	}

}
