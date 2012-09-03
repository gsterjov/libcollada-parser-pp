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

#ifndef COLLADA_PARSER_SOURCE_H_
#define COLLADA_PARSER_SOURCE_H_

#include <string>

#include <ColladaParser/Config.h>
#include <ColladaParser/DataSource.h>


/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	class COLLADA_PARSER_API Source : public DataSource
	{
	public:
		Source (ticpp::Element *element);
		~Source ();
		
		
		const std::string &getID()   { return mID; }
		const std::string &getName() { return mName; }
		
		
	private:
		/* source properties */
		std::string mID;
		std::string mName;
		std::vector<float> mData;
		
		Accessor mAccessor;
		
		
		/* data source implementation */
		int getCount() { return mAccessor.count; }
		float getData (int index) { return mData[index]; }
		
		const Accessor &getAccessor() { return mAccessor; }
		
		
		/* parsing methods */
		void parse         (ticpp::Element *element);
		void parseAccessor (ticpp::Element *element);
		
		DataType parseType (std::string type);
	};

}


#endif /* COLLADA_PARSER_SOURCE_H_ */
