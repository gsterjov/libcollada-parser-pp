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

#ifndef COLLADA_PARSER_GEOMETRY_H_
#define COLLADA_PARSER_GEOMETRY_H_


#include <vector>

#include <ColladaParser/Config.h>
#include <ColladaParser/Source.h>
#include <ColladaParser/Input.h>


/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	/* primitive geometry data */
	class COLLADA_PARSER_API Primitive
	{
	public:
		/* primitive type */
		enum Type
		{
			LINES,
			LINE_STRIPS,
			POLYGONS,
			POLYLIST,
			TRIANGLES,
			TRI_FANS,
			TRI_STRIPS
		};
		
		
		Primitive (ticpp::Element* element, SourceMap* sources);
		~Primitive ();
		
		
		const std::string& getName() const { return mName; }
		const std::string& getMaterial() const { return mMaterial; }
		
		
		bool hasNormals() const;
		bool hasTexCoords() const;
		
		Vector   getVertex   (int index) const;
		Vector   getNormal   (int index) const;
		TexCoord getTexCoord (int index) const;
		
		int getIndex (int index) const;
		int getIndexCount() const { return mIndices->getCount(); }
		
		
	private:
		/* primitive properties */
		std::string mName;
		std::string mMaterial;
		
		std::vector<Input*> mInputs;
		Indices* mIndices;
		
		SourceMap* mSources;
		
		
		/* parsing methods */
		void parse (ticpp::Element* element);
	};
	
	
	
	
	
	class COLLADA_PARSER_API Geometry
	{
	public:
		explicit Geometry (ticpp::Element* element);
		~Geometry ();
		
		
		const std::string& getID() const { return mID; }
		const std::string& getName() const { return mName; }
		
		const std::vector<Primitive*>& getPrimitives() const { return mPrimitives; }
		
		
	private:
		/* source properties */
		std::string mID;
		std::string mName;
		
		SourceMap mSources;
		std::vector<Primitive*> mPrimitives;
		
		
		/* parsing methods */
		void parse (ticpp::Element* element);
	};

}


#endif /* COLLADA_PARSER_GEOMETRY_H_ */
