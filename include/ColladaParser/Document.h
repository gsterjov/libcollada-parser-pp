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

#ifndef COLLADA_PARSER_DOCUMENT_H_
#define COLLADA_PARSER_DOCUMENT_H_


#include <string>
#include <vector>

#include <ColladaParser/Config.h>

#include <ColladaParser/Material.h>
#include <ColladaParser/Effect.h>
#include <ColladaParser/Geometry.h>
#include <ColladaParser/VisualScene.h>



/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	/* list types */
	typedef std::vector<Material*>    MaterialList;
	typedef std::vector<Effect*>      EffectList;
	typedef std::vector<Geometry*>    GeometryList;
	typedef std::vector<VisualScene*> VisualSceneList;
	
	
	
	class COLLADA_PARSER_API Document
	{
	public:
		Document (const std::string& file);
		~Document ();
		
		bool open ();
		
		
		const MaterialList&    getMaterials    () const { return mMaterials; }
		const EffectList&      getEffects      () const { return mEffects; }
		const GeometryList&    getGeometries   () const { return mGeometries; }
		const VisualSceneList& getVisualScenes () const { return mVisualScenes; }
		
		
	private:
		std::string mFile;
		
		MaterialList    mMaterials;
		EffectList      mEffects;
		GeometryList    mGeometries;
		VisualSceneList mVisualScenes;
		
		
		void parseMaterials    (ticpp::Element* element);
		void parseEffects      (ticpp::Element* element);
		void parseGeometries   (ticpp::Element* element);
		void parseVisualScenes (ticpp::Element* element);
	};

}


#endif /* COLLADA_PARSER_DOCUMENT_H_ */
