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

#ifndef COLLADA_PARSER_VISUAL_SCENE_H_
#define COLLADA_PARSER_VISUAL_SCENE_H_


#include <ColladaParser/Config.h>
#include <ColladaParser/Node.h>


/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	/**
	 * Visual scene.
	 * 
	 * A visual scene defines the hierarchy of the geometry objects as
	 * well as any transformations it may have. It encapsulates data
	 * from the '<visual_scene>' element of the Collada specification.
	 */
	class COLLADA_PARSER_API VisualScene
	{
	public:
		/**
		 * Constructor.
		 * Reads the 'visual_scene' element from the given XML node.
		 * @param element The XML node to parse.
		 */
		VisualScene (ticpp::Element* element);
		
		/**
		 * Destructor.
		 */
		~VisualScene ();
		
		
		/**
		 * The visual scene ID.
		 */
		std::string getID() const { return mID; }
		
		/**
		 * The visual scene name.
		 */
		std::string getName() const { return mName; }
		
		
		/**
		 * A list of nodes in the visual scene.
		 */
		const NodeList& getNodes() const { return mNodes; }
		
		
	private:
		/* properties */
		std::string mID;
		std::string mName;
		
		NodeList mNodes;
		
		
		/* parsing methods */
		void parse (ticpp::Element* element);
	};

}


#endif /* COLLADA_PARSER_VISUAL_SCENE_H_ */
