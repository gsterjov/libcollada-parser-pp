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

#include "Transform.h"

#include <stdexcept>
#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	Transform::Transform (Type type, ticpp::Element* element) : mType(type)
	{
		parse (element);
	}
	
	
	
	/* parse transform element */
	void Transform::parse (ticpp::Element* element)
	{
		mSID  = element->GetAttributeOrDefault ("sid", "");
		
		/* get transformation data */
		std::string data = element->GetText();
		std::istringstream stream (data);
		
		
		/* parse data according to type */
		switch (mType)
		{
		case TRANSLATE:
			/* parse data: '[x] [y] [z]' */
			stream >> mData.vec.x >> mData.vec.y >> mData.vec.z;
			break;
			
		case SCALE:
			/* parse data: '[x] [y] [z]' */
			stream >> mData.vec.x >> mData.vec.y >> mData.vec.z;
			break;
			
		case ROTATE:
			/* parse data: '[x] [y] [z] [angle]' */
			float x, y, z, angle;
			stream >> x >> y >> z >> angle;
			
			mData.vec.x = 0;
			mData.vec.y = 0;
			mData.vec.z = 0;
			
			if      (x) mData.vec.x = angle;
			else if (y) mData.vec.y = angle;
			else if (z) mData.vec.z = angle;
			
			break;
		}
	}

}
