#ifndef INPUT_H
#define INPUT_H

#include <cml/cml.h>
#include "InputListener.h"

namespace control
{
	class CInput  
	{
	public:
		CInput();
		~CInput();	

		GLvoid update(UINT message, WPARAM wParam, LPARAM lParam);
	
		cml::vector2i getMousePos();

		bool isKeyDown(char key);		
		bool isLeftMouseDown();
		bool isRightMouseDown();

		GLvoid setMousePos(GLint xpos, GLint ypos);

		GLvoid registerListener(control::CInputListener *listener);

	private:
		bool keys[256];
		bool lmbd,rmbd;
		
		std::vector<control::CInputListener*>			registeredListeners;
		std::vector<control::CInputListener*>::iterator rlIter;
	};
};

#endif // INPUT_H
