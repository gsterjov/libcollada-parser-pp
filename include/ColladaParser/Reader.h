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

#ifndef COLLADA_PARSER_READER_H_
#define COLLADA_PARSER_READER_H_


#include <string>
#include <vector>

#include <ColladaParser/Config.h>
#include <ColladaParser/Effect.h>
#include <ColladaParser/Geometry.h>
#include <ColladaParser/VisualScene.h>



/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	/* document info from quick parse */
	struct COLLADA_PARSER_API DocumentInfo
	{
		/* geometry element info */
		struct COLLADA_PARSER_API GeometryInfo
		{
			std::string id;
			std::string name;
		};
		
		
		/* document elements */
		std::vector<GeometryInfo> geometries;
	};
	
	
	
	
	class COLLADA_PARSER_API ReaderHandler
	{
	public:
		virtual void loadEffect (Effect* effect) {}
		virtual void loadGeometry (Geometry* geometry) {}
		virtual void loadVisualScene (VisualScene* scene) {}
	};
	
	
	class COLLADA_PARSER_API Reader
	{
	public:
		Reader (const std::string& file, ReaderHandler* handler);
		~Reader ();
		
		
		bool open ();
		
		DocumentInfo getInfo ();
		
		
	private:
		std::string mFile;
		ReaderHandler *mHandler;
		
		
		void parseEffects      (ticpp::Element* element);
		void parseGeometries   (ticpp::Element* element);
		void parseVisualScenes (ticpp::Element* element);
	};

}


#endif /* COLLADA_PARSER_READER_H_ */
