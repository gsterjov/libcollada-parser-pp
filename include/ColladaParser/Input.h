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

#ifndef COLLADA_PARSER_COLLADA_INPUT_H_
#define COLLADA_PARSER_COLLADA_INPUT_H_


#include <ColladaParser/Config.h>
#include <ColladaParser/Types.h>
#include <ColladaParser/DataSource.h>


/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	enum COLLADA_PARSER_API InputSemantic
	{
		INPUT_SEMANTIC_UNKNOWN,
		
		INPUT_SEMANTIC_BINORMAL,
		INPUT_SEMANTIC_COLOR,
		INPUT_SEMANTIC_CONTINUITY,
		INPUT_SEMANTIC_IMAGE,
		INPUT_SEMANTIC_INPUT,
		INPUT_SEMANTIC_IN_TANGENT,
		INPUT_SEMANTIC_INTERPOLATION,
		INPUT_SEMANTIC_INV_BIND_MATRIX,
		INPUT_SEMANTIC_JOINT,
		INPUT_SEMANTIC_LINEAR_STEPS,
		INPUT_SEMANTIC_MORPH_TARGET,
		INPUT_SEMANTIC_NORMAL,
		INPUT_SEMANTIC_OUTPUT,
		INPUT_SEMANTIC_OUT_TANGENT,
		INPUT_SEMANTIC_POSITION,
		INPUT_SEMANTIC_TANGENT,
		INPUT_SEMANTIC_TEXBINORMAL,
		INPUT_SEMANTIC_TEXCOORD,
		INPUT_SEMANTIC_TEXTANGENT,
		INPUT_SEMANTIC_UV,
		INPUT_SEMANTIC_VERTEX,
		INPUT_SEMANTIC_WEIGHT
	};
	
	
	
	
	class COLLADA_PARSER_API Indices
	{
	public:
		Indices () { mStride = 1; }
		~Indices () {}
		
		
		void setStride (int stride) { mStride = stride; }
		
		int getCount() { return mIndices.size() / mStride; }
		
		
		/* add index to list */
		void add (int index)
		{
			mIndices.push_back (index);
		}
		
		/* get index from list */
		int get (int index, int offset = 0)
		{
			index *= mStride;
			index += offset;
			return mIndices[index];
		}
		
		
		void reserve (int size) { mIndices.reserve (size); }
		
		
	private:
		/* each offset has its own index map */
		std::vector<int> mIndices;
		int mStride;
	};
	
	
	
	
	class COLLADA_PARSER_API Input : public DataSource
	{
	public:
		Input (ticpp::Element *element, SourceMap &sources);
		~Input ();
		
		
		void setIndices (Indices *indices) { mIndices = indices; }
		
		
		InputSemantic getSemantic() { return mSemantic; }
		unsigned int getOffest() { return mOffset; }
		
		
		void getData (int index, std::vector<float> &data);
		
		
	private:
		InputSemantic mSemantic;
		unsigned int mOffset;
		
		DataSource *mSource;
		Indices *mIndices;
		
		
		
		/* data source implementation */
		int getCount() { return mSource->getCount(); }
		float getData (int index) { return mSource->getData (index); }
		
		const Accessor &getAccessor() { return mSource->getAccessor(); }
		
		
		
		/* parsing methods */
		void parse (ticpp::Element *element, SourceMap &sources);
		
		InputSemantic parseSemantic (const std::string &semantic);
	};

}


#endif /* COLLADA_PARSER_INPUT_H_ */
