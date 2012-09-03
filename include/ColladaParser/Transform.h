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

#ifndef COLLADA_PARSER_TRANSFORM_H_
#define COLLADA_PARSER_TRANSFORM_H_


#include <string>

#include <ColladaParser/Config.h>
#include <ColladaParser/Types.h>


/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	/**
	 * Transform class.
	 * 
	 * A transform class holds information about how to transform a specific
	 * element encountered within the Collada document. This class can hold
	 * all possible kinds of transformations and should be applied to
	 * the element in the order found within the document.
	 */
	class COLLADA_PARSER_API Transform
	{
	public:
		/**
		 * Transform type.
		 */
		enum Type { LOOK_AT, MATRIX, ROTATE, SCALE, SKEW, TRANSLATE };
		
		
		/**
		 * Constructor.
		 * Reads a transform element from the given XML node.
		 * @param type The type of the transform element.
		 * @param element The XML node to parse.
		 */
		Transform (Type type, ticpp::Element* element);
		
		
		/**
		 * The transform type.
		 * @return a transform Type.
		 */
		Type getType() const { return mType; }
		
		
		/**
		 * The transform SID.
		 * @return A scoped identifier.
		 */
		const std::string& getSID() const { return mSID; }
		
		
		/**
		 * The transform translation vector.
		 */
		const Vector& getTranslation() { return mData.vec; }
		
		
		/**
		 * The transform scale vector.
		 */
		const Vector& getScale() { return mData.vec; }
		
		
		/**
		 * The transform rotation vector.
		 */
		const Vector& getRotation() { return mData.vec; }
		
		
	private:
		Type mType;
		std::string mSID;
		
		
		union
		{
			Vector vec;
		} mData;
		
		
		/* parsing methods */
		void parse (ticpp::Element* element);
	};

}


#endif /* COLLADA_PARSER_TRANSFORM_H_ */
