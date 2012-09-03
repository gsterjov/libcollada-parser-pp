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

#ifndef COLLADA_PARSER_PROFILE_H_
#define COLLADA_PARSER_PROFILE_H_


#include <string>

#include <ColladaParser/Config.h>


/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	struct COLLADA_PARSER_API Colour
	{
		float r,g,b,a;
	};
	
	
	
	
	class COLLADA_PARSER_API ProfileCommon
	{
	public:
		/* common properties to all shader elements */
		struct COLLADA_PARSER_API ShaderCommon
		{
			Colour emission;
			Colour reflective;
			Colour transparent;
			
			float reflectivity;
			float transparency;
			float indexOfRefraction;
		};
		
		
		/* lambert shader element */
		struct COLLADA_PARSER_API Lambert : ShaderCommon
		{
			Colour ambient;
			Colour diffuse;
		};
		
		/* phong shader element */
		struct COLLADA_PARSER_API Phong : Lambert
		{
			Colour specular;
			float shininess;
		};
		
		
		/* constant shader element */
		typedef ShaderCommon Constant;
		
		/* blinn shader element */
		typedef Phong Blinn;
		
		
		
		/* common profile technique */
		struct COLLADA_PARSER_API Technique
		{
			std::string id;
			std::string sid;
			
			Constant* constant;
			Lambert*  lambert;
			Phong*    phong;
			Blinn*    blinn;
			
			Technique () : constant(0), lambert(0), phong(0), blinn(0) {}
		};
		
		
		
		ProfileCommon (ticpp::Element* element);
		~ProfileCommon ();
		
		
		std::string getID() { return mID; }
		const Technique& getTechnique() { return mTechnique; }
		
		
	private:
		std::string mID;
		Technique mTechnique;
		
		void parse (ticpp::Element* element);
		void parseShaderCommon (ticpp::Element* element, ShaderCommon* shader);
		
		void parseBlinn    (ticpp::Element* element);
		void parseConstant (ticpp::Element* element);
		void parseLambert  (ticpp::Element* element);
		void parsePhong    (ticpp::Element* element);
	};

}


#endif /* COLLADA_PARSER_PROFILE_H_ */
